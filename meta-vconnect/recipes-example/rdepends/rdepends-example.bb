DESCRIPTION = "recipe for RDEPENDS and copy bash script in /usr/bin"
LICENSE = "CLOSED"

SRC_URI = "file://helloWorld.sh"
S = "${WORKDIR}"

RDEPENDS:${PN} = "bash"

do_install(){
    install -d ${D}${bindir}
    install -m 777 ${S}/helloWorld.sh ${D}${bindir}


}