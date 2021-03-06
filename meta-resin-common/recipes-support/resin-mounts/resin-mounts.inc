LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${RESIN_COREBASE}/COPYING.Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

SRC_URI += "file://bind-path.service.in"

S = "${WORKDIR}"

inherit systemd allarch

SYSTEMD_SERVICE_${PN} += "${@bindmounts_systemd_services(d)}"

FILES_${PN} += " \
	${sysconfdir} \
	${systemd_unitdir} \
	"

BINDMOUNTS += " \
	/etc/dropbear \
	/etc/hostname \
	/etc/NetworkManager/system-connections \
	/home/root/.rnd \
	/var/lib/NetworkManager \
	"

PACKAGES = "${PN}"

RDEPENDS_${PN} += "util-linux bindmount"

def bindmounts_systemd_services(d):
    services = []
    for bindmount in d.getVar("BINDMOUNTS", True).split():
        services.append("bind-%s.service" % bindmount[1:].replace("/", "-"))
    return " ".join(services)

do_compile () {
	for bindmount in ${BINDMOUNTS}; do
		servicefile="bind-${bindmount#/}"
		servicefile="$(echo "$servicefile" | tr / -).service"
		sed -e "s#@target@#$bindmount#g" bind-path.service.in > $servicefile

		# Service specific changes
		if [ "$bindmount" = "/var/lib/systemd" ]; then
			# Systemd services need to write to /var/lib/systemd so make sure
			# that is mounted.
			sed -i -e "/^Before=/s/\$/ systemd-random-seed.service systemd-rfkill.service systemd-timesyncd/" \
			-e "/^WantedBy=/s/\$/ systemd-random-seed.service systemd-rfkill.service systemd-timesyncd/" \
			$servicefile
		elif [ "$bindmount" = "/var/log/journal" ]; then
			# This bind mount is only invoked manually in persistent logs
			# service. No need to run it with default target.
			sed -i -e "/^WantedBy=/ d" $servicefile
		fi
	done
}
do_compile[dirs] = "${WORKDIR}"

do_install () {
	install -d ${D}${sysconfdir}
	touch ${D}/${sysconfdir}/hostname
}
