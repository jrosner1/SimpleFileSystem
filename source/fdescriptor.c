#include <stdlib.h>
#include <stdio.h>
#include "fdescriptor.h"

struct fd_entry open_file[80];

//Create file descriptor table
int initialize_descriptor(){
    struct fd_entry ** cur = (struct fd_entry **) malloc(sizeof(struct fd_entry *));
    for(int i = 0; i < 80; i++){
        *cur = &open_file[i];
        (*cur)->isAvailable = 1;
    }
    free(cur);
    return 0;
}

//Close an entry in the file descriptor table
int close_descriptor_entry(int descriptor_index){
    struct fd_entry ** cur = (struct fd_entry **) malloc(sizeof(struct fd_entry *));
    *cur = &open_file[descriptor_index];
    if((*cur)->isAvailable){
        free(cur);
        return -1;
    }
    (*cur)->isAvailable = 1;
    free(cur);
    return 0;
}

//Get a fileDescriptor if the file is open. If not, return -1
int get_iNode_index(int FD){
    struct fd_entry ** cur = (struct fd_entry **) malloc(sizeof(struct fd_entry *));
    *cur = &open_file[FD];
    int toReturn = -1;
    if((*cur)->isAvailable){
        free(cur);
        return toReturn;
    }else{
        toReturn = (*cur)->i_node_index;
        free(cur);
        return toReturn;
    }
}

struct fd_entry* get_fd_entry(int FD){
    if(get_iNode_index(FD) != -1){
        return &open_file[FD];
    }else{
        exit(1);
    }
}

int add_descriptor_entry(int iNode_index){
    struct fd_entry ** cur = (struct fd_entry **) malloc(sizeof(struct fd_entry *));
    for(int i = 0; i < 80; i++){
        *cur = &open_file[i];
        if((*cur)->isAvailable == 0 && (*cur)->i_node_index == iNode_index){
            //This file is already open!
            free(cur);
            return -2;
        }
    }
    for(int i = 0; i < 80; i++){
        *cur = &open_file[i];
        if((*cur)->isAvailable){
            (*cur)->readPtr = 0;
            (*cur)->writePtr = 0;
            (*cur)->isAvailable = 0;
            (*cur)->i_node_index = iNode_index;
            free(cur);
            return i;
        }
    }
    free(cur);
    return -1;
}