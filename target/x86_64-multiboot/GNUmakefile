SRCDIR := src
OBJDIR := build/obj

SRCFILES := $(shell cd src && find -L * -type f | LC_ALL=C sort)
CFILES := $(filter %.c,$(SRCFILES))
NASMFILES := $(filter %.asm,$(SRCFILES))

kernel_obj := $(kernel_cpp_obj)
x86_64_obj := $(x86_64_asm_obj) $(x86_64_cpp_obj)

$(kernel_cpp_obj): build/kernel/%.o : src/kernel/%.cpp
	mkdir -p $(dir $@) && \
	x86_64-elf-g++ -c -g -I src/intf -I src/x86_64/boot/intf -ffreestanding -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mgeneral-regs-only -Wall $(patsubst build/kernel/%.o, src/kernel/%.cpp, $@) -o $@

$(x86_64_asm_obj): build/x86_64/%.o : src/x86_64/%.asm
	mkdir -p $(dir $@) && \
	nasm -g -F dwarf -f elf64 $(patsubst build/x86_64/%.o, src/x86_64/%.asm, $@) -o $@

$(x86_64_cpp_obj): build/x86_64/%.o : src/x86_64/%.cpp
	mkdir -p $(dir $@) && \
	x86_64-elf-g++ -c -g -I src/intf -I src/x86_64/boot/intf -ffreestanding -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mgeneral-regs-only -Wall $(patsubst build/x86_64/%.o, src/x86_64/%.cpp, $@) -o $@

.PHONY: build-x86_64
build-x86_64: $(kernel_obj) $(x86_64_obj)
	mkdir -p dist/x86_64 && \
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld $(kernel_obj) $(x86_64_obj) && \
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin &&\
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso

