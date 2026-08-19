DESCRIPTION = "recipe for mbpoll version v1.5.4"
LICENSE = "GPL-3.0-or-later"
LIC_FILES_CHKSUM = "file://COPYING;md5=1ebbd3e34237af26da5dc08a4e440464"

SRC_URI = "git://github.com/epsilonrt/mbpoll.git;protocol=https;branch=master"
SRCREV = "d541211d0e2451888de11c82fb1604c3d6285b1a"
S="${WORKDIR}/git"
DEPENDS += "libmodbus"
inherit pkgconfig
