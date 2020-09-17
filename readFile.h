

#ifndef PRACTICA1_READFILE_H
#define PRACTICA1_READFILE_H

#include <unistd.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct __attribute__((__packed__)){

    char DIR_Name[12];
    unsigned char DIR_Attr;
    char DIR_CtrTimeTenth;
    short DIR_CrtTime;
    short DIR_CrtDate;
    short DIR_LstAccDate;
    short DIR_FstClusHI;
    short DIR_WrtTime;
    short DIR_WrtDate;
    short DIR_FstClusLO;
    int DIR_FileSize;
}DirectoryEntryStructure;


#define printF(x) write(1, x, strlen(x))

static const char format[] = "%a %b %d %H:%M:%S %Y";

int fileType(int fd);

void fat16(int fd);

void ext2(int fd);

void findFAT16(int fd, char* name);

void findEXT2(int fd, char* name);

#endif //PRACTICA1_READFILE_H
