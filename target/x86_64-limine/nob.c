#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX

#include "nob.h"

#define SRC_FOLDER "src"
#define BUILD_FOLDER "build"
#define BIN_FOLDER "bin"

#define CFLAGS \
    "-Wall", \
    "-Wextra", \
    "-std=gnu11", \
    "-ffreestanding", \
    "-fno-stack-protector", \
    "-fno-stack-check", \
    "-fno-PIC", \
    "-m64", \
    "-march=x86-64", \
    "-mno-80387", \
    "-mno-mmx", \
    "-mno-sse", \
    "-mno-sse2", \
    "-mno-red-zone", \
    "-mcmodel=kernel"

#define CPPFLAGS \
    "-I", \
    SRC_FOLDER, \
    "-I", \
    "../limine/", \
    "-I", \
    "../bootstub/src/include", \
    "-I", \
    "src/arch/x86_64", \
    "-DLIMINE_API_REVISION=3", \
    "-MMD", \
    "-MP"

#define NASMFLAGS \
    "-Wall", \
    "-f", \
    "elf64"

#define LDFLAGS \
    "-Wl,-m,elf_x86_64", \
    "-Wl,--build-id=none", \
    "-nostdlib", \
    "-static", \
    "-z", \
    "max-page-size=0x1000", \
    "-T", \
    "../target/x86_64-limine/linker.ld"

/*
    Inspired by DCraft BG (TM) (R) (C)
*/
static bool walk_directory(
    File_Paths* dirs,
    File_Paths* c_sources,
    File_Paths* asm_sources,
    const char* path
) {
    DIR *dir = opendir(path);
    if(!dir) {
        nob_log(ERROR, "Could not open directory %s: %s", path, strerror(errno));
        return false;
    }

    errno = 0;
    struct dirent *ent;
    while(ent = readdir(dir)) {
        if(*ent->d_name == '.') continue;
        
        size_t temp = temp_save();
        const char* p = temp_sprintf("%s/%s", path, ent->d_name); 

        String_View sv = sv_from_cstr(p);
        File_Type type = get_file_type(p);
        if (type == FILE_DIRECTORY) {
            if (dirs) da_append(dirs, p);
            if(!walk_directory(dirs, c_sources, asm_sources, p)) {
                closedir(dir);
                return false;
            }
            continue;
        }

        if(sv_end_with(sv, ".c")) {
            da_append(c_sources, p);
        } else if(sv_end_with(sv, ".asm")) {
            da_append(asm_sources, p);
        } else {
            temp_rewind(temp);
        }
    }
    closedir(dir);
    return true;
}

int main(int argc, char **argv)
{
    // chdir("../../");

    NOB_GO_REBUILD_URSELF(argc, argv);
    File_Paths dirs = {0};
    File_Paths c_sources = {0};
    File_Paths asm_sources = {0};
    File_Paths objs = {0};

    if (!walk_directory(&dirs, &c_sources, &asm_sources, SRC_FOLDER)) return 1;

    mkdir_if_not_exists(BUILD_FOLDER);
    mkdir_if_not_exists(BUILD_FOLDER "/" SRC_FOLDER);
    mkdir_if_not_exists(BUILD_FOLDER "/" BIN_FOLDER);

    Cmd cmd = {0};
    
    for (size_t i = 0; i < dirs.count; i++) {
        const char* p = temp_sprintf("%s/%s", BUILD_FOLDER, dirs.items[i]);
        mkdir_if_not_exists(p);
    }

    /* 
        Compile C files
    */
    for (size_t i = 0; i < c_sources.count; i++) {
        const char* p = temp_sprintf("%s/%s.o", BUILD_FOLDER, c_sources.items[i]);

        nob_cmd_append(&cmd, 
            "cc",
            CFLAGS,
            CPPFLAGS,
            "-c",
            c_sources.items[i],
            "-o",
            p
        );
        nob_da_append(&objs, p);
        if (!nob_cmd_run(&cmd)) return 1;
    }


    nob_cmd_append(&cmd, 
        "cc",
        CFLAGS,
        CPPFLAGS,
        "-c",
        "../bootstub/src/limine_bootstub.c",
        "-o",
        "build/src/limine_bootstub.c.o"
    );
    
    nob_da_append(&objs, "build/src/limine_bootstub.c.o");
    if (!nob_cmd_run(&cmd)) return 1;

    /*
        Assemble ASM files
    */
    for (size_t i = 0; i < asm_sources.count; i++) {
        const char* p = temp_sprintf("%s/%s.o", BUILD_FOLDER, asm_sources.items[i]);

        nob_cmd_append(&cmd, 
            "nasm",
            NASMFLAGS,
            asm_sources.items[i],
            "-o",
            p
        );
        nob_da_append(&objs, p);
        if (!nob_cmd_run(&cmd)) return 1;
    }

    /*
        Linking
    */
    nob_cmd_append(&cmd, 
        "cc",
        CFLAGS,
        LDFLAGS
    );

    nob_da_append_many(&cmd,
        objs.items, 
        objs.count
    );

    nob_cmd_append(&cmd,
        "-o",
        BUILD_FOLDER "/" BIN_FOLDER "/open95",
    );

    if (!nob_cmd_run(&cmd)) return 1;    

    nob_cmd_append(&cmd,
        "make",
        "-C",
        "../limine",
    );

    if (!nob_cmd_run(&cmd)) return 1;    
    
    
    if (!mkdir_if_not_exists("build/iso_root") 
    || !mkdir_if_not_exists("build/iso_root/boot") 
    || !mkdir_if_not_exists("build/iso_root/boot/limine") 
    || !mkdir_if_not_exists("build/iso_root/EFI")
    || !mkdir_if_not_exists("build/iso_root/EFI/BOOT")
    ) return 1;
    
    if (!nob_copy_file("build/bin/open95", "build/iso_root/boot/open95")) return 1;

    nob_cmd_append(&cmd,
        "cp",
        "-v",
        "../target/x86_64-limine/limine.conf",
        "../limine/limine-bios.sys",
        "../limine/limine-bios-cd.bin",
        "../limine/limine-uefi-cd.bin",
        "build/iso_root/boot/limine/"
    );

    if (!nob_cmd_run(&cmd)) return 1;    

    nob_cmd_append(&cmd,
        "cp",
        "-v",
        "../limine/BOOTX64.EFI",
        "../limine/BOOTIA32.EFI",
        "build/iso_root/EFI/BOOT"
    );

    if (!nob_cmd_run(&cmd)) return 1;    

    nob_cmd_append(&cmd,
        "xorriso",
        "-as",
        "mkisofs", 
        "-R",
        "-r",
        "-J",
        "-b",
        "boot/limine/limine-bios-cd.bin",
		"-no-emul-boot",
        "-boot-load-size",
        "4",
        "-boot-info-table",
        "-hfsplus",
		"-apm-block-size",
        "2048",
        "--efi-boot",
        "boot/limine/limine-uefi-cd.bin",
		"-efi-boot-part",
        "--efi-boot-image",
        "--protective-msdos-label",
		"build/iso_root",
        "-o",
        "build/image.iso"
    );

    if (!nob_cmd_run(&cmd)) return 1;    

    nob_cmd_append(&cmd,
        "../limine/limine",
        "bios-install", 
        "build/image.iso"
    );
	
    if (!nob_cmd_run(&cmd)) return 1;

    nob_cmd_append(&cmd,
        "qemu-system-x86_64",
        "-serial",
        "stdio", 
        "-d", 
        "int",
        "-D", 
        "build/qemu.log",
        "build/image.iso", 
        "--no-reboot", 
        "--no-shutdown"
    );

    if (!nob_cmd_run(&cmd)) return 1;
}