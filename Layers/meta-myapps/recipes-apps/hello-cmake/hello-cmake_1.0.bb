SUMMARY = "Simple Hello World app built with CMake"
LICENSE = "CLOSED"
SRC_URI = "file://hello.cpp file://CMakeLists.txt"
S = "${WORKDIR}"

inherit cmake

do_install() {
    install -d ${D}${bindir}
    install -m 0755 hello_cmake ${D}${bindir}/hello_cmake
}
