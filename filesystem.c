#include "filesystem.h"

Inode *addInode(Inode *fs, char *name) {
    Inode *new_inode = (Inode *)malloc(sizeof (Inode));
    new_inode->name = (char *)malloc(sizeof (char) * (strlen(name) + 1));
    strcpy(new_inode->name, name);
    new_inode->parent = fs;
    for(int i = 0; i < MAX_CHILDREN; i++) {
        new_inode->children[i] = NULL;
    }
    if(fs == NULL) {
        return new_inode;
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if(fs->children[i] == NULL) {
            fs->children[i] = new_inode;
            return fs;
        }
        if(strcmp(name, fs->children[i]->name) == 0) {
            printf ("File %s already exists in /", name);
            free(new_inode->name);
            free(new_inode);
            return fs;
        }
    }
    free(new_inode->name);
    free(new_inode);
    printf ("Directory / is full.\n");
    return fs;
}

// print files and directories in current directory
void printDirectory(struct inode *fs) {
    if(fs == NULL) {
        return;
    }
    for(int i = 0; i < MAX_CHILDREN; i++) {
        if(fs->children[i] != NULL) {
            char *current_name = fs->children[i]->name;
            if(current_name[strlen(current_name) - 1] == '/') {
                printf("Directory: %s\n", current_name);
            } else {
                printf("File: %s\n", current_name);
            }
        }
    }
}

void changeDirectory(struct inode **fs, char *dir) {
    // case parent directory
    if(strcmp(dir, "..") == 0) {
        if((*fs)->parent != NULL) {
            *fs = (*fs)->parent;
        }
        return;
    }
    // case no valid directory
    if(dir[strlen(dir) - 1] != '/') {
        return;
    } else {
        // case root directory
        if(strlen(dir) == 1) {
            return;
        }
    }
    Inode *current = *fs;
    for(int i = 0; i < MAX_CHILDREN; i++) {
        if(current->children[i] != NULL) {
            if(strcmp(dir, current->children[i]->name) == 0) {
                *fs = current->children[i];
                return;
            }
        }
    }
    printf("Directory does not exist!\n");
}

// get absolute path of a file/directory
char *get_full_path(Inode *fs) {
    char aux1[100], aux2[100];
    aux1[0] = 0;
    aux2[0] = 0;
    struct inode *current = fs;
    while(current != NULL) {
        strcpy(aux2, aux1);
        strcpy(aux1, current->name);
        strcat(aux1, aux2);
        current = current->parent;
    }
    char *path = (char *)calloc(strlen(aux1) + 1, sizeof(char));
    strcpy(path, aux1);
    return path;
}

void printFullPath(struct inode *fs) {
    char *path = get_full_path(fs);
    printf("Full path: %s\n", path);
    free(path);
}

void printDirectoryRecursive(struct inode *fs) {
    if(fs == NULL) {
        return;
    }
    printDirectory(fs);
    for(int i = 0; i < MAX_CHILDREN; i++) {
        if(fs->children[i] != NULL) {
            printDirectoryRecursive(fs->children[i]);
        }
    }
}

void removeInode(struct inode *fc, char *fn) {
    // decide if it is directory
    int directory;
    if(fn[strlen(fn) - 1] == '/') {
        directory = 1;
    } else {
        directory = -1;
    }
    for(int i = 0; i < MAX_CHILDREN; i++) {
        if(fc->children[i] != NULL) {
            if(strcmp(fn, fc->children[i]->name) == 0) {
                if(directory == -1) {
                    // file
                    free(fc->children[i]->name);
                    free(fc->children[i]);
                    fc->children[i] = NULL;
                    return;
                } else {
                    // directory
                    int is_empty_directory = 0;
                    Inode *directory = fc->children[i];
                    for(int j = 0; j < MAX_CHILDREN; j++) {
                        if(directory->children[j] != NULL) {
                            is_empty_directory ++;
                            break;
                        }
                    }
                    if(is_empty_directory == 0) {
                        // empty directory
                        free(fc->children[i]->name);
                        free(fc->children[i]);
                        fc->children[i] = NULL;
                        return;
                    } else {
                        // full directory
                        printf("Cannot delete non-empty directory.\n");
                        return;
                    }
                }
            }
        }
    }
    printf("File or directory %s does not exist in %s\n", fn, fc->name);
}

void sortDirectory(struct inode *fs) {
    for(int i = 0; i < MAX_CHILDREN - 1; i++) {
        for(int j = i + 1; j < MAX_CHILDREN; j++) {
            if(fs->children[i] != NULL && fs->children[j] != NULL) {
                if(strcmp(fs->children[i]->name, fs->children[j]->name) > 0) {
                    Inode *aux = fs->children[i];
                    fs->children[i] = fs->children[j];
                    fs->children[j] = aux;
                }
            }
        }
    }
}

void findInodeAnywhere(struct inode *fs, char *name) {
    for(int i = 0; i < MAX_CHILDREN; i++) {
        if(fs->children[i] != NULL) {
            if(strcmp(name, fs->children[i]->name) == 0) {
                char *path = get_full_path(fs->children[i]);
                printf("%s\n", path);
                free(path);
            }
            findInodeAnywhere(fs->children[i], name);
        }
    }
}

struct inode *changeDirectoryAbsolute(struct inode *fs, char *dir) {
    if(dir[0] != '/') {
        return NULL;
    }
    Inode *current_dir = fs;
    while(current_dir->parent != NULL) {
        current_dir = current_dir->parent;
    }
    
    char *found = dir;
    char *found2 = strchr(found + 1, '/');
    if(found2 == NULL) {
        if(strlen(dir) == 1) {
            return current_dir;
        } else {
            return NULL;
        }
    }
    char dirr_name[100];
    int index = 0;
    for(char *aux = found + 1; aux <= found2; aux++) {
        dirr_name[index++] = *aux;
    }
    dirr_name[index] = 0;
    changeDirectory(&current_dir, dirr_name);
    
    while(1) {
        found = strchr(found2 + 1, '/');
        if(found == NULL) {
            if(*(found2 + 1) == '\0') {
                return current_dir;
            } else {
                return NULL;
            }
        } else {
            index = 0;
            for(char *aux = found2 + 1; aux <= found; aux++) {
                dirr_name[index++] = *aux;
            }
            dirr_name[index] = 0;
            changeDirectory(&current_dir, dirr_name);
            found2 = found;
        }
    }
}

void move(struct inode *fSrc, struct inode *fDst) {
    if(fSrc == NULL) {
        printf("Source directory doesn't exist.\n");
        return;
    }
    if(fDst == NULL) {
        printf("Destination directory doesn't exist.\n");
        return;
    }
    if(fDst->name[strlen(fDst->name) - 1] != '/') {
        printf("Destination is not a directory.\n");
        return;
    }
    if(strcmp(fSrc->name, "/") == 0) {
        printf("Source directory can't be root.\n");
        return;
    }

    int found = 0;
    for(int i = 0; i < MAX_CHILDREN; i++) {
        if(fDst->children[i] == NULL) {
            // delete source from parent
            for(int i = 0; i < MAX_CHILDREN; i++) {
                if(fSrc->parent->children[i] == fSrc) {
                    fSrc->parent->children[i] = NULL;
                    fSrc->parent = NULL;
                    break;
                }
            }
            fDst->children[i] = fSrc;
            fSrc->parent = fDst;
            // mark as moved
            found++;
            break;
        }
    }
    if(!found) {
        printf("Unable to move.\n");
    }
}

int main () {
    // buffers
    char command;
    char name [100], dir[100];
    char source[50];
    char destination[50];
    // Create root directory
    Inode *fc = addInode(NULL, "/");
    Inode *root = fc;
    while (1) {
        // get command
        // printf ("Command? ");
        char *absolutePath = get_full_path(fc);
        printf("user:%s ", absolutePath);
        free(absolutePath);
        scanf(" %c", &command);
        switch (command) {
            // exit program
            case 'q':
                printf ("Program terminating! \n");
                return 0;
                break;
            // add file or directory to current directory
            case 'a':
                printf ("Give filename: ");
                scanf (" %s", name);
                fc = addInode(fc, name);
                break;
            // print the current directory
            case 'p':
                printDirectory(fc);
                break;
            // change path
            case 'c':
                printf ("Change path to: ");
                scanf (" %s", dir);
                changeDirectory(&fc, dir);
                break;
            // print absolute path of current directory
            case 'f':
                printFullPath(fc);
                break;
            // print directory recursively
            case 'P':
                printDirectoryRecursive(fc);
                break;
            // remove file or directory
            case 'r':
                printf ("Give filename: ");
                scanf (" %s", name);
                removeInode(fc, name);
                break;
            case 's':
                sortDirectory(fc);
                break;
            // find all occurences of a file or directory
            case 'F':
                printf ("Give filename: ");
                scanf (" %s", name);
                findInodeAnywhere(fc, name);
                break;
            // change path to an absolute path
            case 'C':
                printf ("Change path to: ");
                scanf (" %s", name);
                Inode *aux = changeDirectoryAbsolute(fc, name);
                if(aux != NULL) {
                    fc = aux;
                } else {
                    printf("Directory does not exist.\n");
                }
                break;
            // move source to destination directory
            case 'm':
                printf("Source path: ");
                scanf("%s", source);
                printf("Destination path: ");
                scanf("%s", destination);
                Inode *src_inode = changeDirectoryAbsolute(root, source);
                Inode *dest_src = changeDirectoryAbsolute(root, destination);
                move(src_inode, dest_src);
                break;
            default: 
                printf("Unknown command '%c' \n", command);
                break;
        }
    }
    // finish with success
    return 0;
}
