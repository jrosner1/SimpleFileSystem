#ifndef Inode_sfs
#define Inode_sfs

//Note iNode table size is 5 blocks supports up to 80 files
struct iNode
{
    //Note iNode is 64 bytes
    int uid;
    int available;
    int size;
    int pointers[12];
    int indirectBlockIndex;
};

//The memory iNode table structure
extern struct iNode iNode_table[80];

//Allocates an iNode at the index specified
int allocate_iNode(int);

//Returns the iNode at the specified index
struct iNode* get_iNode(int);

//Initializes the iNode table in memory
int initialize_iNode_table();

//Writes the iNode table in memory to the disk
int write_iNode_table();

//Reads the iNode table from the disk into memory
int read_iNode_table();

//finds the next available iNode
int find_next_free_inode();

//Checks if the block has at least length bytes available. Returns 1 if if does, 0 otherwise
int file_has_enough_size(int iNode_index, int length);

//Removes a file's iNode from the table
int remove_file(int index);
#endif