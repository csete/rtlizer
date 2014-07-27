# The graphical image + SDK for building SW on target

require nanosdr-gfx-image.bb

IMAGE_INSTALL += " \
	gdb gdbserver \
	git \
    gtk+-dev \
    librtlsdr-dev \
	minicom \
	nano \
	screen \
	task-sdk-target \
	tar \
	vim vim-vimrc \
"

export IMAGE_BASENAME = "nanosdr-dev-image"
