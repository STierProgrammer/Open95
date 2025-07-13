#!/usr/bin/sh

set -e

make
make -C limine

mkdir -p build/iso_root/boot/limine build/iso_root/EFI/BOOT

cp -v build/bin/open95 build/iso_root/boot/

cp -v limine.conf \
      limine/limine-bios.sys \
      limine/limine-bios-cd.bin \
      limine/limine-uefi-cd.bin \
      build/iso_root/boot/limine/

cp -v limine/BOOTX64.EFI limine/BOOTIA32.EFI build/iso_root/EFI/BOOT

# Create the bootable ISO.
xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        build/iso_root -o build/image.iso

# Install Limine stage 1 and 2 for legacy BIOS boot.
./limine/limine bios-install build/image.iso
