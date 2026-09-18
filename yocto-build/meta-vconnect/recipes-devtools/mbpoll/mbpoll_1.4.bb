DESCRIPTION = "recipe for mbpoll version v1.5"
LICENSE = "GPL3"
LIC_FILES_CHKSUM = "file://COPYING;md5=1ebbd3e34237af26da5dc08a4e440464"

SRC_URI = "git://github.com/epsilonrt/mbpoll.git;protocol=https;branch=master"
SRCREV = "ed00121bfa04afdef2d037aac4f574ad7b410601"
S="${WORKDIR}/git"

inherit cmake
