#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILDREN 10

typedef struct inode {
    char *name;
    struct inode *parent;
    struct inode *children [MAX_CHILDREN];
}Inode;

Inode *addInode(Inode *fs, char *name);

void printDirectory(struct inode *fs);

void changeDirectory(struct inode **fs, char *dir);

char *get_full_path(Inode *fs);

void printFullPath(struct inode *fs);

void printDirectoryRecursive(struct inode *fs);

void removeInode(struct inode *fc, char *fn);

void sortDirectory(struct inode *fs);

void findInodeAnywhere(struct inode *fs, char *name);

struct inode *changeDirectoryAbsolute(struct inode *fs, char *dir);

void move(struct inode *fSrc, struct inode *fDst);