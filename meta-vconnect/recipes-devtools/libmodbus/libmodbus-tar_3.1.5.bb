DESCRIPTION="recipe for libmodbus v3.1.5 using tar file"

LICENSE = "LGPLv2.1"
LIC_FILES_CHKSUM = "file://COPYING.LESSER;md5=4fbd65380cdd255951079008b364516c"

SRC_URI = "https://github.com/stephane/libmodbus/releases/download/v3.1.5/libmodbus-3.1.5.tar.gz"
SRC_URI[sha256sum] = "f7a9538f23a8786b1ee62a4b75879b5c0e194e728350de1b741ce7d595970f06"
DEPENDS += "automake-native autoconf-native libtool-native"

S = "${WORKDIR}/libmodbus-3.1.5" 
# libmodbus-3.1.5 name should match the name after extracting libmodbus-3.1.5.tar.gz
#inherit autotools pkgconfig
do_configure() {
    cd ${S}
    ./configure --prefix=/usr --host=${HOST_SYS}
}

do_compile() {
    make
}

do_install() {
    make DESTDIR=${D} install
}
