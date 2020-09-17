#include "readFile.h"

/*
 * @Return:
 *      1:  FAT16
 *      2:  EXT2
 *      0: Error
 */


int fileType(int fd){

    lseek(fd, 54, SEEK_SET);
    char type[9];
    read(fd, type, 8);
    type[8] = '\0';
    if (!strcmp(type, "FAT16   ")){

        lseek(fd, 0, SEEK_SET);
        return 1;
    }

    lseek(fd, 1024+56, SEEK_SET);
    unsigned short ex2 = 0;

    read(fd, &ex2, 2);

    if (ex2 == 61267){

        lseek(fd, 0, SEEK_SET);
        return 2;
    }

    return 0;
}

void fat16(int fd){
    printF("Filesystem: FAT16\n");
    char* c = (char*)malloc(8);
    lseek(fd, 3, SEEK_SET);
    read(fd, c, 8);
    printF("System Name: ");
    printF(c);
    printF("\n");
    free(c);

    unsigned short s;
    printF("Mida del sector: ");
    lseek(fd, 11, SEEK_SET);
    read(fd, &s, 2);
    c = malloc(10);
    sprintf(c,"%d\n", s);
    printF(c);
    free(c);

    c = (char*)malloc(10);

    lseek(fd, 13, SEEK_SET);
    read(fd, c, 1);
    sprintf(c, "%d\n", (int)c[0]);
    printF("Sectors Per Clauster: ");
    printF(c);
    free(c);

    lseek(fd, 14, SEEK_SET);
    read(fd, &s, 2);
    c = (char*)malloc(10);
    sprintf(c, "%d\n", s);
    printF("Sectors reservats: ");
    printF(c);
    free(c);

    c = (char*)malloc(10);
    lseek(fd, 16, SEEK_SET);
    read(fd, c, 1);
    sprintf(c, "%d\n", (int)c[0]);
    printF("Numero de FATs: ");
    printF(c);
    free(c);

    lseek(fd, 17, SEEK_SET);
    read(fd, &s, 2);
    c = (char*)malloc(10);
    sprintf(c, "%d\n", s);
    printF("MaxRootEntries: ");
    printF(c);
    free(c);

    lseek(fd, 22, SEEK_SET);
    read(fd, &s, 2);
    c = (char*)malloc(10);
    sprintf(c, "%d\n", s);
    printF("Sectors per FAT: ");
    printF(c);
    free(c);

    c = (char*)malloc(11);
    lseek(fd, 43, SEEK_SET);
    read(fd, c, 11);
    printF("Label: ");
    printF(c);
    printF("\n");
    free(c);

}



void ext2(int fd){

    printF("Filesystem: EXT2\n");
    printF("\n");
    printF("INFO INODE\n");
    char *c = (char*) malloc(10);

    unsigned short s = 0;
    lseek(fd, 1024+88, SEEK_SET);
    read(fd, &s, 2);
    printF("Mida Inode: ");
    sprintf(c, "%d\n", s);
    printF(c);
    free(c);

    c = (char*)malloc(10);
    unsigned int i;
    lseek(fd, 1024, SEEK_SET);
    read(fd, &i, 4);
    printF("Num Inodes: ");
    sprintf(c, "%d\n", i);
    printF(c);
    free(c);

    c = (char*)malloc(10);
    lseek(fd, 1024+84, SEEK_SET);
    read(fd, &i, 4);
    printF("Primer Inode: ");
    sprintf(c, "%d\n", i);
    printF(c);
    free(c);

    c = (char*)malloc(10);
    lseek(fd, 1024+40, SEEK_SET);
    read(fd, &i, 4);
    printF("Inodes Grup: ");
    sprintf(c, "%d\n", i);
    printF(c);
    free(c);

    c = (char*)malloc(10);
    lseek(fd, 1024+16, SEEK_SET);
    read(fd, &i, 4);
    printF("Inodes Lliures: ");
    sprintf(c, "%d\n", i);
    printF(c);
    free(c);


    printF("\nINFO BLOC\n");

    c = (char*)malloc(10);
    lseek(fd, 1024+24, SEEK_SET);
    read(fd, &i, 4);
    printF("Mida Bloc: ");
    i = (unsigned int)1024 << i;
    sprintf(c, "%d\n", i);
    printF(c);
    free(c);

    c = (char*)malloc(10);
    lseek(fd, 1024+8, SEEK_SET);
    read(fd, &i, 4);
    printF("Blocs Reservats: ");
    sprintf(c, "%d\n", i);
    printF(c);
    free(c);

    c = (char*)malloc(10);
    lseek(fd, 1024+12, SEEK_SET);
    read(fd, &i, 4);
    printF("Blocs Lliures: ");
    sprintf(c, "%d\n", i);
    printF(c);
    free(c);

    c = (char*)malloc(10);
    lseek(fd, 1024+4, SEEK_SET);
    read(fd, &i, 4);
    printF("Total Blocs: ");
    sprintf(c, "%d\n", i);
    printF(c);
    free(c);

    c = (char*)malloc(10);
    lseek(fd, 1024+20, SEEK_SET);
    read(fd, &i, 4);
    printF("Primer Bloc: ");
    sprintf(c, "%d\n", i);
    printF(c);
    free(c);

    c = (char*)malloc(10);
    lseek(fd, 1024+32, SEEK_SET);
    read(fd, &i, 4);
    printF("Blocs grup: ");
    sprintf(c, "%d\n", i);
    printF(c);
    free(c);

    c = (char*)malloc(10);
    lseek(fd, 1024+36, SEEK_SET);
    read(fd, &i, 4);
    printF("Frags grup: ");
    sprintf(c, "%d\n", i);
    printF(c);
    free(c);


    printF("\nINFO VOLUM\n");

    c = (char*)malloc(16);
    lseek(fd, 1024+120, SEEK_SET);
    read(fd, c, 16);
    printF("Nom volum: ");
    printF(c);
    printF("\n");
    free(c);

    time_t t = 0;
    lseek(fd, 1024+64, SEEK_SET);
    read(fd, &t, 4);
    printF("Ultima comprov: ");
    //sprintf(c, "%d\n", i);
    printF(ctime(&t));


    lseek(fd, 1024+44, SEEK_SET);
    read(fd, &t, 4);
    printF("Ultim muntatge: ");
    //sprintf(c, "%d\n", i);
    printF(ctime(&t));



    lseek(fd, 1024+48, SEEK_SET);
    read(fd, &t, 4);
    printF("Ultima escriptura: ");
    //sprintf(c, "%d\n", i);
    printF(ctime(&t));

}


char block(unsigned int blockGroupByte, unsigned int localInode, int fd, char* nameFile);

void findEXT2(int fd, char* nameFile){

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
            int acumulator = 0;
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
                    lseek(fd, s_log_block_size * bg_inode_table + (s_inode_size * inode) + 4, SEEK_SET);
                    unsigned int file_i_size;
                    read(fd, &file_i_size, sizeof(int));
                    printf("Fitxer trobat. Ocupa %u bytes.\n", file_i_size);
                    flag = 1;
                    break;

                }

                if (file_type == 2 && name[0] != '.'){
                    unsigned int blockGroup = (inode - 1) / s_inodes_per_group;
                    unsigned int localInode = (inode - 1) % s_inodes_per_group;
                    unsigned int blockGroupByte = blockGroup * s_log_block_size * s_blocks_per_group;
                    flag = block(blockGroupByte, localInode, fd, nameFile);

                    if (flag){
                        break;
                    }

                }

                movement += rec_len;
            }

        }
    }

    if (!flag){
        printF("No s'ha trobat el fitxer");
    }

}


char block(unsigned int blockGroupByte, unsigned int localInode, int fd, char* nameFile){

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
                unsigned int blockGroupByte2 = blockGroup * s_log_block_size * s_blocks_per_group;
                lseek(fd, s_log_block_size * bg_inode_table + (s_inode_size * localInode2) + 4 + blockGroupByte2, SEEK_SET);
                unsigned int file_i_size;
                read(fd, &file_i_size, sizeof(int));
                printf("Fitxer trobat. Ocupa %u bytes.\n", file_i_size);
                return 1;

            }

            if (file_type == 2 && name[0] != '.') {
                unsigned int blockGroup = (inode - 1) / s_inodes_per_group;
                unsigned int localInode2 = (inode - 1) % s_inodes_per_group;
                unsigned int blockGroupByte2 = blockGroup * s_log_block_size * s_blocks_per_group;

                if (block(blockGroupByte2, localInode2, fd, nameFile)) {
                    return 1;
                }
            }

            movement += rec_len;
        }
    }

    return 0;
}


char cluster(int fd, unsigned int FirstSectorCluster, char BPB_SecPerClus, int FirstDataSector, char* name);


void findFAT16(int fd, char* name){

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
            lseek(fd, FirstRootDirSecNum*BPB_BytsPerSec + 32*i + 28, SEEK_SET);
            unsigned int size;
            read(fd, &size, sizeof(unsigned int));
            printf("Fitxer trobat. Ocupa %u bytes.", size);
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

            finded = cluster(fd, FirstSectorCluster, BPB_SecPerClus, FirstDataSector, name);

            if (finded){
                break;
            }
        }

    }

    if (!finded){
        printF("No s'ha trobat el fitxer\n");
    }


}

char cluster(int fd, unsigned int FirstSectorCluster, char BPB_SecPerClus, int FirstDataSector, char* name){

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
            lseek(fd, FirstSectorCluster + 32*i + 28, SEEK_SET);
            unsigned int size;
            read(fd, &size, sizeof(unsigned int));
            printf("Fitxer trobat. Ocupa %u bytes.", size);
            return 1;
        }

        if(a.DIR_Attr & 0x10 && a.DIR_Name[0] != '.'){

            lseek(fd, FirstSectorCluster + 32*i + 26, SEEK_SET);
            unsigned short StartingCluster;
            read(fd, &StartingCluster, sizeof(unsigned short));

            unsigned int FirstSectorCluster2 = (unsigned int)((StartingCluster - 2) * BPB_SecPerClus * BPB_BytsPerSec) + FirstDataSector;

            finded = cluster(fd, FirstSectorCluster2, BPB_SecPerClus, FirstDataSector, name);

            if (finded){
                return 1;
            }
        }

    }

    return 0;
}
