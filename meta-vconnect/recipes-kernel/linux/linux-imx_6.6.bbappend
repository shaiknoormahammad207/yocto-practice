FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " \
    file://0001-Added-v6.6-dts-for-mys-6ull.patch \
"
KERNEL_DEVICETREE:append = " \
    nxp/imx/mys-6ull-14x14-gpmi-weim.dtb \
"
