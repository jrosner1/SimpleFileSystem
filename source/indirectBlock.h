#include "Inode_sfs.h"
#include "fdescriptor.h"
#ifndef indirectBlock
#define indirectBlock
extern int iBlock[256];

//Gets the pointer to the index'th block in the indirect block
int get_block_from_index(struct iNode*, int);

//Adds block pointer to indirectBlock
int place_block(struct iNode*, int, int);

//Creates the indirect block, places first block pointer
int create_indirect_block(struct iNode*, int);

#endif