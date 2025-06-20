#!/usr/bin/sh

cd ..
qemu-system-x86_64 -d int -D qemu.log image.iso
