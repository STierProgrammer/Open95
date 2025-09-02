#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX

#include "nob.h"
#include <stddef.h>

#define SRC_FOLDER "src/"
#define BUILD_FOLDER "build/"

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
    NOB_GO_REBUILD_URSELF(argc, argv);
    File_Paths c_sources = {0};
    File_Paths asm_sources = {0};

    if (!walk_directory(NULL, &c_sources, &asm_sources, SRC_FOLDER)) return 1;

    mkdir_if_not_exists(BUILD_FOLDER);

    Cmd cmd = {0};
    
    for (size_t i = 0; i < c_sources.count; i++) {
        const char* build_path = temp_sprintf("%s%s.o", BUILD_FOLDER, c_sources.items[i] + strlen(SRC_FOLDER));
        nob_cmd_append(&cmd, 
            "cc", 
            "-Wall", 
            "-Wextra", 
            "-I", "../limine", 
            "-DLIMINE_API_REVISION=3",
            "-Wall",
            "-std=gnu11",
            "-ffreestanding",
            "-fno-stack-protector",
            "-fno-stack-check",
            "-fno-PIC",
            "-m64",
            "-march=x86-64",
            "-mno-80387",
            "-mno-mmx",
            "-Wextra",
            "-mno-sse",
            "-mno-sse2",
            "-mno-red-zone",
            "-mcmodel=kernel",
            "-o", build_path, c_sources.items[i]
        );
        if (!nob_cmd_run(&cmd)) return 1;
    }

}
