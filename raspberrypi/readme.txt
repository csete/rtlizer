
These instructions are preliminary and work in progress.

The objective is to make rtlizer work on the Rapsberry Pi equipped with
a tiny Adafruit piTFT touch screen. I can only test with a Raspberry Pi
model B and the original piTFT, so any help and feedback is welcome.

The status is that it works but there is no support for the buttons yet.
You can, however, use the arrow keys on a standard keyboard attached to
the second USB port of the Pi.

Start with an SD card prepared with piTFT support. Do not enable login
to X session. After boot you should end up with a text screen showing
boot messages and a login prompt.

Install the packages necessary to build rtl-sdr:

  $ sudo apt-get install cmake libusb-1.0-0-dev

Then build and install rtl-sdr:

  $ git clone git://git.osmocom.org/rtl-sdr.git
  $ cd rtl-sdr
  $ mkdir build
  $ cd build
  $ cmake -DDETACH_KERNEL_DRIVER=on -DINSTALL_UDEV_RULES=on ..
  $ make
  $ sudo make install
  $ sudo ldconfig
  $ cd

Now, power down the Pi, attach the rtl-sdr dongle and power up the Pi
again. Test the rtl-sdr dongle using the rtl_test application.

Install the packages necessary to build and run rtlizer:

  $ sudo apt-get install libgtk-3-dev

Download and build rtlizer:

  $ wget https://github.com/csete/rtlizer/archive/v2.1.tar.gz -O rtlizer-2.1.tar.gz
  $ tar xvf rtlizer-2.1.tar.gz
  $ cd rtlizer-2.1
  $ make -C src
  $ sudo cp src/rtlizer /usr/local/bin/

Test rtlizer using:

  $ xinit /usr/local/bin/rtlizer


TODO: Automatic start after boot.

