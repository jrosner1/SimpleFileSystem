#include "Inode_sfs.h"
#include "bitmap.h"
#include "disk_emu.h"
#include "indirectBlock.h"
#include <stdlib.h>

struct iNode iNode_table[80];

int initialize_iNode_table(){
    struct iNode ** cur = (struct iNode **) malloc(sizeof(struct iNode *));
    for(int i = 0; i<80; i++){
        *cur = &iNode_table[i];
        (*cur)->uid = i;
        (*cur)->available = 1;
        for(int j = 0; j < 12; j++){
            (*cur)->pointers[j] = -1;
        }
    }
    free(cur);
    write_iNode_table();
    for(int i = 1; i < 6; i++){
        close_block(i);
    }
    write_bitmap();
    return 0;
}

int allocate_iNode(int index){
    read_iNode_table();
    struct iNode ** cur = (struct iNode **) malloc(sizeof(struct iNode *));
    *cur = &iNode_table[index];
    (*cur)->available = 0;
    (*cur)->size = 0;
    free(cur);
    write_iNode_table();
    return 0;
}

int file_has_enough_size(int iNode_index, int length){
    read_iNode_table();
    struct iNode ** cur = (struct iNode **) malloc(sizeof(struct iNode *));
    *cur = &iNode_table[iNode_index];
    int num_blocks;
    if((*cur)->size % 1024 == 0){
        num_blocks = (*cur)->size/1024;
    }else{
        num_blocks = ((*cur)->size/1024) + 1;
    }
    if((num_blocks * 1024) - (*cur)->size >= length){
        //No need to allocate another block
        free(cur);
        return 1;
    }else{
        free(cur);
        return 0;
    }
}

int write_iNode_table(){
    write_blocks(1, 5, &iNode_table);
    return 0;
}

int read_iNode_table(){
    read_blocks(1, 5, &iNode_table, "iNode");
    return 0;
}

struct iNode *get_iNode(int index){
    return &iNode_table[index];
}

int find_next_free_inode(){
    read_iNode_table();
    struct iNode ** cur = (struct iNode **) malloc(sizeof(struct iNode *));
    for(int i = 0; i<80; i++){
        *cur = &iNode_table[i];
        if((*cur)->available == 1){
            free(cur);
            return i;
        }
    }
    free(cur);
    return -1;
}

int remove_file(int index){
    int numBlocks = ((&iNode_table[index])->size) / 1024;
    if(((&iNode_table[index])->size) % 1024 != 0) numBlocks++;
    int currBlock;
    for(int i = 0; i < numBlocks; i++){
        if(i<12){
            currBlock = ((&iNode_table[index])->pointers)[i];
            open_block(currBlock);
            ((&iNode_table[index])->pointers)[i] = -1;
         }else{
            currBlock = get_block_from_index((&iNode_table[index]), (i-12));
            open_block(currBlock);
        }
    }
    open_block((&iNode_table[index])->indirectBlockIndex);
    (&iNode_table[index])->available = 1;
    (&iNode_table[index])->size = 0;
    write_bitmap();
    return 0;
}
