DESCRIPTION="example recipe for auto configure"
LICENSE="CLOSED"
SRC_URI="file://hello.c \
        file://configure.ac \
        file://Makefile.am"

S="${WORKDIR}"

#inherit autotools
DEPENDS = "autoconf-native automake-native libtool-native"

do_configure() {
    autoreconf -fiv
    ./configure --host=${HOST_SYS} --prefix=/usr
}

do_compile() {
    oe_runmake
}

do_install() {
    install -d ${D}${bindir}
    install -m 777 hello ${D}${bindir}
}
