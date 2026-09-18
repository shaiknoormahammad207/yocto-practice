SUMMARY = "Out-of-tree Character Device Driver for i.MX6ULL"

LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

inherit module

SRC_URI = " \
    file://my_chardev.c \
    file://Makefile \
"

S = "${WORKDIR}"

KERNEL_MODULE_AUTOLOAD += "my_chardev"
