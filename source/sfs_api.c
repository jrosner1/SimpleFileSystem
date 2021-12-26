#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk_emu.h"
#include "Super_block.h"
#include "bitmap.h"
#include "sfs_api.h"
#include "file_dir.h"
#include "Inode_sfs.h"
#include "fdescriptor.h"
#include "dataBlock.h"

//Note the number of files in the file system is limited to 80
void mksfs(int flag){
    /*TODO HERE
        1. Allocate memory to the disk 1024 * 1024 bytes (1024 blocks of 1024 bytes)
        2. Initialize and write Free BitMap (ByteMap)
        3. Initialize and write Super Block
        4. Create directory
        5. Create I-node Table (fixed size for now)
        
    */
   //Step 1
    if(flag){
        if(init_fresh_disk(DISKNAME, BLOCKSIZE, NUMBLOCKS) != 0){
            printf("Error allocating memory for the disk\n");
            exit(1);
        }
    //Step 2
    initialize_bitmap();
    //Step 3
    struct super_block *thisSuper = (struct super_block *) malloc(sizeof(struct super_block));
    initialize_super(thisSuper);
    write_to_disk(thisSuper);
    free(thisSuper);
    //Step 4
    initialize_directory();
    //Step 5 nothing has to be done with this other than to set the bitmaps for the 1st - 5th blocks as taken
    initialize_iNode_table();
    //printf("The size of a dir entry is %d", sizeof(struct directory_entry));
    initialize_descriptor();
    }else if(init_disk(DISKNAME, BLOCKSIZE, NUMBLOCKS) != 0){
            printf("Error allocating memory for the disk\n");
            exit(1);
    }
}

int sfs_getnextfilename(char* fileName){
    return (get_next_file(&fileName));
}

int sfs_getfilesize(const char* fileName){
    int toOpen;
    if((toOpen = get_inode_from_filename(fileName)) != -1){
        //file is open
        struct iNode * thisINode = get_iNode(toOpen);
        return thisINode->size;
    }
    else return -1;
}

int sfs_fopen(char* fileName){
    int toOpen;
    if((toOpen = get_inode_from_filename(fileName)) == -1){
        //The file needs to be created
        toOpen = find_next_free_inode();
        allocate_iNode(toOpen);
        create_entry(fileName, toOpen);
        return add_descriptor_entry(toOpen);
    }else{
        return add_descriptor_entry(toOpen);
    }
}

int sfs_fclose(int fileID){
    return close_descriptor_entry(fileID);
}

int sfs_fwrite(int fileID, const char* buffer, int length){
    struct fd_entry** this_entry = (struct fd_entry**) malloc(sizeof(struct fd_entry*));
    struct iNode **this_node = (struct iNode **) malloc(sizeof(struct iNode*));
    if(get_iNode_index(fileID) != -1){
        //File is open
        *this_entry = get_fd_entry(fileID);
        *this_node = get_iNode((*this_entry)->i_node_index);
        write_file(&buffer, length, *this_entry, *this_node);
    }else{
        free(this_entry);
        free(this_node);
        return -1;
    }
    free(this_entry);
    free(this_node);
    return length;
}

int sfs_fread(int fileID, char* buffer, int length){
    struct fd_entry** this_entry = (struct fd_entry**) malloc(sizeof(struct fd_entry*));
    struct iNode **this_node = (struct iNode **) malloc(sizeof(struct iNode*));
    if(get_iNode_index(fileID) != -1){
        //File is open
        *this_entry = get_fd_entry(fileID);
        *this_node = get_iNode((*this_entry)->i_node_index);
        length = read_file(buffer, length, *this_entry, *this_node);
    }else{
        free(this_entry);
        free(this_node);
        return -1;
    }
    free(this_entry);
    free(this_node);
    return length;
}

//NOTE: mode = 1 means adjust read pointer, mode = 0 means adjust write pointer, and mode = 2 means both pointers are adjusted
int sfs_fseek(int fileID, int loc, int mode){
    if(get_iNode_index(fileID) != -1){
        if(loc < 0) return -1;
        struct fd_entry** this_entry = (struct fd_entry**) malloc(sizeof(struct fd_entry*));
        struct iNode** this_node = (struct iNode **) malloc(sizeof(struct iNode*));
        *this_entry = get_fd_entry(fileID);
        *this_node = get_iNode((*this_entry)->i_node_index);
        if(loc > (*this_node)->size){
            free(this_node);
            free(this_entry);
            return -1;
        }
        if(mode == 1){
            //Adjust read pointer
            (*this_entry)->readPtr = loc;
        }else if(mode == 0){
            //Adjust write pointer
            (*this_entry)->writePtr = loc;
            
        }else if(mode == 2){
            //Adjust both pointers
            (*this_entry)->writePtr = loc;
            (*this_entry)->readPtr = loc;
        }else{
            printf("Error wrong usage in fSeek. Mode must be in [0,2]\n");
            return -1;
        }
        free(this_node);
        free(this_entry);
        return 0;
    }else{
        return -1;
    }
}

int sfs_remove(char *file){
    int iNode_index = remove_from_dir_get_iNode(file);
    if(iNode_index == -1){
        return -1;
    }else{
        remove_file(iNode_index);
    }
    return 0;
}