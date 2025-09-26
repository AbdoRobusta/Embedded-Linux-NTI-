DESCRIPTION = "GPIO Library"
LICENSE = "CLOSED"
SRC_URI = "file://gpio.cpp \
           file://gpio.hpp"

S = "${WORKDIR}"

do_compile() {
    ${CXX} ${CXXFLAGS} -c gpio.cpp -o gpio.o
    ${AR} rcs libgpio.a gpio.o
}

do_install() {
    install -d ${D}${includedir}
    install -d ${D}${libdir}

    install -m 0644 gpio.hpp ${D}${includedir}/
    install -m 0644 libgpio.a ${D}${libdir}/
}
