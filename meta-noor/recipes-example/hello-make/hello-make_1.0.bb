DESCRIPTION="hello make example"
LICENSE="CLOSED"

SRC_URI="file://hello.c \
        file://Makefile"

S="${WORKDIR}"

do_compile(){
    cd ${S}
    make
}

do_install(){
    install -d ${D}/usr/bin
    install -m 777 ${S}/hello ${D}/usr/bin
}

