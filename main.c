#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <fcntl.h>
#include "readFile.h"
#include "deleteFile.h"



int main(int argc, char **argv) {

    char opcio = 0;

    if (argc < 2){

        printF("Error. Nombre de parametres incorrecte.\n");
        return -1;
    }

    if(!strcmp(argv[1], "/info")){
        opcio = 1;
    }

    if(!strcmp(argv[1], "/find")){
        opcio = 2;
    }

    if(!strcmp(argv[1], "/delete")){
        opcio = 3;
    }

    int fd;

    switch(opcio){
        case 1:
            if (argc != 3){

                printF("Error. Nombre de parametres incorrecte.\n");
                return -1;
            }

            fd = open(argv[2], O_RDONLY);
            if (fd == -1){

                printF("Error. Volum no trobat.\n");
                return -1;
            }

            char tipus = (char) fileType(fd);
            if (!tipus){

                close(fd);
                printF("Error. Volum no formatat en FAT16 ni EXT2.\n");
                return -1;
            }

            if (tipus == 1){    //FAT16

                fat16(fd);
            }else{              //Ext2

                ext2(fd);
            }

            break;

        case 2:

            if (argc != 4){

                printF("Error. Nombre de parametres incorrecte.\n");
                return -1;
            }

            fd = open(argv[2], O_RDONLY);

            if (fd == -1){

                printF("Error. Volum no trobat.\n");
                return -1;
            }

            char tipus2 = (char) fileType(fd);

            if (!tipus2){

                close(fd);
                printF("Error. Volum no formatat en FAT16 ni EXT2.\n");
                return -1;
            }

            if (tipus2 == 1){    //FAT16

                findFAT16(fd, argv[3]);
            }else{              //Ext2

                findEXT2(fd, argv[3]);
            }

            break;

        case 3:
            if (argc != 4){

                printF("Error. Nombre de parametres incorrecte.\n");
                return -1;
            }

            fd = open(argv[2], O_RDWR);

            if (fd == -1){

                printF("Error. Volum no trobat.\n");
                return -1;
            }

            char tipus3 = (char) fileType(fd);

            if (!tipus3){

                close(fd);
                printF("Error. Volum no formatat en FAT16 ni EXT2.\n");
                return -1;
            }

            if (tipus3 == 1){    //FAT16

                deleteFAT16(fd, argv[3]);
            }else{              //Ext2

                deleteEXT2(fd, argv[3]);
            }

            break;

        default:

            printF("No hi ha un argument valid\n");
            return -1;
            break;
    }

    close(fd);

    return 0;
}