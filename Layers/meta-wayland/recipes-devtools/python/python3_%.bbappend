do_install:append:class-target() {
	ln -sf ${bindir}/python3 ${D}${bindir}/python
}

PACKAGES:prepend:class-target = "${PN}-as-python "
FILES:${PN}-as-python = "${bindir}/python"
RRECOMMENDS:${PN}-core:append:class-target = " ${PN}-as-python"

