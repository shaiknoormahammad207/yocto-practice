DESCRIPTION = "Hello Linux Kernel Module"
LICENSE = "CLOSED"

SRC_URI = " \
    file://hello.c \
    file://Makefile \
"

S = "${WORKDIR}"

inherit module
