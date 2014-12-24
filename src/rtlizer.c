/*
 * rtlizer - a simple spectrum analyzer using rtlsdr
 * 
 * Copyright (C) 2013 Alexandru Csete
 * 
 * Includes code from rtl_test.c:
 * Copyright (C) 2012 by Steve Markgraf <steve@steve-m.de>
 *
 * rtlizer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rtlizer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libfcd.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cairo.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <rtl-sdr.h>
#include <stdlib.h>
#include <unistd.h>

#include "kiss_fft.h"


#define DEFAULT_SAMPLE_RATE 2048000
#define DYNAMIC_RANGE 90.f  /* -dBFS coreresponding to bottom of screen */
#define SCREEN_FRAC 0.7f  /* fraction of screen height used for FFT */

uint8_t *buffer;
uint32_t dev_index = 0;
uint32_t frequency = 98000000;
uint32_t samp_rate = DEFAULT_SAMPLE_RATE;
uint32_t buff_len = 2048;
int ppm_error = 0;

int fft_size = 320;
kiss_fft_cfg  fft_cfg;
kiss_fft_cpx *fft_in;
kiss_fft_cpx *fft_out;
float         *log_pwr_fft; /* dbFS relative to 1.0 */
float scale;
int yzero = 0;
int text_margin = 0;

static rtlsdr_dev_t *dev = NULL;
static gint width, height; /* screen width and height */
static gboolean freq_changed = TRUE;



static gboolean delete_event(GtkWidget *widget, GdkEvent *e, gpointer d)
{
    return FALSE;
}

static void destroy(GtkWidget *widget, gpointer data)
{
   	rtlsdr_close(dev);
	free(buffer);
    
    free(fft_cfg);
    free(fft_in);
    free(fft_out);
    free(log_pwr_fft);

    gtk_main_quit();
}

gint keypress_cb(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	guint event_handled = TRUE;
    int r;
	
	switch (event->key.keyval)
	{
	case GDK_KEY_Return:
		/* exit application */
		gtk_widget_destroy(widget);
		break;

	case GDK_KEY_Left:
		/* increase frequency */
        frequency += samp_rate/4;
        r = rtlsdr_set_center_freq(dev, frequency);
        if (r < 0)
            fprintf(stderr, "WARNING: Failed to set center freq.\n");
        break;

	case GDK_KEY_Right:
		/* decrease frequency */
        frequency -= samp_rate/4;
        r = rtlsdr_set_center_freq(dev, frequency);
        if (r < 0)
            fprintf(stderr, "WARNING: Failed to set center freq.\n");
        break;

	case GDK_KEY_Up:
		/* increase bandwidth with 256 kHz */
        if (samp_rate < 2304000)
        {
            samp_rate += 256000;
            r = rtlsdr_set_sample_rate(dev, samp_rate);
            if (r < 0)
                fprintf(stderr, "WARNING: Failed to set sample rate.\n");
        }
        break;

	case GDK_KEY_Down:
		/* decrease bandwidth with 256 kHz */
        if (samp_rate > 1024000)
        {
            samp_rate -= 256000;
            r = rtlsdr_set_sample_rate(dev, samp_rate);
            if (r < 0)
                fprintf(stderr, "WARNING: Failed to set sample rate.\n");
        }
        break;

	default:
		event_handled = FALSE;
		break;
	}

	return event_handled;
}

static int db_to_pixel(float dbfs)
{
    return yzero+(int)(-dbfs*scale);
}

static void draw_text(cairo_t *cr)
{
    cairo_text_extents_t cte;
    double txt1_y, txt2_y;

    gchar *freq_str = g_strdup_printf("%.3f MHz", 1.e-6f*(float)frequency);
    gchar *delta_str = g_strdup_printf("BW: %.1f kHz   RBW: %.2f kHz",
                                       1.e-3f*(float)samp_rate,
                                       1.e-3f*(float)(samp_rate/fft_size));

    /* clear area */
    cairo_set_source_rgb(cr, 0.02, 0.02, 0.09);
    cairo_set_line_width(cr, 1.0);
    cairo_rectangle(cr, 0, 0, width, yzero);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);

    cairo_select_font_face(cr, "Sans",
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 24);
    cairo_text_extents(cr, freq_str, &cte);
    txt1_y = text_margin + cte.height;
    cairo_set_source_rgba(cr, 0.97, 0.98, 0.02, 0.8);
    cairo_move_to(cr, text_margin, txt1_y);
    cairo_show_text(cr, freq_str);

    cairo_set_font_size(cr, 12);
    cairo_text_extents(cr, delta_str, &cte);
    txt2_y = txt1_y + cte.height + text_margin;
    cairo_set_source_rgba(cr, 0.97, 0.98, 0.02, 0.8);
    cairo_move_to(cr, text_margin, txt2_y);
    cairo_show_text(cr, delta_str);

    g_free(freq_str);
    g_free(delta_str);
}

static void draw_fft(cairo_t *cr)
{
    cairo_set_source_rgb(cr, 0.02, 0.02, 0.09);
    cairo_set_line_width(cr, 1.0);

    cairo_rectangle(cr, 0, yzero, width, height);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);

    //cairo_set_source_rgba(cr, 0.49, 0.50, 0.63, 0.85);
    cairo_set_source_rgba(cr, 0.7, 0.7, 0.7, 0.9);

    int x, y;
    for (x = 0; x < width; x++ )
    {
        y = db_to_pixel(log_pwr_fft[x]);
        g_random_int_range(10, 100);
        cairo_move_to(cr, x, height);
        cairo_line_to(cr, x, y);
    }

    cairo_stroke(cr);    
}

static void setup_rtlsdr()
{
    int device_count;
    int r;

    buffer = malloc(buff_len * sizeof(uint8_t));

    device_count = rtlsdr_get_device_count();
    if (!device_count)
    {
        fprintf(stderr, "No supported devices found.\n");
        exit(1);
    }

    r = rtlsdr_open(&dev, dev_index);
    if (r < 0)
    {
        fprintf(stderr, "Failed to open rtlsdr device #%d.\n", dev_index);
        exit(1);
    }

    if ( ppm_error != 0 ) {
        r = rtlsdr_set_freq_correction(dev, ppm_error);
        if (r < 0)
            fprintf(stderr, "WARNING: Failed to set PPM error.\n");
    }

    r = rtlsdr_set_sample_rate(dev, samp_rate);
    if (r < 0)
        fprintf(stderr, "WARNING: Failed to set sample rate.\n");

    r = rtlsdr_set_center_freq(dev, frequency);
    if (r < 0)
        fprintf(stderr, "WARNING: Failed to set center freq.\n");

    r = rtlsdr_set_tuner_gain_mode(dev, 0);
    if (r < 0)
		fprintf(stderr, "WARNING: Failed to enable automatic gain.\n");

    r = rtlsdr_reset_buffer(dev);
    if (r < 0)
        fprintf(stderr, "WARNING: Failed to reset buffers.\n");

}

static gboolean read_rtlsdr()
{
    gboolean error = FALSE;
    int n_read;
    int r;

    r = rtlsdr_read_sync(dev, buffer, buff_len, &n_read);
    if (r < 0) {
        fprintf(stderr, "WARNING: sync read failed.\n");
        error = TRUE;
    }

    if ((uint32_t)n_read < buff_len) {
        fprintf(stderr, "Short read (%d / %d), samples lost, exiting!\n", n_read, buff_len);
        error = TRUE;
    }

    return error;
}

static void run_fft()
{   
    int i;
    kiss_fft_cpx pt;
    float pwr;
    
    for (i = 0; i < fft_size; i++)
    {
        fft_in[i].r = ((float)buffer[2*i])/255.f;
        fft_in[i].i = ((float)buffer[2*i+1])/255.f;
    }
    kiss_fft(fft_cfg, fft_in, fft_out);
    for (i = 0; i < fft_size; i++)
    {
        /* shift, normalize and convert to dBFS */
        if (i < fft_size / 2)
        {
            pt.r = fft_out[fft_size/2+i].r / fft_size;
            pt.i = fft_out[fft_size/2+i].i / fft_size;
        }
        else
        {
            pt.r = fft_out[i-fft_size/2].r / fft_size;
            pt.i = fft_out[i-fft_size/2].i / fft_size;
        }
        pwr = pt.r * pt.r + pt.i * pt.i;
        
        log_pwr_fft[i] = 10.f * log10(pwr + 1.0e-20f);
    }
}

gint timeout_cb(gpointer darea)
{
	/* get samples from rtlsdr */
	if (read_rtlsdr())
        return FALSE;  /* error reading -> exit */
    
	/* calculate FFT */
    run_fft();
	
	/* update plot */
	cairo_t *cr;
	cr = gdk_cairo_create(gtk_widget_get_window(GTK_WIDGET(darea)));
	draw_fft(cr);
    if (freq_changed)
    {
        draw_text(cr);
        //freq_changed = FALSE;
    }
	cairo_destroy(cr);
	
	return TRUE;
}

int main(int argc, char *argv[])
{
    /* GtkWidget is the storage type for widgets */
    GtkWidget *window;
    guint  tid;
    int opt;
    
    gtk_init (&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);

    g_signal_connect(window, "delete-event", G_CALLBACK (delete_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK (destroy), NULL);
    g_signal_connect(window, "key_press_event", G_CALLBACK (keypress_cb), NULL);

    /* parse cmd line */
    while ((opt = getopt(argc, argv, "d:f:p:h")) != -1) {
        switch (opt) {
            case 'd':
                dev_index = atoi(optarg);
                break;
            case 'f':
                frequency = atoll(optarg);
                break;
            case 'p':
                ppm_error = atoi(optarg);
                break;
            case 'h':
            case '?':
            default:
                printf("usage: rtlizer [-d device_index] [-f frequency [Hz]] [-p ppm_error] [WIDTHxHEIGHT+XOFF+YOFF]\n");
                exit(EXIT_SUCCESS);
                break;
        }
    }

    /* default window size if no geometry is specified */
    width = 320;  //gdk_screen_width();
    height = 240; //gdk_screen_height();
    if (argc > optind)
    {
        if (!gtk_window_parse_geometry(GTK_WINDOW(window), argv[optind]))
            fprintf(stderr, "Failed to parse '%s'\n", argv[optind]);
        else
            gtk_window_get_default_size(GTK_WINDOW(window), &width, &height);
    }

    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    scale = (float)height/DYNAMIC_RANGE * SCREEN_FRAC;
    yzero = (int)(height*(1.0f-SCREEN_FRAC));
    text_margin = yzero/10;

    g_print("window size: %dx%d pixels\n", width, height);
    g_print("SCALE: %.2f / Y0: %d / TXTMARG: %d\n", scale, yzero, text_margin);

    gtk_widget_show(window);
    gdk_window_set_cursor(gtk_widget_get_window(window), gdk_cursor_new(GDK_BLANK_CURSOR));

    /* set up FFT */
    fft_size = 2 * width/2;
    fft_cfg = kiss_fft_alloc(fft_size, FALSE, NULL, NULL);
    fft_in = malloc(width * sizeof(kiss_fft_cpx));
    fft_out = malloc(width * sizeof(kiss_fft_cpx));
    log_pwr_fft = malloc(width * sizeof(float));

    setup_rtlsdr();

    tid = g_timeout_add(50, timeout_cb, window);
    
    gtk_main();
    
    g_source_remove(tid);
    
    return 0;
}

