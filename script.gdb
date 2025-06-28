file build/bin/open95
target remote :1234
set disassembly-flavor intel
break initGDT
continue
layout asm