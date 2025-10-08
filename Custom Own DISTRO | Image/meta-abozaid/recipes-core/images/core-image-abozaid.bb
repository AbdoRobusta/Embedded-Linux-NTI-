# =========================================================
#  Abozaid Linux - Custom Image (based on core-image-weston)
# =========================================================
SUMMARY = "A graphical image for Abozaid Linux (based on core-image-weston)"
DESCRIPTION = "Abozaid Linux image with Weston, WiFi, Bluetooth, and SSH support."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835c964f2efb25f9a9e41c4b47d3f75"

inherit core-image

# Base features (same as Weston)
IMAGE_FEATURES += "splash package-management ssh-server-openssh weston"

# Include common functionality
IMAGE_INSTALL += " \
    packagegroup-core-boot \
    packagegroup-core-x11 \
    packagegroup-base-extended \
    wayland weston \
    wifi-static \
    bluez5 \
    bash \
    vim \
"

# Branding
IMAGE_NAME_SUFFIX = "-abozaid"
DISTRO_FEATURES:append = " wifi bluetooth wayland"

# Default user
EXTRA_USERS_PARAMS = "useradd -p '' abozaid; usermod -a -G wheel abozaid;"

# Message of the Day
ROOTFS_POSTPROCESS_COMMAND += "echo 'Welcome to Abozaid Linux with Weston UI!' > ${IMAGE_ROOTFS}/etc/motd;"
