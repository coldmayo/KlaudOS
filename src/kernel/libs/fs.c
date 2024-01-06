#include "include/stdio.h"
#include "include/strings.h"
#include "include/mem.h"
#include "include/fs.h"
#include <stddef.h>

#define BLOCKSIZE 416

struct SuperBlock SB;
struct inode *inodes;
struct block *dbs;
struct folder *dirs;
struct Disk D;

int initFS(void) {

    SB.numInodes = 1;
    SB.numBlocks = 100;
    SB.numDirs = 1;
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

void syncFS(void) {

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

int fsInfo(void) {
    printf("SuperBlock info:\n");
    printf("# of inodes %d\n", SB.numInodes);
    printf("# of blocks %d\n", SB.numBlocks);
    printf("# of directories %d\n", SB.numDirs);
    printf("block sizes: %d\n", SB.sizeBlocks);
    printf("File info:\n");
    int i;
    int elems;
    for (i=0;i<SB.numInodes; i++) {
        printf("size: %d first block: %d name: %s location: %d\n", inodes[i].size, inodes[i].firstBlock, inodes[i].name, inodes[i].directory);
        elems++;
    }
    printf("Directory info\n");
    for (i=0;i<SB.numDirs; i++) {
        printf("dir index: %d prev directory: %d name: %s\n", dirs[i].index, dirs[i].prevDir, dirs[i].name);
        elems++;
    }
    return elems;
}

// find nearest empty inode

int findEmptyInode(void) {
    int i;
    for(i=0; i<SB.numInodes;i++) {
        if (inodes[i].firstBlock == -1) {
            return i;
        }
    }
    return -1;
}

// find nearest empty block

int findEmptyBlock(void) {
    int i;
    for(i=0; i<SB.numBlocks;i++) {
        if (dbs[i].nextBlockNum == -1) {
            return i;
        }
    }
    return -1;
}

int makeFile(char * fileName) {
    addMoreInodes(1);
    int inode = findEmptyInode();
    int block = findEmptyBlock();
    inodes[inode].firstBlock = 1;
    inodes[inode].directory = SB.currDir;
    dbs[block].nextBlockNum = -2;
    strcpy(inodes[inode].name, fileName);
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

int delFile(char * fileName) {
    int fileNum = findFileNum(fileName);
    if (fileNum == -1) {
        return -1;
    }
    int i;
    int start = inodes[fileNum].firstBlock;
    int end = inodes[fileNum].size/BLOCKSIZE + start;
    for (i=start; end >= i; i++) {
        dbs[i].nextBlockNum = -1;
    }
    inodes[fileNum].size = -1;
    inodes[fileNum].firstBlock = -1;
    inodes[fileNum].directory = -1;
    strcpy((inodes[fileNum].name), "klaud__");
    return 0;
}

int delFolder(char * foldName) {
    int dirNum = findDirNum(foldName);
    if (dirNum == -1) {
        return -1;
    }
    dirs[dirNum].index = -1;
    dirs[dirNum].prevDir = -1;
    strcpy(dirs[dirNum].name, "klaud__");
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

void clearAllData(void) {
    SB.numInodes = 1;
    SB.numBlocks = 100;
    SB.numDirs = 1;
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

int getCurrDir(void) {
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

int findEmptyFolder(void) {
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
    addMoreDirs(1);
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

int ls(void) {
    int i;
    int elems = 0;
    printf("\nfiles in this directory:\n");
    for (i=0; i<SB.numInodes; i++) {
        if (inodes[i].directory == SB.currDir) {
            printf("%s\n", inodes[i].name);
            elems++;
        }
    }
    printf("folders in this directory:\n");
    for (i=0; i<SB.numDirs; i++) {
        if (dirs[i].prevDir == SB.currDir) {
            printf("%s\n", dirs[i].name);
            elems++;
        }
    }
    return elems;
}

void addMoreInodes(int num) {
    for (int i = 0; num > i; i++) {
        SB.numInodes = SB.numInodes+1;
        int j = SB.numInodes-1;
        inodes[j].size = -1;
        inodes[j].firstBlock = -1;
        inodes[j].directory = -1;
        strcpy(inodes[j].name, "klaud__");
    }
}

void addMoreDirs(int num) {
    for (int i = 0; num > i; i++) {
        SB.numDirs = SB.numDirs + 1;
        int j = SB.numDirs-1;
        dirs[j].index = -1;
        dirs[j].prevDir = -1;
        strcpy(dirs[j].name, "klaud__");
    }
}