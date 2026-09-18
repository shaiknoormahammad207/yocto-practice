DESCRIPTION = "recipe for libiec61850 library"
LICENSE = "GPLv3"

LIC_FILES_CHKSUM = "file://COPYING;md5=d32239bcb673463ab874e80d47fae504"
SRC_URI = "git://github.com/mz-automation/libiec61850.git;protocol=https;branch=${BRANCH}"
#SRCREV = "a13961110b8238d2d8ea577c1fb7592ba3017ad8"
S = "${WORKDIR}/git"
#DEPENDS += "cmake-native"

inherit cmake

###################################
# build using cmake 

#do_configure(){
#    mkdir -p ${S}/build
#    cd ${S}/build
#    cmake .. -DCMAKE_INSTALL_PREFIX=/usr
#}

#do_compile(){
#    make
#
#}
#
#do_install(){ #need to know how to copy the lib in /usr
#    cd ${S}/build
#    make DESTDIR=${D} install
#}
###################################

###################################
# build using make
#do_compile(){
#    cd ${S}
#    make
#}
##################################
