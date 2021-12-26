#include "indirectBlock.h"
#include "bitmap.h"
#include "disk_emu.h"
#include "Inode_sfs.h"
int iBlock[256];

int place_block(struct iNode* this_iNode, int index, int toPlace){
    read_blocks((this_iNode->indirectBlockIndex), 1, &iBlock, "indirect place");
    iBlock[index] = toPlace;
    write_blocks((this_iNode->indirectBlockIndex), 1, &iBlock);
    return 0;
}

int get_block_from_index(struct iNode* this_iNode, int index){
    read_blocks((this_iNode->indirectBlockIndex), 1, &iBlock, "indirect get");
    return iBlock[index];
}

int reset_iBlock(){
    for(int i = 0; i < 256; i++){
        iBlock[i] = 0;
    }
    return 0;
}

int create_indirect_block(struct iNode* this_iNode, int toPlace){
    int this_indirect = allocate_block();
    (this_iNode->indirectBlockIndex) = this_indirect;
    reset_iBlock();
    iBlock[0] = toPlace;
    write_blocks(this_indirect, 1, &iBlock);
    return 0;
}