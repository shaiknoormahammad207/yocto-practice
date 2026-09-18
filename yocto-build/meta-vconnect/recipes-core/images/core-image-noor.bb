DESCRIPTION= "noor custom rootfs"
require recipes-core/images/core-image-minimal.bb
INSTALL_IMAGE:append = " \
                        hello \
                        hello-cpp \
"

