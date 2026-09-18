DESCRIPTION="hello cmake example"
LICENSE="CLOSED"

SRC_URI="file://hello.c \
        file://CMakeLists.txt"

DEPENDS += "cmake-native"

#inherit cmake

S="${WORKDIR}"
B="${WORKDIR}/build"
inherit cmake
