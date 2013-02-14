Rtlizer
=======

Rtlizer is a simple spectrum analyzer application written using rtlsdr, kiss_fft and Gtk+. It was originally created as a Beaglebone+rtlsdr demo, but it will probably run on anything with rtlsdr and Gtk+ installed.

You can see a brief demo on thei YouTube video: _link to be added_

Dependencies
------------

* [rtlsdr](http://sdr.osmocom.org/trac/wiki/rtl-sdr)
* [Gtk+ v2](http://www.gtk.org/)
* [kiss_fft](http://kissfft.sourceforge.net/) (included)

Build
-----

There are no makefiles, just two simple scripts containing a gcc command line:

* build - for building on i386-like computers
* build-target - for building rtlizer directly on the Beaglebone or copatible boards

Usage
-----

You can basically run rtlizer as is in an X11 environment. The default window size is set to 320x240 but can be overridden using X Window System geometry string, e.g.

  rtlizer 640x360+0+0

On the Beaglebone I am running my own built linux with X but no display or window manager. In this case I start rtlizer using the xinit command. This linux is available under the nanosdr project.

The file rtlizer.service is used to autostart the application on the Beaglebone using systemd.

Credits
-------

Rtlizer was written by Alexandru Csete.
Uses code from rtl_test by Steve Markgraf.
Includes kiss_fft by Mark Borgerding.

