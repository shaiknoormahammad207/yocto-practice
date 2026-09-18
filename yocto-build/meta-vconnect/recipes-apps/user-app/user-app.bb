SUMMARY = "GPIO test application"
DESCRIPTION = "Userspace application to control my_gpio driver"
LICENSE = "CLOSED"

SRC_URI = " \
    file://user_app.c \
    file://Makefile \
"

S = "${WORKDIR}"

do_compile() {
    oe_runmake
}

do_install() {
    install -d ${D}${bindir}

    install -m 0755 ${S}/user-app \
        ${D}${bindir}/user-app
}
