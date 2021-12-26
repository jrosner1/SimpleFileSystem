#include "bitmap.h"
#include "disk_emu.h"
#include <stdlib.h>
#include <stdio.h>

int bitMap[1024];

int initialize_bitmap(){
    //1 for the super block
    bitMap[0] = 1;
    //1 for the locations of the bitmap
    bitMap[1023] = 1;
    bitMap[1022] = 1;
    bitMap[1021] = 1;
    bitMap[1020] = 1;
    if(write_bitmap() != 0){
        return -1;
    }
    return 0;
}

int close_block(int block_to_close){
    if(block_to_close < 1024 && block_to_close >=0){
        bitMap[block_to_close] = 1;
        return 0;
    }else{
        return -1;
    }
}

int open_block(int block_to_open){
    if(block_to_open < 1024 && block_to_open >=0){
        bitMap[block_to_open] = 0;
        return 0;
    }else{
        return -1;
    }
}

int read_bitmap(){
    if(read_blocks(1020, 4, &bitMap, "bitmap") != 4){
        return -1;
    }
    return 0;
}

int write_bitmap(){
    if(write_blocks(1020, 4, &bitMap)) != 4){
        return -1;
    }
    return 0;
}

int allocate_block(){
    read_bitmap();
    for(int i = 0; i < 1024; i++){
        if(bitMap[i] == 0){
            close_block(i);
            write_bitmap();
            if(i == 0){
                exit(0);
            }
            return i;
        }
    }
    return -1;
}

