#!/usr/bin/sh

qemu-system-x86_64 -serial stdio -d int -D qemu.log build/image.iso

#-s -S -monitor telnet:127.0.0.1:1235,server,nowait
