DESCRIPTION = "Simple Hello Example using Shared Library"
LICENSE = "CLOSED"

SRC_URI = " \
    file://main.c \
    file://mathlib.c \
    file://mathlib.h \
"

S = "${WORKDIR}"

do_compile() {
    # Build shared library
    ${CC} ${CFLAGS} -fPIC -c mathlib.c -o mathlib.o

    ${CC} ${LDFLAGS} -shared \
        -Wl,-soname,libmath.so.1 \
        -o libmath.so.1.0 mathlib.o

    # Create application
    ${CC} ${CFLAGS} main.c \
        -L${S} \
        -l:libmath.so.1.0 \
        ${LDFLAGS} \
        -o hello
}

do_install() {
    install -d ${D}${bindir}
    install -d ${D}${libdir}

    # Install application
    install -m 0755 hello ${D}${bindir}

    # Install versioned shared library
    install -m 0755 libmath.so.1.0 ${D}${libdir}

    # Create library symlinks
    ln -sf libmath.so.1.0 ${D}${libdir}/libmath.so.1
    ln -sf libmath.so.1 ${D}${libdir}/libmath.so
}


