SUMMARY = "Simple Hello World app built using g++ manually"
LICENSE = "CLOSED"
SRC_URI = "file://hello.cpp"
S = "${WORKDIR}"

do_compile() {
    ${CXX} hello.cpp -o hello_gpp
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 hello_gpp ${D}${bindir}/hello_gpp
}
