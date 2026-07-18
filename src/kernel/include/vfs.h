#include "include/stdio.h"

#define BLOCKSIZE   508     // + 4 bytes nextBlockNum = 512 = exactly one sector
#define MAX_INODES  64
#define MAX_BLOCKS  100
#define MAX_DIRS    32

#define FS_DISK_ID   0x81   // second disk in your QEMU setup — adjust to match
#define FS_START_LBA 50   // first 49 used for

struct SuperBlock {
    int numInodes;
    int numBlocks;
    int numDirs;
    int sizeBlocks;
    int ifMounted;
    int currDir;
};

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

struct FSImage {
    struct SuperBlock sb;
    struct inode  inodes[MAX_INODES];
    struct block  blocks[MAX_BLOCKS];
    struct folder dirs[MAX_DIRS];
};

int initFS(void);
int mountFS(void);
void syncFS(void);
int fsInfo(void);
void makeFile(char * fileName);
void setFileSize(int fileNum, int size);
void writeToByte(int fileNum, int pos, char *data);
int findFileNum(char* fileName);
int unmountFS(void);
int delFile(char * fileName);
void clearAllData(void);
void makeFolder(char * foldName);
char * findDirName(int id);
int findDirNum(char * foldName);
int getCurrDir(void);
int cd(char * foldName);
int ls(void);
