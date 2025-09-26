DESCRIPTION = "Main app using GPIO"
LICENSE = "CLOSED"
SRC_URI = "file://main.cpp"

DEPENDS += "gpio"

S = "${WORKDIR}"

do_compile() {
    ${CXX} ${CXXFLAGS} ${LDFLAGS} \
        -I${STAGING_INCDIR} \
        -L${STAGING_LIBDIR} \
        main.cpp -lgpio \
        -o gpio_app
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 gpio_app ${D}${bindir}/
}
