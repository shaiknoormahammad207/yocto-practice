DESCRIPTION="example for static library"
LICENSE = "CLOSED"

SRC_URI = "file://main.c \
           file://mathlib.c \
           file://mathlib.h"

S = "${WORKDIR}"

do_compile() {
    ${CC} ${CFLAGS} -c mathlib.c -o mathlib.o
    ${AR} rcs libmath.a mathlib.o

    ${CC} ${CFLAGS} main.c -L${S} -lmath ${LDFLAGS} -o hello
}


do_install() {
    install -d ${D}${bindir}
    install -m 777 hello ${D}${bindir}/hello_liba
}
