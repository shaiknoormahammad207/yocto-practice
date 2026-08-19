DESCRIPTION = "recipe for libmodbus version v3.1.6"
LICENSE = "LGPL-2.1"

LIC_FILES_CHKSUM = "file://COPYING.LESSER;md5=4fbd65380cdd255951079008b364516c"

SRC_URI = "git://github.com/stephane/libmodbus.git;protocol=https"
SRCREV = "f9fe3b0a5343f7fbb3f5c74196bd0fce88df39d5"

DEPENDS="automake-native autoconf-native libtool-native"

S="${WORKDIR}/git"

do_configure(){
    ./autogen.sh
    ./configure --prefix=/usr --host=${HOST_SYS}

}

do_compile(){
    make
}

do_install(){
    make DESTDIR=${D} install

}

