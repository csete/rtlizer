Rtlizer
=======

Rtlizer is a simple spectrum analyzer application written using rtlsdr, kiss_fft and Gtk+. It was originally created as a Beaglebone+rtlsdr demo, but it will probably run on anything with rtlsdr and Gtk+ installed.

<a href="http://www.flickr.com/photos/csete/8474999050/" title="Beaglebone rtlizer"><img src="http://farm9.staticflickr.com/8385/8474999050_4d418b232d.jpg" width="500" height="254" alt="Beaglebone rtlizer"></a>

You can see a short video demo on YouTube: https://www.youtube.com/watch?v=jzmFXreuFR4

Dependencies
------------

* [rtlsdr](http://sdr.osmocom.org/trac/wiki/rtl-sdr)
* [Gtk+ v2](http://www.gtk.org/)
* [kiss_fft](http://kissfft.sourceforge.net/) (included)

Build
-----

There are no makefiles, just two simple scripts containing a gcc command line:

* build - for building on i386-like computers
* build-target - for building rtlizer directly on the Beaglebone or compatible boards

Usage
-----

You can basically run rtlizer as is in an X11 environment. The default window size is set to 320x240 but can be overridden using X Window System geometry string, e.g.

  rtlizer 640x360+0+0

On the Beaglebone I am running my own built linux with X but no display or window manager. In this case I start rtlizer using the xinit command.
Recipes for creating such linux images are available under the [nanosdr project](https://github.com/csete/nanosdr).

The file rtlizer.service is used to autostart the application on the Beaglebone using systemd.

Credits
-------

Rtlizer was written by Alexandru Csete.
Uses code from rtl_test by Steve Markgraf.
Includes kiss_fft by Mark Borgerding.

