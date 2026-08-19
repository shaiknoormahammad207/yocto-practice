DESCRIPTION="recipe hello cpp example"
LICENSE="CLOSED"

SRC_URI="file://hello.cpp"

S="${WORKDIR}"

do_compile(){
    cd ${S} 
    ${CXX}  ${LDFLAGS} hello.cpp -o hello_cpp
}

do_install(){
    install -d ${D}/usr/bin
    install -m 777 ${S}/hello_cpp ${D}/usr/bin/hello_cpp
}

