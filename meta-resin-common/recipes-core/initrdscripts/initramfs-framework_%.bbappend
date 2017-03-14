FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " \
    file://machineid \
    file://resindataexpander \
    file://ro-rootfs \
    file://rootfs \
    file://finish \
    "

do_install_append() {
    install -m 0755 ${WORKDIR}/rootfs ${D}/init.d/90-rootfs
    install -m 0755 ${WORKDIR}/finish ${D}/init.d/99-finish

    install -m 0755 ${WORKDIR}/machineid ${D}/init.d/91-machineid
    install -m 0755 ${WORKDIR}/resindataexpander ${D}/init.d/88-resindataexpander
    install -m 0755 ${WORKDIR}/ro-rootfs ${D}/init.d/89-ro-rootfs
}

PACKAGES_append = " \
    initramfs-module-machineid \
    initramfs-module-resindataexpander \
    initramfs-module-ro-rootfs \
    "

RRECOMMENDS_${PN}-base += "initramfs-module-rootfs"

SUMMARY_initramfs-module-machineid = "Bind mount machine-id to rootfs"
RDEPENDS_initramfs-module-machineid = "${PN}-base initramfs-module-udev"
FILES_initramfs-module-machineid = "/init.d/91-machineid"

SUMMARY_initramfs-module-resindataexpander = "Expand the data partition to the end of device"
RDEPENDS_initramfs-module-resindataexpander = "${PN}-base initramfs-module-udev busybox parted bc util-linux-lsblk"
FILES_initramfs-module-resindataexpander = "/init.d/88-resindataexpander"

SUMMARY_initramfs-module-ro-rootfs = "Mount our rootfs"
RDEPENDS_initramfs-module-ro-rootfs = "${PN}-base"
FILES_initramfs-module-ro-rootfs = "/init.d/89-ro-rootfs"

SUMMARY_initramfs-module-rootfs = "initramfs support for locating and mounting the root partition"
RDEPENDS_initramfs-module-rootfs = "${PN}-base"
FILES_initramfs-module-rootfs = "/init.d/90-rootfs"
