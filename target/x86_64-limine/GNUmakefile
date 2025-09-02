SRCDIR := kernel/src
OBJDIR := build/obj
BINDIR := build/bin
ISODIR := build/iso_root
OUTPUT := open95

ARCH := x86_64
CC := cc
CFLAGS := -g -O2 -pipe
CPPFLAGS := -I $(SRCDIR)/arch/$(ARCH) -I limine/
NASMFLAGS := -F dwarf -g
LDFLAGS := 

# Internal C flags that should not be changed by the user.
override CFLAGS += \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-PIC \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -mcmodel=kernel

# Internal C preprocessor flags that should not be changed by the user.
override CPPFLAGS := \
    -I $(SRCDIR) \
    $(CPPFLAGS) \
    -DLIMINE_API_REVISION=3 \
    -MMD \
    -MP

# Internal nasm flags that should not be changed by the user.
override NASMFLAGS += \
    -Wall \
    -f elf64

# Internal linker flags that should not be changed by the user.
override LDFLAGS += \
    -Wl,-m,elf_x86_64 \
    -Wl,--build-id=none \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -T target/x86_64-limine/linker.ld

SRCFILES := $(shell cd $(SRCDIR) && find -L * -type f | LC_ALL=C sort)
CFILES := $(filter %.c,$(SRCFILES))
NASMFILES := $(filter %.asm,$(SRCFILES))

OBJFILES := $(addprefix build/obj/,$(CFILES:.c=.c.o) $(NASMFILES:.asm=.asm.o))
HEADERDEPS := $(addprefix build/obj/,$(CFILES:.c=.c.d) $(NASMFILES:.asm=.asm.d))

build/bin/$(OUTPUT): target/x86_64-limine/GNUmakefile target/x86_64-limine/linker.ld $(OBJFILES)
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJFILES) -o $@

build/obj/%.c.o: $(SRCDIR)/%.c target/x86_64-limine/GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/obj/%.asm.o: $(SRCDIR)/%.asm target/x86_64-limine/GNUmakefile
	mkdir -p "$$(dirname $@)"
	nasm $(NASMFLAGS) $< -o $@

-include $(HEADERDEPS)

.PHONY: all
all:
	$(MAKE) $(BINDIR)/$(OUTPUT)
	$(MAKE) build
	$(MAKE) run

.PHONY: build
build:
	$(MAKE) -C limine

	mkdir -p $(ISODIR)/boot/limine $(ISODIR)/EFI/BOOT

	cp -v $(BINDIR)/$(OUTPUT) $(ISODIR)/boot/

	cp -v target/x86_64-limine/limine.conf \
		limine/limine-bios.sys \
		limine/limine-bios-cd.bin \
		limine/limine-uefi-cd.bin \
		$(ISODIR)/boot/limine/

	cp -v limine/BOOTX64.EFI limine/BOOTIA32.EFI $(ISODIR)/EFI/BOOT

	# Create the bootable ISO.
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		build/iso_root -o build/image.iso

	# Install Limine stage 1 and 2 for legacy BIOS boot.
	./limine/limine bios-install build/image.iso

.PHONY: run
run:
	qemu-system-x86_64 -serial stdio -d int -D build/qemu.log build/image.iso --no-reboot --no-shutdown

.PHONY: bootstub
bootstub:
	

.PHONY: run_gdb
run_gdb:
	qemu-system-x86_64 -s -S -monitor telnet:127.0.0.1:1235,server,nowait -serial stdio -d int -D build/qemu.log build/image.iso --no-reboot --no-shutdown
