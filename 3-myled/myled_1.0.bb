SUMMARY = "C++ OOP LED control app"
LICENSE = "CLOSED"
SRC_URI = "file://myled.cpp"

S = "${WORKDIR}"

do_compile() {
    ${CXX} ${CXXFLAGS} ${LDFLAGS} -o myled myled.cpp
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 myled ${D}${bindir}
}
