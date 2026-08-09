DESCRIPTION="hello cmake example"
LICENSE="CLOSED"

SRC_URI="file://hello.c \
        file://CMakeLists.txt"

DEPENDS += "cmake-native"

inherit cmake

S="${WORKDIR}"
B="${WORKDIR}/build"
do_configure(){
    mkdir -p ${B}
    cd ${B}
    cmake ..
}

do_compile(){
    cd ${B}
    make
}

do_install(){
    install -d ${D}/usr/bin
    install -m 777 ${S}/build/hello ${D}/usr/bin
}

