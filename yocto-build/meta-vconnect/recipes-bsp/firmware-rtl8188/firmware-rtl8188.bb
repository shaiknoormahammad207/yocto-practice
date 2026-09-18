SUMMARY = "Firmware for Realtek RTL8188FU / RTL8188FTV Wi-Fi module"
LICENSE = "CLOSED"

SRC_URI = "file://rtl8188fufw.bin"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${nonarch_base_libdir}/firmware/rtlwifi/
    install -m 0644 ${S}/rtl8188fufw.bin ${D}${nonarch_base_libdir}/firmware/rtlwifi/
}

FILES:${PN} += "${nonarch_base_libdir}/firmware/rtlwifi/rtl8188fufw.bin"
