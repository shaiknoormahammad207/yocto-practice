DESCRIPTION="recipe for libmodbus version v3.1.5"

LICENSE="LGPL-2.1"
LIC_FILES_CHKSUM = "file://COPYING.LESSER;md5=4fbd65380cdd255951079008b364516c"

SRC_URI = "git://github.com/stephane/libmodbus.git;protocol=https"
SRCREV = "890e689b9e6a806e26d7072e142cf2b3bda62755"
DEPENDS="automake-native autoconf-native libtool-native"
S = "${WORKDIR}/git"

do_configure() {
    ./autogen.sh
    ./configure --prefix=/usr --host=${HOST_SYS}
}

do_compile() {
    make
}

do_install() {
    cd ${S}
    make DESTDIR=${D} install
}
