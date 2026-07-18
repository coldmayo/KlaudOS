#include "include/stdio.h"
#include "include/strings.h"
#include "include/mem.h"
#include "include/fs.h"
#include "include/disp.h"
#include <stddef.h>
#include "include/fdc.h"

#define BLOCKSIZE 512

struct SuperBlock SB;

static struct FSImage img;
#define FS_IMAGE_SECTORS ((sizeof(struct FSImage) + 511) / 512)
#define inodes (img.inodes)
#define dbs (img.blocks)
#define dirs (img.dirs)

int initFS(void) {

    if (!init_fdc()) {
        return -1;
    }

    SB.numInodes = 0;
    SB.numBlocks = MAX_BLOCKS;
    SB.numDirs = 0;
    SB.sizeBlocks = sizeof(struct block);

    SB.currDir = 0;
    SB.ifMounted = 0;
    SB.magic = KLAUDFS_MAGIC;

    int i;
    for (i=0; i<MAX_INODES; i++) {
        inodes[i].size = -1;
        inodes[i].firstBlock = -1;
        inodes[i].directory = -1;
        strcpy(inodes[i].name, "klaud__");
    }
    printf("inodes zeroed\n");
    for (i=0; i<MAX_BLOCKS;i++) {
        dbs[i].nextBlockNum = -1;
    }
    printf("blocks zeroed\n");
    for (i=0; i<MAX_DIRS;i++) {
        dirs[i].index = -1;
        dirs[i].prevDir = -1;
        strcpy(dirs[i].name, "klaud__");
    }
    printf("dirs zeroed\n");

    syncFS();
    printf("Syncing done!");
    return 0;
}

// Takes forever, only do this once at the beginning
void syncFS(void) {
    img.sb = SB;
    fdc_write_sectors(FS_START_LBA, &img, FS_IMAGE_SECTORS);
}

void syncRegion(void* struct_ptr, size_t struct_size) {
    // Calculate byte offset from the start of the global image
    uint32_t byte_offset = (uint32_t)((uint8_t*)struct_ptr - (uint8_t*)&img);

    // Find which sectors contain this data
    uint32_t start_sector = byte_offset / BLOCKSIZE;
    uint32_t end_sector = (byte_offset + struct_size + BLOCKSIZE - 1) / BLOCKSIZE;
    uint32_t num_sectors = end_sector - start_sector;

    uint8_t* data_ptr = (uint8_t*)&img + (start_sector * BLOCKSIZE);

    // Write only the sectors we want
    for (uint32_t i = 0; i < num_sectors; i++) {
        fdc_write_sectors(FS_START_LBA + start_sector + i, data_ptr + (i * BLOCKSIZE), 1);
    }
}

int mountFS(void) {
    printf("initializing fdc...\n");
    if (!init_fdc()) {
        return -1;
    }
    printf("FDC initialized\n");
    if (!fdc_read_sectors(FS_START_LBA, FS_IMAGE_SECTORS, &img)) {
        return -1;
    }
    printf("Data was read\n");
    SB = img.sb;
    if (SB.magic != KLAUDFS_MAGIC) {
        return -1;
    }
    printf("Previous FS detected...\n");
    SB.ifMounted = 1;
    syncFS();
    printf("Syncing done!");
    return 0;
}

int unmountFS(void) {
    SB.ifMounted = 0;
    syncFS();
    return 0;
}

int fsInfo(void) {
    printf("SuperBlock info:\n");
    printf("# of inodes %d\n", SB.numInodes);
    printf("# of blocks %d\n", SB.numBlocks);
    printf("# of directories %d\n", SB.numDirs);
    printf("block sizes: %d\n", SB.sizeBlocks);
    printf("File info:\n");
    int i;
    int elems = SB.numInodes + SB.numDirs;
    for (i=0;i<SB.numInodes; i++) {
        printf("size: %d first block: %d name: %s location: %d\n", inodes[i].size, inodes[i].firstBlock, inodes[i].name, inodes[i].directory);
    }
    printf("Directory info\n");
    for (i=0;i<SB.numDirs; i++) {
        printf("dir index: %d prev directory: %d name: %s\n", dirs[i].index, dirs[i].prevDir, dirs[i].name);
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

    int i;
    for (i=0; i<SB.numInodes; i++) {
        if (strcmp(fileName, inodes[i].name) == 0) {
            printf("'%s' name already taken\n", fileName);
            scroll(1);
            return -1;
        }
    }

    int inode = findEmptyInode();
    int block = findEmptyBlock();
    inodes[inode].firstBlock = 1;
    inodes[inode].directory = SB.currDir;
    dbs[block].nextBlockNum = -2;
    strcpy(inodes[inode].name, fileName);

    //printf("Syncing inodes");
    //syncRegion(&inodes[inode], sizeof(struct inode));
    //printf("Syncing blocks");
    //syncRegion(&dbs[block], sizeof(struct block));
    //printf("Syncing SB");
    //syncRegion(&SB, sizeof(struct SuperBlock));
    syncFS();
    return inode;
}

void shortenFile(int begNum) {
    int nextNum = dbs[begNum].nextBlockNum;
    if(nextNum >= 0) {
        shortenFile(nextNum);
    }
    dbs[begNum].nextBlockNum = -1;
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

int makeFolder(char * foldName) {
    addMoreDirs(1);

    int i;
    for (i=0; i<SB.numDirs; i++) {
        if (strcmp(foldName, dirs[i].name) == 0) {
            printf("'%s' name already taken\n", foldName);
            scroll(1);
            return -1;
        }
    }

    int folder = findEmptyFolder();
    dirs[folder-1].index = folder;
    dirs[folder-1].prevDir = SB.currDir;
    strcpy(dirs[folder-1].name, foldName);

    //syncRegion(&dirs[folder-1], sizeof(struct folder));
    //syncRegion(&SB, sizeof(struct SuperBlock));
    syncFS();
    return folder-1;
}

int findDirNum(char *name) {
    for (int i = 0; i < SB.numDirs; i++) {
        if (dirs[i].prevDir == SB.currDir &&
            strcmp(dirs[i].name, name) == 0)
            return i;
    }

    return -1;
}
// enter into any directory inside of current directory

int cd(char *name) {
    //printf("entered cd\n");
    //printf("ptr=%x\n", name);
    //printf("0=%x\n", name[0]);
    //printf("1=%x\n", name[1]);
    //printf("2=%x\n", name[2]);
    if (name == NULL || name[0] == '\0') {
        SB.currDir = 0;
        return 0;
    }

    if (strcmp(name, ".") == 0)
        return 0;

    if (strcmp(name, "..") == 0) {
        if (SB.currDir != 0)
            SB.currDir = dirs[SB.currDir - 1].prevDir;
        return 0;
    }

    if (strcmp(name, "root") == 0) {
        SB.currDir = 0;
        return 0;
    }

    int dir = findDirNum(name);

    if (dir == -1)
        return -1;

    SB.currDir = dirs[dir].index;
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
    for (int i = 0; num > i && SB.numInodes < MAX_INODES; i++) {
        SB.numInodes++;
        int j = SB.numInodes - 1;
        inodes[j].size = -1;
        inodes[j].firstBlock = -1;
        inodes[j].directory = -1;
        strcpy(inodes[j].name, "klaud__");
    }
}

void addMoreDirs(int num) {
    for (int i = 0; num > i && SB.numDirs < MAX_DIRS; i++) {
        SB.numDirs = SB.numDirs + 1;
        int j = SB.numDirs-1;
        dirs[j].index = -1;
        dirs[j].prevDir = -1;
        strcpy(dirs[j].name, "klaud__");
    }
}
