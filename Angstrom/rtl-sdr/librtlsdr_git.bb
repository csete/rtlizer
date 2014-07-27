DESCRIPTION = "RTL SDR library and utilities"
HOMEPAGE = "http://sdr.osmocom.org/trac/wiki/rtl-sdr"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=751419260aa954499f7abaabaa882bbe"

DEPENDS = "libusb"

PR = "r0"
 
# SRC_URI = "file:///home/alc/beagle/nanosdr/librtlsdr-20130201.tar.gz"
# SRC_URI[md5sum] = "5a7d189585db864b98a9e5bec5a48abf"
# SRC_URI[sha256sum] = "90966e3ec73e0bedb5ad0d7f27c9992c911285a8994601159a6fa5af5ecea4cb" 

SRC_URI = "git://git.osmocom.org/rtl-sdr.git;branch=master;protocol=git"
SRCREV = "HEAD"
# necessary for git:
S = "${WORKDIR}/git"

inherit autotools pkgconfig

