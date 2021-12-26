#include "file_dir.h"
#include "bitmap.h"
#include "disk_emu.h"
#include "Inode_sfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Maximum directory size is 5 blocks (blocks 6-10) supports up to 320 files
struct directory_entry dir_block[80];
int currfile;

int initialize_directory(){
    struct directory_entry ** cur = (struct directory_entry **) malloc(sizeof(struct directory_entry *));
    for(int i = 0; i<80; i++){
        *cur = &dir_block[i];
        (*cur)->available = 1;
    }
    free(cur);
    write_dir();
    //Mark the appropriate blocks as taken
    for(int i = 6; i < 11; i++){
        close_block(i);
    }
    write_bitmap();
    currfile = 0;
    return 0;
}

int write_dir(){
    write_blocks(6, 5, &dir_block);
    return 0;
}

int read_dir(){
    read_blocks(6, 5, &dir_block, "dir");
    return 0;
}

int create_entry(char *toCreate, int i_node_index){
    struct directory_entry ** cur = (struct directory_entry **) malloc(sizeof(struct directory_entry *));
    int *index = (int *) malloc(sizeof(int));
    *index = find_next_free_dir();
    *cur = &dir_block[*index];
    (*cur)->available = 0;
    strcpy((*cur)->filename, toCreate);
    (*cur)->i_node_index = i_node_index;
    free(cur);
    free(index);
    write_dir();
    return 0;
}

int get_inode_from_filename(char *fname){
    struct directory_entry ** cur = (struct directory_entry **) malloc(sizeof(struct directory_entry *));
    int toReturn;
    for(int i = 0; i<80; i++){
        *cur = &dir_block[i];
        if((*cur)->available == 0 && (strcmp(((*cur)->filename), fname) == 0)){
            toReturn = (*cur)->i_node_index;
            free(cur);
            return toReturn;
        }
    }
    return -1;
}

int find_next_free_dir(){
    struct directory_entry ** cur = (struct directory_entry **) malloc(sizeof(struct directory_entry *));
    for(int i = 0; i < 80; i++){
        *cur = &dir_block[i];
        if((*cur)->available){
            free(cur);
            return i;
        }
    }
    free(cur);
    return -1;
}

int get_next_file(char **toWrite){
    struct directory_entry ** cur = (struct directory_entry **) malloc(sizeof(struct directory_entry *));
    while(currfile < 80){
        *cur = &dir_block[currfile];
        if((*cur)->available == 0){
            strcpy((*toWrite), (*cur)->filename);
            free(cur);
            currfile++;
            return 1;
        }
        currfile++;
    }
    currfile = 0;
    free(cur);
    return 0;
}

int remove_from_dir_get_iNode(char * toRemove){
    char *toTest = (char *) malloc(sizeof(char[56]));
    int i = 0;
    while(get_next_file(&toTest) != 0){
        if(strcmp(toTest, toRemove) == 0){
            currfile = 0;
            (&dir_block[i])->available = 1;
            memset((&dir_block[i])->filename, 0, 56);
            return ((&dir_block[i])->i_node_index);
        }
        i++;
    }
    return -1;
}

int get_file_size(char ** path){
    struct directory_entry ** cur = (struct directory_entry **) malloc(sizeof(struct directory_entry *));
    int toReturn;
    struct iNode * node = (struct iNode *) malloc(sizeof(struct iNode));
    for(int i = 0; i<80; i++){
        *cur = &dir_block[i];
        if(!((*cur)->available) && strcmp((*cur)->filename, (*path))){
            int index = (*cur)->i_node_index;
            node = get_iNode(index);
            toReturn = node->size;
            free(node);
            free(cur);
            return toReturn;
        }
    }
    free(cur);
    return -1;
}