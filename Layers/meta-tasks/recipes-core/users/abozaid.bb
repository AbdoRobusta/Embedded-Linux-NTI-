SUMMARY = "Add a custom user"
DESCRIPTION = "Recipe that creates a new user at image creation time"
LICENSE = "CLOSED"

# Inherit the useradd class
inherit useradd

# The name of the user you want to add
USERADD_PACKAGES = "${PN}"
USERADD_PARAM:${PN} = "-u 1234 -d /home/abozaid -r -s /bin/bash -m abozaid"
FILES:${PN} += "/home/abozaid"

# # Create the home directory and basic setup
do_install:append() {
    install -d ${D}/home/abozaid
    chown abozaid:abozaid ${D}/home/abozaid
}