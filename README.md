Rtlizer
=======

Rtlizer is a simple spectrum analyzer application written using rtlsdr,
kiss_fft and Gtk+. It was originally created as a Beaglebone + rtlsdr demo,
but it will probably run on anything with rtlsdr and Gtk+ installed.

<a href="http://www.flickr.com/photos/csete/8474999050/" title="Beaglebone rtlizer"><img src="http://farm9.staticflickr.com/8385/8474999050_4d418b232d.jpg" width="500" height="254" alt="Beaglebone rtlizer"></a>

You can see a short video demo on YouTube:
https://www.youtube.com/watch?v=jzmFXreuFR4

Dependencies
------------

* [rtlsdr](http://sdr.osmocom.org/trac/wiki/rtl-sdr)
* [Gtk+ v3](http://www.gtk.org/)
* [kiss_fft](http://kissfft.sourceforge.net/) (included)

Building
--------

The source code is located in the src/ subdirectory. You can imply type "make"
in this directory and the program should build on any platform where the
dependencies are met. The build process will generate a single executable
called rtlizer and you can copy this to a location of your choice.

Instructions for setting rtlizer up on specific targets are available for:
* Beaglebone running Debian 7 console image


Usage
-----

You can basically run rtlizer as is in any X11 environment. The default window
size is set to 320x240 but can be overridden using X Window System geometry
string, e.g.

  rtlizer 640x360+0+0

There are also other command line options are available, see rtlizer -h.

You can use the following keys while rtlizer is running:

    LEFT    Increase frequency
    RIGHT   Decrease frequency
    UP      Increase sample rate
    DOWN    Decrease sample rate
    ENTER   Exit


Credits
-------

Rtlizer was written by Alexandru Csete.
Uses code from rtl_test by Steve Markgraf.
Includes kiss_fft by Mark Borgerding.
