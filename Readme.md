menuentry "INSTALACJA FEDORY" {
    set isofile="/home/dartz/fedora1"
    loopback loop (hd0,1)$isofile
    linux (loop)/images/pxeboot/vmlinuz inst.stage2=hd:LABEL=Fedora-W-Live-41-1-4 iso-scan/filename=$isofile quiet
    initrd (loop)/images/pxeboot/initrd.img
}
