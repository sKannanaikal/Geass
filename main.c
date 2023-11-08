#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fnctl.h>
#include <unistd.h>
#include <sys/syscall.h>

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

    bytesRead = sys_getdents64(fd, directoryEntry, sizeof(struct linux_dirent));

    while (bytesRead > 0){
        if((strcmp(directoryEntry->d_name, ".") != 0) && (strcmp(directoryEntry->d_name, "..")) != 0 ){
            printf("File name: %s\n", directoryEntry->d_name);
        }

        bytesRead = sys_getdents64(fd, directoryEntry, sizeof(struct linux_dirent));
    }

    return 0;
}

int main(int argc char *argv[]){

    char buffer[5000]; // trying to reserve 5000 bytes of memory to write into

    findFilesinDirectory();

    printf("Infector Machine\n");
    return 0;
}