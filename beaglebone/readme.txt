
Introduction

This document describes how to setup rtlizer on the Beaglebone. It has
been tested with the following items:

 - Beaglebone Black and White
 - LCD3 cape
 - 4DCAPE_43
 - An rtlsdr dongle with R820T tuner (other tuners should work as well)
 - Beaglebone Debian 7.7 console image 2014-12-19, 2014-12-31

The whole system fits well in 2 GB flash (SD or eMMC) and requires about
50 MB RAM. I have only tested using SD cards, however, I expect it would
work the same way if the OS is flashed into the eMMC.


Prepare the OS

Create an SD card with the Beaglebone Debian 7 console image from:
  http://elinux.org/Beagleboard:BeagleBoneBlack_Debian

The image I tested with is:
  bone-debian-7.7-console-armhf-2014-12-19-2gb.img

If you are using the eMMC-flasher, flash it now.
If you are using an SD card make sure to run the grow_partition.sh
script to take advantage of the whole SD card:

  $ sudo /opt/scripts/tools/grow_partition.sh
  $ sudo reboot

Update the installed packages to their latest version:

  $ sudo apt-get update
  $ sudo apt-get upgrade

Install additional packages required to build rtl-sdr:

  $ sudo apt-get install build-essential cmake git libusb-1.0-0-dev

Install additional packages required by rtlizer:

  $ sudo apt-get install xinit xserver-xorg libgtk-3-dev

This will take a while and install 220 new packages but when done you
are ready to build rtl-sdr and rtlizer.


Build and install rtl-sdr

The following steps will fetch the latest rtl-sdr code from their git
repository, compile it and install it:

  $ git clone git://git.osmocom.org/rtl-sdr.git
  $ cd rtl-sdr
  $ mkdir build
  $ cd build
  $ cmake -DDETACH_KERNEL_DRIVER=on -DINSTALL_UDEV_RULES=on ..
  $ make
  $ sudo make install
  $ sudo ldconfig
  $ cd

Plugin your rtl dongle into the USB port and test it using the rtl_test
application.


Build and install rtlizer

  $ wget https://github.com/csete/rtlizer/archive/v2.0.tar.gz -O rtlizer-2.0.tar.gz
  $ tar xvf rtlizer-2.0.tar.gz
  $ cd rtlizer-2.0
  $ make -C src

There should be no errors or warnings and there should be a binary 
called src/rtlizer. Copy it to /usr/local/bin/ to have it together with
the rtl-sdr programs:

  $ sudo cp src/rtlizer /usr/local/bin/

You can now test that rtlizer works:

  $ sudo xinit /usr/local/bin/rtlizer

You should see the spectrum on the LCD. You can terminate it with the
ENTER button on the LCD or just CTRL-C in the terminal where you
launched it.

The rtlizer/beaglebone subdirectory contains an rtlizer.service file
which can be used to have rtlizer started when the Beaglebone boots.
Copy this file to /etc/systemd/system:

  $ sudo cp rtlizer/beaglebone/rtlizer.service /etc/systemd/system/
  $ sudo systemctl --system daemon-reload

Test that the script works:

  $ sudo systemctl start rtlizer.service

Rtlizer should be running. Stop it again using:

  $ sudo systemctl stop rtlizer.service

If all OK, enable the service:

  $ sudo systemctl enable rtlizer.service

Restart the Beaglebone and watch rtlizer start automatically :-)

If you ever wish to disable the auto start you can do that using:

  $ sudo systemctl disable rtlizer.service

