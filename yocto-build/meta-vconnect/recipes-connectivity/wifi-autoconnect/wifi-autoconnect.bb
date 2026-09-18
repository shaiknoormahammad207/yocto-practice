SUMMARY = "Automated Wi-Fi startup service and configuration"
DESCRIPTION = "Brings up wlan0, authenticates via wpa_supplicant, and assigns IP via DHCP"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit systemd

SRC_URI = " \
    file://wifi-autoconnect.service \
    file://wpa_supplicant.conf \
"

S = "${WORKDIR}"

# Enable the systemd service unit automatically in the target rootfs
SYSTEMD_SERVICE:${PN} = "wifi-autoconnect.service"
SYSTEMD_AUTO_ENABLE = "enable"

do_install() {
    # 1. Install systemd service unit
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/wifi-autoconnect.service ${D}${systemd_system_unitdir}/

    # 2. Install pre-configured Wi-Fi credentials
    install -d ${D}${sysconfdir}/wifi
    install -m 0600 ${WORKDIR}/wpa_supplicant.conf ${D}${sysconfdir}/wifi/wpa_supplicant.conf
}

FILES:${PN} += " \
    ${systemd_system_unitdir}/wifi-autoconnect.service \
    ${sysconfdir}/wifi/wpa_supplicant.conf \
"

# Ensure runtime tools exist in the final image
RDEPENDS:${PN} += " \
    wpa-supplicant \
    iw \
"
