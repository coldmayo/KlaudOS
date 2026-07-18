#include "include/stdio.h"

#define BLOCKSIZE 512
#define MAX_INODES 64
#define MAX_DIRS 32
#define MAX_BLOCKS 100

#define FLOPPY_TOTAL_SECTORS 2880   // full 1.44MB image
#define FAT_RESERVED_SECTORS 2000   // must equal FAT_KIB*1024/512 from the Makefile (1000 KiB -> 2000 sectors)
#define FS_START_LBA FAT_RESERVED_SECTORS
#define FS_RESERVED_SECTORS (FLOPPY_TOTAL_SECTORS - FS_START_LBA) // 880 sectors = 450560 bytes for KlaudFS
#define KLAUDFS_MAGIC 0x4B4C4144   // KLAU

struct folder {
    int index;
    int prevDir;
    char name[8];
};

struct inode {
    int size;
    int firstBlock;
    int directory;
    char name[8];
};

struct block {
    int nextBlockNum;
    char data[BLOCKSIZE];
};

struct SuperBlock {
    int numInodes;
    int numBlocks;
    int numDirs;
    int sizeBlocks;
    int ifMounted;
    int currDir;
    uint32_t magic;
};

struct FSImage {
    struct SuperBlock sb;
    struct inode inodes[MAX_INODES];
    struct block blocks[MAX_BLOCKS];
    struct folder dirs[MAX_DIRS];
    uint8_t _pad[512 - (sizeof(struct SuperBlock) + sizeof(struct inode)*MAX_INODES + sizeof(struct block)*MAX_BLOCKS + sizeof(struct folder)*MAX_DIRS) % 512];
};

int initFS(void);
int mountFS(void);
void syncFS(void);
int fsInfo(void);
int makeFile(char * fileName);
void setFileSize(int fileNum, int size);
void writeToByte(int fileNum, int pos, char *data);
int findFileNum(char* fileName);
int unmountFS(void);
int delFile(char * fileName);
void clearAllData(void);
int makeFolder(char * foldName);
char * findDirName(int id);
int findDirNum(char * foldName);
int getCurrDir(void);
int cd(char * foldName);
int ls(void);
char * findDirName(int id);
int delFolder(char * foldName);
void addMoreInodes(int num);
void addMoreDirs(int num);
