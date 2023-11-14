#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>

int checkifElf(Elf64_Ehdr elfHeader){
    unsigned char *fileMagicNumbers = elfHeader.e_ident;

    if(fileMagicNumbers[1] == 'E' && fileMagicNumbers[2] == 'L' && fileMagicNumbers[3] == 'F'){
        return 0;
    }

    return 1;
}

void filesInDirectory(){
    
}

int main(int argc, char *argv[]){
    Elf64_Ehdr elfHeader;
    FILE *filePointer;
    int result;

    filePointer = fopen("target", "rb");
    if(filePointer == NULL){
        return 1;
    }
    fread(&elfHeader, sizeof(elfHeader), 1, filePointer);

    unsigned int entryPoint = elfHeader.e_entry;

    printf("The Entry Point is 0x%x\n", entryPoint);

    result = checkifElf(elfHeader);
    if(result == 0){
        printf("This is an elf file!\n");
    }

    return 0;
}