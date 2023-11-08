#include <stdio.h>
#include <linux/dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

#define BUFFERSIZE 2048

int alreadyInfected(){

    return 0;
}

int findFilesinDirectory(){
    int fd;
    struct linux_dirent * directoryEntry;
    int bytesRead;

    fd = open(".", O_RDONLY);
    if (fd > 0){
        printf("[-] error obtaining file descriptor to current working directory\n");
        return -1;
    }
    
    bytesRead = syscall(SYS_getdents, fd, directoryEntry, BUFFERSIZE);

    while (bytesRead > 0){
        if((strcmp(directoryEntry->d_name, ".") != 0) && (strcmp(directoryEntry->d_name, "..")) != 0 ){
            printf("File name: %s\n", directoryEntry->d_name);
        }

        bytesRead = syscall(SYS_getdents, fd, directoryEntry, BUFFERSIZE);
    }

    return 0;
}

int main(int argc, char *argv[]){

    char buffer[5000]; // trying to reserve 5000 bytes of memory to write into

    findFilesinDirectory();

    printf("Infector Machine\n");
    return 0;
}