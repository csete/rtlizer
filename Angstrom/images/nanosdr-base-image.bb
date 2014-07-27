# Basic Linux image for nanosdr devices
# Contains all the necessary runtime to have a simple SDR running

require recipes-misc/images/ti-hw-bringup-image.bb

ROOTFSTYPE_beaglebone = "ext4"

IMAGE_INSTALL += " \
    canutils \
	connman-tests \
	cronie-systemd \
	iproute2 \
	led-config \
    librtlsdr \
    ntpdate \
	procps \
	rsync \
	systemd-analyze \
"

export IMAGE_BASENAME = "nanosdr-base-image"

