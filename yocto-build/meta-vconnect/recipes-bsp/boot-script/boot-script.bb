SUMMARY = "i.MX6ULL automatic boot script"
DESCRIPTION = "U-Boot boot script for automatic Linux boot on i.MX6ULL"
LICENSE = "CLOSED"

SRC_URI = "file://boot.cmd"

S = "${WORKDIR}"

DEPENDS += "u-boot-tools-native"

inherit deploy

do_compile() {
    mkimage -A arm \
            -T script \
            -C none \
            -n "i.MX6ULL boot script" \
            -d ${WORKDIR}/boot.cmd \
            ${B}/boot.scr
}

do_deploy() {
    install -d ${DEPLOYDIR}
    install -m 0644 ${B}/boot.scr ${DEPLOYDIR}/boot.scr
}

addtask deploy after do_compile
