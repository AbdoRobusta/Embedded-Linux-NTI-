SUMMERY="My Graphie Recipe QT Project"
LICENSE="MIT"

inherit core-image

CORE_IMAGE_BASE_INSTAL += "gtk+3-demo"


QB_MEN = "-m 512"
# IMAGE_FSTYPES = "ext3 ext4 bz2"
IMAGE_INSTALL:append = " python3 \
                          connman-client \
                          psplash \
                          rdp-certs"

IMAGE_FEATURES:append = " splash \
                          ssh-server-openssh \
                          package-management \
                          hwcodecs weston"
DEPENDS:append = " qtbase \
                   qtquick3d \
                   qttools \
                   cups \
                   qtdeclarative-native \
                   qtwayland"

RDEPENDS:${PN} = " qtbase \
                   qtquick3d \
                   qttools \
                   cups \
                   qtdeclarative \
                   qtwayland"
                   

DISTRO_FEATURES:append = " wayland "
IMAGE_ROOTFS_EXTRA_SPACE = "5242880"
PACKAGE_CLASSES = " package_rpm"

# Enable RDP backend in Weston
IMAGE_INSTALL:append = " weston weston-init weston-examples"
