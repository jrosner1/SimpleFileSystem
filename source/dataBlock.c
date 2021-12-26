#include "dataBlock.h"
#include "Inode_sfs.h"
#include "fdescriptor.h"
#include "disk_emu.h"
#include "bitmap.h"
#include "indirectBlock.h"
#include <stdio.h>
char currBlock[1024];

int write_file(const char** buffer, int length, struct fd_entry* this_entry, struct iNode* this_iNode){
    int writeLocation = this_entry->writePtr;
    int numBlocks = (this_iNode->size / 1024);
    if(this_iNode->size % 1024 != 0){
        numBlocks++;
    }
    int blockIndex;
    int blockToGet;
    int toWriteInBlock;
    int written = 0;

    while(length > 0){
        //Get block number of location to write
        blockToGet = (writeLocation / 1024) + 1;
        if(blockToGet > numBlocks){
            numBlocks++;
            //Need to allocate a new block in file
            blockIndex = allocate_block();
            if(blockToGet == 13){
                //Create Indirect block and put new block in it
                create_indirect_block(this_iNode, blockIndex);
            }else if(blockToGet > 13){
                //put the new block in the existing indirect block
                place_block(this_iNode, (blockToGet-13), blockIndex);
            }else{
                (this_iNode->pointers)[blockToGet-1] = blockIndex;
            }
        }else if(blockToGet > 12){
            blockIndex = get_block_from_index(this_iNode, (blockToGet-12));
        }else if(blockToGet > 268){
            //File too large
            return -1;
        }else{
            blockIndex = (this_iNode->pointers)[blockToGet-1];
        }
        //Now read the block into memory and write to it
        read_blocks(blockIndex, 1, &currBlock, "write");
        toWriteInBlock = writeLocation - (1024 * (blockToGet-1));
        while(length > 0 && toWriteInBlock < 1024){
            char currentToWrite = (*buffer)[written];
            currBlock[toWriteInBlock] = (*buffer)[written];
            toWriteInBlock++;
            written++;
            writeLocation++;
            length--;
        }
        write_blocks(blockIndex, 1, &currBlock);
    }
    //Update inode and file pointer data
    if(writeLocation > this_iNode->size){
        this_iNode->size = writeLocation;
    }
    this_entry->writePtr = writeLocation;
    this_entry->readPtr = writeLocation;
    return 0;
}

int read_file(char* buffer, int length, struct fd_entry* this_entry, struct iNode* this_iNode){
    int readLocation = this_entry->readPtr;
    int blockToGet;
    int blockNum;
    int toReadInBlock;
    int read = 0;
    //Adjust size of file if the writing will make the file larger
    if(this_entry->readPtr + length > this_iNode->size){
        length = this_iNode->size - this_entry->readPtr;
    }
    while(length > 0){
        blockNum = (readLocation / 1024) + 1;
        //If the block number is larger than 12, get data from an indirect block
        if(blockNum > 12){
            blockToGet = get_block_from_index(this_iNode, (blockNum-13));
        }else{
            blockToGet = (this_iNode->pointers)[blockNum-1];
        }
        //read the block into memory and read it to the buffer
        read_blocks(blockToGet, 1, &currBlock, "read");
        toReadInBlock = readLocation - (1024*(blockNum-1));
        while(length > 0 && toReadInBlock < 1024){
            char toRead = currBlock[toReadInBlock];
            buffer[read] = currBlock[toReadInBlock];
            toReadInBlock++;
            readLocation++;
            read++;
            length--;
        }
        //If we get to the end of the file, break out of the while loop
        if(readLocation == (this_iNode->size)){
            write_blocks(blockToGet,1, &currBlock);
            break;
        }
        write_blocks(blockToGet,1, &currBlock);
    }
    //Update read and write pointers
    this_entry->writePtr = readLocation;
    this_entry->readPtr = readLocation;

    return read;
}