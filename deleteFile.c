#include <unistd.h>
#include <strings.h>
#include "deleteFile.h"
#include "readFile.h"

char cluster2(int fd, unsigned int FirstSectorCluster, char BPB_SecPerClus, int FirstDataSector, char* name);

void deleteFAT16(int fd, char* name){
    char finded = 0;

    lseek(fd, 22, SEEK_SET);
    short  BPB_FATSz16;
    read(fd, &BPB_FATSz16, 2);

    lseek(fd, 14, SEEK_SET);
    short BPB_ResvdSecCnt;
    read(fd, &BPB_ResvdSecCnt, 2);

    lseek(fd, 16, SEEK_SET);
    char BPB_NumFATs;
    read(fd, &BPB_NumFATs, 1);

    lseek(fd, 17, SEEK_SET);
    short BPB_RootEntCnt;
    read(fd, &BPB_RootEntCnt, 2);

    lseek(fd, 11, SEEK_SET);
    short BPB_BytsPerSec;
    read(fd, &BPB_BytsPerSec, 2);

    int RootDirSectors = ((BPB_RootEntCnt * 32) + (BPB_BytsPerSec - 1)) / BPB_BytsPerSec;
    int FirstDataSector = BPB_ResvdSecCnt + (BPB_NumFATs * BPB_FATSz16) + RootDirSectors;
    FirstDataSector *= BPB_BytsPerSec;

    lseek(fd, FirstDataSector, SEEK_SET);

    int FirstRootDirSecNum = BPB_ResvdSecCnt + (BPB_NumFATs * BPB_FATSz16);

    lseek(fd, FirstRootDirSecNum*BPB_BytsPerSec, SEEK_SET);

    DirectoryEntryStructure a;

    int i;
    for(i = 0; i < BPB_RootEntCnt; i++) {
        lseek(fd, FirstRootDirSecNum*BPB_BytsPerSec + 32*i, SEEK_SET);
        read(fd, &a.DIR_Name, 11);
        a.DIR_Name[11] = '\0';
        read(fd, &a.DIR_Attr, 1);

        if ((unsigned char) a.DIR_Name[0] == 0xE5){
            continue;
        }

        if (a.DIR_Name[0] == 0x00){
            break;
        }

        if (a.DIR_Name[0] == 0x05){
            a.DIR_Name[0] = (char) 0xE5;
        }

        unsigned char flag = a.DIR_Attr&(unsigned char)0x20;

        int j = 7;
        char aux[13] = "\0\0\0\0\0\0\0\0\0\0\0\0\0";
        while(1){
            if(a.DIR_Name[j] != ' '){
                break;
            }
            j--;
        }
        aux[j+1] = '.';
        for(;j >= 0; j--){
            aux[j] = a.DIR_Name[j];
        }

        int pepe;
        if(a.DIR_Name[8] != ' '){
            pepe = 0;
            while(aux[pepe] != '.'){
                pepe++;
            }
            pepe++;

            j = 8;
            while(j < 11 && a.DIR_Name[j] != ' '){
                aux[pepe] = a.DIR_Name[j];
                j++;
                pepe++;
            }

        }else{
            pepe = 0;
            while(aux[pepe] != '.'){
                pepe++;
            }
            aux[pepe] = '\0';
        }

        if(flag != 0 && !strcasecmp(aux, name)){
            while(1){
                i++;
                lseek(fd, FirstRootDirSecNum*BPB_BytsPerSec + 32*i, SEEK_SET);
                char info[32];
                read(fd, info, sizeof(char)*32);
                lseek(fd, FirstRootDirSecNum*BPB_BytsPerSec + 32*(i-1), SEEK_SET);
                write(fd, info, sizeof(char)*32);

                if (info[0] == 0x00){
                    break;
                }
            }
            printF("El fitxer ");
            printF(name);
            printF(" ha estat eliminat.\n");
            finded = 1;
            break;
        }

        if(a.DIR_Attr & 0x10){

            lseek(fd, FirstRootDirSecNum*BPB_BytsPerSec + 32*i + 26, SEEK_SET);
            unsigned short StartingCluster;
            read(fd, &StartingCluster, sizeof(unsigned short));

            lseek(fd, 13, SEEK_SET);
            char BPB_SecPerClus;
            read(fd, &BPB_SecPerClus, sizeof(char));

            unsigned int FirstSectorCluster = (unsigned int)((StartingCluster - 2) * BPB_SecPerClus * BPB_BytsPerSec) + FirstDataSector;

            finded = cluster2(fd, FirstSectorCluster, BPB_SecPerClus, FirstDataSector, name);

            if (finded){
                break;
            }
        }

    }

    if (!finded){
        printF("No s'ha trobat el fitxer\n");
    }
}


char cluster2(int fd, unsigned int FirstSectorCluster, char BPB_SecPerClus, int FirstDataSector, char* name){

    unsigned short MaxRootEntries;
    lseek(fd, 17, SEEK_SET);
    read(fd, &MaxRootEntries, 2);

    unsigned short BPB_BytsPerSec;
    lseek(fd, 11, SEEK_SET);
    read(fd, &BPB_BytsPerSec, 2);

    DirectoryEntryStructure a;

    int i;
    char finded = 0;
    for(i = 0; i*32 < BPB_BytsPerSec; i++) {
        lseek(fd, FirstSectorCluster + 32*i, SEEK_SET);
        read(fd, &a.DIR_Name, 11);
        a.DIR_Name[11] = '\0';
        read(fd, &a.DIR_Attr, 1);

        if ((unsigned char) a.DIR_Name[0] == 0xE5){
            continue;
        }

        if (a.DIR_Name[0] == 0x00){
            break;
        }

        if (a.DIR_Name[0] == 0x05){
            a.DIR_Name[0] = (char) 0xE5;
        }

        unsigned char flag = a.DIR_Attr&(unsigned char)0x20;

        int j = 7;
        char aux[13] = "\0\0\0\0\0\0\0\0\0\0\0\0\0";
        while(1){
            if(a.DIR_Name[j] != ' '){
                break;
            }
            j--;
        }
        aux[j+1] = '.';
        for(;j >= 0; j--){
            aux[j] = a.DIR_Name[j];
        }

        int pepe;
        if(a.DIR_Name[8] != ' '){
            pepe = 0;
            while(aux[pepe] != '.'){
                pepe++;
            }
            pepe++;

            j = 8;
            while(j < 11 && a.DIR_Name[j] != ' '){
                aux[pepe] = a.DIR_Name[j];
                j++;
                pepe++;
            }

        }else{
            pepe = 0;
            while(aux[pepe] != '.'){
                pepe++;
            }
            aux[pepe] = '\0';
        }

        if(flag != 0 && !strcasecmp(aux, name)){
            while(1){
                i++;
                lseek(fd, FirstSectorCluster + 32*i, SEEK_SET);
                char info[32];
                read(fd, info, sizeof(char)*32);
                lseek(fd, FirstSectorCluster + 32*(i-1), SEEK_SET);
                write(fd, info, sizeof(char)*32);

                if (info[0] == 0x00){
                    break;
                }
            }
            printF("El fitxer ");
            printF(name);
            printF(" ha estat eliminat.\n");
            return 1;
        }

        if(a.DIR_Attr & 0x10 && a.DIR_Name[0] != '.'){

            lseek(fd, FirstSectorCluster + 32*i + 26, SEEK_SET);
            unsigned short StartingCluster;
            read(fd, &StartingCluster, sizeof(unsigned short));

            unsigned int FirstSectorCluster2 = (unsigned int)((StartingCluster - 2) * BPB_SecPerClus * BPB_BytsPerSec) + FirstDataSector;

            finded = cluster2(fd, FirstSectorCluster2, BPB_SecPerClus, FirstDataSector, name);

            if (finded){
                return 1;
            }
        }

    }

    return 0;
}


char block2(unsigned int blockGroupByte, unsigned int localInode, int fd, char* nameFile);

void deleteEXT2(int fd, char* nameFile){

    char flag = 0;

    unsigned int s_inodes_count;
    lseek(fd, 1024, SEEK_SET);
    read(fd, &s_inodes_count, sizeof(int));

    unsigned int s_blocks_per_group;
    lseek(fd, 1024 + 32, SEEK_SET);
    read(fd, &s_blocks_per_group, sizeof(unsigned int));

    unsigned int s_free_inodes_count;
    lseek(fd, 1024 + 16, SEEK_SET);
    read(fd, &s_free_inodes_count, sizeof(int));

    unsigned int s_inodes_per_group;
    lseek(fd, 1024 + 40, SEEK_SET);
    read(fd, &s_inodes_per_group, sizeof(unsigned int));

    unsigned int s_block_count;
    lseek(fd, 1024 + 4, SEEK_SET);
    read(fd, &s_block_count, sizeof(unsigned int));

    unsigned short s_inode_size = 0;
    lseek(fd, 1024+88, SEEK_SET);
    read(fd, &s_inode_size, 2);

    lseek(fd, 1024 + 24, SEEK_SET);
    unsigned int s_log_block_size;
    read(fd, &s_log_block_size, sizeof(int));
    s_log_block_size = (unsigned int) 1024 << s_log_block_size;

    lseek(fd, 1024 + 1024 + 4, SEEK_SET);
    unsigned int bg_inode_bitmap;
    read(fd, &bg_inode_bitmap, sizeof(int));

    lseek(fd, 1024 + 1024 + 8, SEEK_SET);
    unsigned int bg_inode_table;
    read(fd, &bg_inode_table, sizeof(int));

    unsigned int j;
    for(j = 0; j < s_inodes_count - s_free_inodes_count && !flag; j++) {

        lseek(fd, s_log_block_size * bg_inode_table + (s_inode_size * j) + 28, SEEK_SET);
        int i_blocks;
        read(fd, &i_blocks, sizeof(int));
        i_blocks = (int)(i_blocks / (((float)(1024<<s_log_block_size))/(float)512));

        char name[255];
        int i;
        for (i = 0; i < i_blocks && !flag; i++) {
            //Lectura dels punters als Linked List Directory (file info)
            lseek(fd, (s_log_block_size * bg_inode_table) + (s_inode_size * j) + 40 + (i * sizeof(int)), SEEK_SET);
            int i_block;
            read(fd, &i_block, sizeof(int));

            int movement =  s_log_block_size * i_block;
            int pastMovement = s_log_block_size * i_block;
            int acumulator = 0;
            unsigned short past_rec_len = 0;
            for(; acumulator < 1023;) {
                //Linked List Directory (file info)
                lseek(fd, movement, SEEK_SET);
                unsigned  int inode;
                read(fd, &inode, sizeof(int));

                lseek(fd, movement + 4, SEEK_SET);
                unsigned short rec_len;
                read(fd, &rec_len, sizeof(short));
                acumulator += rec_len;

                if (!rec_len){
                    break;
                }

                lseek(fd, movement + 6, SEEK_SET);
                char name_len;
                read(fd, &name_len, sizeof(char));

                lseek(fd, movement + 7, SEEK_SET);
                char file_type;
                read(fd, &file_type, sizeof(char));

                lseek(fd, movement + 8, SEEK_SET);
                read(fd, name, (size_t) name_len);
                name[(int) name_len] = '\0';



                if(file_type == 1 && !strcasecmp(nameFile, name)) {
                    flag = 1;

                    unsigned short W_rec_len = rec_len + past_rec_len;

                    lseek(fd, pastMovement + 4, SEEK_SET);
                    write(fd, &W_rec_len, sizeof(short));

                    //bitmap modification
                    lseek(fd, s_log_block_size * bg_inode_bitmap + (int)((inode-1)/8), SEEK_SET);
                    char bitmap_byte;
                    read(fd, &bitmap_byte, sizeof(char));

                    bitmap_byte = bitmap_byte & ~(((char)0x01) << (((inode - 1)%8)));

                    lseek(fd, s_log_block_size * bg_inode_bitmap + (int)((inode-1)/8), SEEK_SET);
                    write(fd, &bitmap_byte, sizeof(char));

                    lseek(fd, 1024 + 16, SEEK_SET);
                    s_free_inodes_count++;
                    write(fd, &s_free_inodes_count, sizeof(int));

                    printF("El fitxer ");
                    printF(name);
                    printF(" ha estat eliminat.\n");

                    break;

                }

                if (file_type == 2 && name[0] != '.'){
                    unsigned int blockGroup = (inode - 1) / s_inodes_per_group;
                    unsigned int localInode = (inode - 1) % s_inodes_per_group;
                    unsigned int blockGroupByte = blockGroup * s_log_block_size * s_blocks_per_group;
                    flag = block2(blockGroupByte, localInode, fd, nameFile);

                    if (flag){
                        break;
                    }

                }
                past_rec_len = rec_len;
                pastMovement = movement;
                movement += rec_len;
            }

        }
    }

    if (!flag){
        printF("No s'ha trobat el fitxer");
    }

}


char block2(unsigned int blockGroupByte, unsigned int localInode, int fd, char* nameFile){

    unsigned int s_inodes_count;
    lseek(fd, 1024 + blockGroupByte, SEEK_SET);
    read(fd, &s_inodes_count, sizeof(int));

    unsigned int s_free_inodes_count;
    lseek(fd, 1024 + 16 + blockGroupByte, SEEK_SET);
    read(fd, &s_free_inodes_count, sizeof(int));

    unsigned int s_blocks_per_group;
    lseek(fd, 1024 + 32 + blockGroupByte, SEEK_SET);
    read(fd, &s_blocks_per_group, sizeof(unsigned int));

    unsigned int s_inodes_per_group;
    lseek(fd, 1024 + 40 + blockGroupByte, SEEK_SET);
    read(fd, &s_inodes_per_group, sizeof(unsigned int));

    unsigned int s_block_count;
    lseek(fd, 1024 + 4 + blockGroupByte, SEEK_SET);
    read(fd, &s_block_count, sizeof(unsigned int));

    unsigned short s_inode_size = 0;
    lseek(fd, 1024 + 88 + blockGroupByte, SEEK_SET);
    read(fd, &s_inode_size, 2);

    lseek(fd, 1024 + 24 + blockGroupByte, SEEK_SET);
    unsigned int s_log_block_size;
    read(fd, &s_log_block_size, sizeof(int));
    s_log_block_size = (unsigned int) 1024 << s_log_block_size;

    lseek(fd, 1024 + 1024 + 8 + blockGroupByte, SEEK_SET);
    unsigned int bg_inode_table;
    read(fd, &bg_inode_table, sizeof(int));

    lseek(fd, 1024 + 1024 + 8 + blockGroupByte, SEEK_SET);
    unsigned int bg_inode_bitmap;
    read(fd, &bg_inode_bitmap, sizeof(int));

    lseek(fd, s_log_block_size * bg_inode_table + (s_inode_size * localInode) + 28 + blockGroupByte, SEEK_SET);
    int i_blocks;
    read(fd, &i_blocks, sizeof(int));
    i_blocks = (int)(i_blocks / (((float)(1024<<s_log_block_size))/(float)512));

    char name[255];
    int i;
    char flag = 0;
    for (i = 0; i < i_blocks && !flag; i++) {
        //Lectura dels punters als Linked List Directory (file info)
        lseek(fd, (s_log_block_size * bg_inode_table) + (s_inode_size * localInode) + 40 + (i * sizeof(int)) + blockGroupByte, SEEK_SET);
        int i_block;
        read(fd, &i_block, sizeof(int));

        unsigned int movement = s_log_block_size * i_block;
        unsigned int pastMovement = s_log_block_size * i_block;
        unsigned short past_rec_len = 0;
        int acumulator = 0;
        for (; acumulator < 1023;) {
            //Linked List Directory (file info)
            lseek(fd, movement, SEEK_SET);
            unsigned int inode;
            read(fd, &inode, sizeof(int));

            lseek(fd, movement + 4, SEEK_SET);
            unsigned short rec_len;
            read(fd, &rec_len, sizeof(short));
            acumulator += rec_len;

            if (!rec_len) {
                break;
            }

            lseek(fd, movement + 6, SEEK_SET);
            char name_len;
            read(fd, &name_len, sizeof(char));

            lseek(fd, movement + 7, SEEK_SET);
            char file_type;
            read(fd, &file_type, sizeof(char));

            lseek(fd, movement + 8, SEEK_SET);
            read(fd, name, (size_t) name_len);
            name[(int) name_len] = '\0';


            if (file_type == 1 && !strcasecmp(nameFile, name)) {
                unsigned int blockGroup = (inode - 1) / s_inodes_per_group;
                unsigned int localInode2 = (inode - 1) % s_inodes_per_group;
                //bitmap modification
                unsigned int locationBitmap = blockGroup * s_log_block_size * s_blocks_per_group + s_log_block_size * bg_inode_bitmap + (int)((localInode2 - 1)/8);
                lseek(fd, locationBitmap, SEEK_SET);
                char bitmapByte;
                read(fd, &bitmapByte, sizeof(char));

                bitmapByte = bitmapByte & ~(((char)0x01) << (((localInode2 - 1)%8)));

                lseek(fd, locationBitmap, SEEK_SET);
                write(fd, &bitmapByte, sizeof(char));

                unsigned short W_rec_len = rec_len + past_rec_len;
                lseek(fd, pastMovement + 4, SEEK_SET);
                write(fd, &W_rec_len, sizeof(short));

                lseek(fd, 1024 + 16 + blockGroupByte, SEEK_SET);
                s_free_inodes_count++;
                write(fd, &s_free_inodes_count, sizeof(int));

                printF("El fitxer ");
                printF(name);
                printF(" ha estat eliminat.\n");

                return 1;

            }

            if (file_type == 2 && name[0] != '.') {
                unsigned int blockGroup = (inode - 1) / s_inodes_per_group;
                unsigned int localInode2 = (inode - 1) % s_inodes_per_group;
                unsigned int blockGroupByte2 = blockGroup * s_log_block_size * s_blocks_per_group;

                if (block2(blockGroupByte2, localInode2, fd, nameFile)) {
                    return 1;
                }
            }

            past_rec_len = rec_len;
            pastMovement = movement;
            movement += rec_len;
        }
    }

    return 0;
}


