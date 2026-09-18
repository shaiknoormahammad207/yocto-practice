FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " \
    file://0001-Added-v6.6-dts-for-mys-6ull.patch \
    file://wifi.cfg \
    file://0001-Changed-UART3_TX-as-GPIO-in-dts-file.patch \
"
#KERNEL_DEVICETREE:append = " \
#    nxp/imx/mys-6ull-14x14-gpmi-weim.dtb \
#"
KERNEL_DEVICETREE = "nxp/imx/mys-6ull-14x14-gpmi-weim.dtb"
