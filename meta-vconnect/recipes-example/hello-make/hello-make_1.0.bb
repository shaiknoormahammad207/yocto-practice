ESCRIPTION = "hello make example"
LICENSE = "CLOSED"

SRC_URI = " \
    file://hello.c \
    file://Makefile \
"

S = "${WORKDIR}"

do_compile() {
    make
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/hello ${D}${bindir}/hello_make
}
