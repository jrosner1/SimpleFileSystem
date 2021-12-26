#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "disk_emu.h"
#include "Super_block.h"

int write_to_disk(struct super_block *blockToWrite){
    write_blocks(0, 1, blockToWrite);
    return 0;
}

int initialize_super(struct super_block* toInitialize){
    toInitialize->Block_size = 1024;
    toInitialize->File_system_size = 1024;
    toInitialize->i_node_table_length=5;
    (toInitialize->magic_number) = "0xACBD0005";
    toInitialize->Root_dir = 0;
    return 0;
}
