#ifndef Super_block
#define Super_block

//The structure that will be the super block on the disk
struct super_block{
    char * magic_number;
    int Block_size;
    int File_system_size; //Number of blocks
    int i_node_table_length; //Number of blocks
    int Root_dir; //iNode number
};

//Function to write super_block to the first block on the disk
int write_to_disk(struct super_block*);

//Function that will initialize the super block on setup
int initialize_super(struct super_block*);
#endif