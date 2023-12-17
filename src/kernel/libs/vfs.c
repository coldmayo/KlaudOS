#include "include/stdio.h"
#include "include/vfs.h"
#include <stddef.h>

#define BLOCKSIZE 420

struct SuperBlock SB;
struct inode *inodes;
struct block *dbs;
struct folder *dirs;
struct Disk D;

int initFS() {

    SB.numInodes = 5;
    SB.numBlocks = 100;
    SB.numDirs = 5;
    SB.sizeBlocks = sizeof(struct block);

    SB.currDir = 0;
    SB.ifMounted = 0;

    int i;
    for (i=0; i<SB.numInodes; i++) {
        inodes[i].size = -1;
        inodes[i].firstBlock = -1;
        inodes[i].directory = -1;
        strcpy(inodes[i].name, "klaud__");
    }

    for (i=0; i<SB.numBlocks;i++) {
        dbs[i].nextBlockNum = -1;
    }
    
    for (i=0; i<SB.numDirs;i++) {
        dirs[i].index = -1;
        dirs[i].prevDir = -1;
        strcpy(dirs[i].name, "klaud__");
    }

    return 0;
}

int mountFS() {
    if (SB.ifMounted == 1) {
        printf("error: fs already mounted\n");
        return -1;
    }

    SB.numInodes = 5;
    SB.numBlocks = 100;
    SB.numDirs = 5;
    SB.sizeBlocks = sizeof(struct block);
    int i;
    memcpy(D.data, &SB, sizeof(struct SuperBlock));

    char inodes[sizeof(struct inode)*SB.numInodes];
    char dbs[sizeof(struct block)*SB.numBlocks];
    char dirs[sizeof(struct folder)*SB.numDirs];

    for (i=0; i<SB.numInodes; i++) {
        memcpy(D.data, &(inodes[i]), sizeof(struct inode));
    }

    for (i = 0; i<SB.numBlocks; i++) {
        memcpy(D.data, &(dbs[i]), sizeof(struct block));
    }

    for (i=0; i<SB.numDirs; i++) {
        memcpy(D.data, &(dirs[i]), sizeof(struct folder));
    }

    SB.ifMounted = 1;
    return 0;
}

int unmountFS() {
    if (SB.ifMounted == 1) {
        SB.numInodes = 0;
        SB.numBlocks = 0;
        SB.numDirs = 0;
        SB.sizeBlocks = sizeof(struct block);
        SB.ifMounted = 0;
        return 0;
    }
    return -1;
}

void syncFS() {

    memcpy(D.data, &SB, sizeof(struct SuperBlock));
    int i;
    for (i=0; i<SB.numInodes; i++) {
        memcpy(D.data, &(inodes[i]), sizeof(struct inode));
    }

    for (i = 0; i<SB.numBlocks; i++) {
        memcpy(D.data, &(dbs[i]), sizeof(struct block));
    }

    for (i=0; i<SB.numDirs; i++) {
        memcpy(D.data, &(dirs[i]), sizeof(struct folder));
    }
}

void fsInfo() {
    printf("SuperBlock info:\n");
    printf("# of inodes %d\n", SB.numInodes);
    printf("# of blocks %d\n", SB.numBlocks);
    printf("# of directories %d\n", SB.numDirs);
    printf("block sizes: %d\n", SB.sizeBlocks);
    printf("inode info:\n");
    int i;
    for (i=0;i<SB.numInodes; i++) {
        printf("size: %d first block: %d name: %s location: %d\n", inodes[i].size, inodes[i].firstBlock, inodes[i].name, inodes[i].directory);
    }
    //printf("block info\n");
    //for (i=0;i<SB.numBlocks;i++) {
    //    printf("block num: %d next block %d\n", i, dbs[i].nextBlockNum);
    //}
    printf("directory info\n");
    for (i=0;i<SB.numDirs; i++) {
        printf("dir index: %d prev directory: %d name: %s\n", dirs[i].index, dirs[i].prevDir, dirs[i].name);
    }
}

// find nearest empty inode

int findEmptyInode() {
    int i;
    for(i=0; i<SB.numInodes;i++) {
        if (inodes[i].firstBlock == -1) {
            return i;
        }
    }
    return -1;
}

// find nearest empty block

int findEmptyBlock() {
    int i;
    for(i=0; i<SB.numBlocks;i++) {
        if (dbs[i].nextBlockNum == -1) {
            return i;
        }
    }
    return -1;
}

int makeFile(char * fileName) {
    int inode = findEmptyInode();
    int block = findEmptyBlock();
    inodes[inode].firstBlock = block;
    inodes[inode].directory = SB.currDir;
    dbs[block].nextBlockNum = -2;
    strcpy(&(inodes[inode].name), fileName);
    return inode;
}

void shortenFile(int begNum) {
    int nextNum = dbs[begNum].nextBlockNum;
    if(nextNum >= 0) {
        shortenFile(nextNum);
    }
    dbs[begNum].nextBlockNum;
}

void setFileSize(int fileNum, int size) {
    int tmp = size+BLOCKSIZE;
    int num = tmp/BLOCKSIZE;
    int begNum = inodes[fileNum].firstBlock;
    num--;
    while(num>0) {
        int nextNum = dbs[begNum].nextBlockNum;
        if (nextNum == -2) {
            int empty = findEmptyBlock();
            dbs[begNum].nextBlockNum = empty;
            dbs[empty].nextBlockNum = -2;
        }
        begNum = dbs[begNum].nextBlockNum;
        num--;
    }

    shortenFile(begNum);
    dbs[begNum].nextBlockNum = -2;
    inodes[fileNum].size = size;
}

int delFile(int fileNum) {
    int i;
    int start = inodes[fileNum].firstBlock;
    int end = inodes[fileNum].size/BLOCKSIZE + start;
    for (i=start; end >= i; i++) {
        dbs[i].nextBlockNum = -1;
    }
    inodes[fileNum].size = -1;
    inodes[fileNum].firstBlock = -1;
    inodes[fileNum].directory = -1;
    strcpy(&(inodes[fileNum].name), "klaud__");
    return 0;
}

int getBlockNum (int fileNum, int offset) {
    int begNum = inodes[fileNum].firstBlock;
    int togo = offset;
    
    while (togo>0) {
        begNum = dbs[begNum].nextBlockNum;
        togo--;
    }

    return begNum;
}

void writeToByte(int fileNum, int pos, char *data) {
    int startBlock = pos / BLOCKSIZE;
    int begNum = getBlockNum(fileNum, startBlock);
    int offset = pos % BLOCKSIZE;
    dbs[begNum].data[offset] = (*data);
}

int findFileNum(char* fileName) {
    int i;
    for (i=0;i<SB.numInodes; i++) {
        if (strcmp(inodes[i].name, fileName) == 0) {
            return i;
        }
    }
    return -1;
}

void clearAllData() {
    SB.numInodes = 5;
    SB.numBlocks = 100;
    SB.numDirs = 5;
    SB.sizeBlocks = sizeof(struct block);
    int i;
    for (i=0; i<SB.numInodes; i++) {
        inodes[i].size = -1;
        inodes[i].firstBlock = -1;
        strcpy(inodes[i].name, "klaud__");
        inodes[i].directory = -1;
    }

    for (i=0; i<SB.numBlocks;i++) {
        dbs[i].nextBlockNum = -1;
    }

    for (i=0; i<SB.numDirs;i++) {
        dirs[i].index = -1;
        dirs[i].prevDir = -1;
        strcpy(dirs[i].name, "klaud__");
    }
}

int getCurrDir() {
    return SB.currDir;
}

char * findDirName(int id) {
    int i;
    for (i=0;i<SB.numDirs; i++) {
        if (dirs[i].index == id) {
            return dirs[i].name;
        }
    }

    if (id == 0) {
        return "root";
    }
}

int findEmptyFolder() {
    int i;
    for(i=0; i<SB.numDirs;i++) {
        if (dirs[i].index == -1) {
            return i+1;   // root is i=0
        }
    }
    return -1;
}

// makes directory

void makeFolder(char * foldName) {
    int folder = findEmptyFolder();
    dirs[folder-1].index = folder;
    dirs[folder-1].prevDir = SB.currDir;
    strcpy(dirs[folder-1].name, foldName);
}

int findDirNum(char * foldName) {
    int i;
    for (i=0;i<SB.numDirs; i++) {
        if (strcmp(dirs[i].name, foldName) == 0) {
            return i;
        }
    }
    return -1;
}

// enter into any directory inside of current directory

int cd(char * foldName) {
    int toFold = findDirNum(foldName);
    if (strcmp(foldName, "root") == 0 || strcmp(foldName, ".") == 0 || strcmp(foldName, "\0") == 0) {
        SB.currDir = 0;   // take it back to root
        return 0;
    }

    if (toFold == -1 || dirs[toFold].prevDir != SB.currDir) {
        return -1;
    }

    SB.currDir = toFold+1;
    return 0;
}

// list all files in current directory

void ls() {
    int i;
    printf("files in this directory:\n");
    for (i=0; i<SB.numInodes; i++) {
        if (inodes[i].directory == SB.currDir) {
            printf("%s\n", inodes[i].name);
        }
    }
    printf("folders in this directory:\n");
    for (i=0; i<SB.numDirs; i++) {
        if (dirs[i].prevDir == SB.currDir) {
            printf("%s\n", dirs[i].name);
        }
    }
}