inherit systemd cargo-dyn

DEPENDS += "dbus openssl"

SRC_URI += " \
	file://os-config.json \
	file://os-config.service \
	"

SYSTEMD_SERVICE_${PN} = "os-config.service"

do_install_append() {
	install -d ${D}${sysconfdir}
	install -m 0644 ${WORKDIR}/os-config.json ${D}${sysconfdir}

	install -d ${D}${systemd_unitdir}/system
	install -c -m 0644 ${WORKDIR}/os-config.service ${D}${systemd_unitdir}/system
}
