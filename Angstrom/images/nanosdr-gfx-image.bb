# Small image with X but without any display manager or desktop environment
# The SDR GUI is launched via xinit.

require nanosdr-base-image.bb

XSERVER ?= "xserver-xorg \
            xf86-video-fbdev \
            xf86-input-evdev \
           "

IMAGE_INSTALL += " \
    xserver-nodm-init-systemd \
    xserver-common \
    ttf-dejavu-sans ttf-dejavu-sans-mono ttf-dejavu-common \
    ${XSERVER} \
"

export IMAGE_BASENAME = "nanosdr-gfx-image"
