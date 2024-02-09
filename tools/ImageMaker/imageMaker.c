#include "writeKernel.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    // file descriptors
    int sourceFd;
    int targetFd;

    // sizes
    int bootLoaderSize;
    int kernel32SectorCount;
    int sourceSize;

    // Check the number of arguments
    if(argc < 3) {
        fprintf(stderr, "[ERROR] ImageMaker BootLoader.bin Kernel32.bin");
        exit(-1);
    }

    // Open the source file
    if((targetFd = open("build/Disk.img", O_RDWR | O_CREAT | O_TRUNC | S_IREAD | S_IWRITE)) == -1) {
        fprintf(stderr, "[ERROR] Disk.img open failed.\n");
        exit(-1);
    }

    // Copy the boot loader to the image file
    printf("[INFO] Copy boot loader to image file\n");
    if((sourceFd = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "[ERROR] %s open failed.\n", argv[1]);
        exit(-1);
    }
    sourceSize = copyFile(sourceFd, targetFd);
    close(sourceFd);

    // Adjust the size of the boot loader to the sector size
    bootLoaderSize = adjustInSectorSize(targetFd, sourceSize);
    printf("[INFO] %s size = [%d] and sector count = [%d]\n", argv[1],
           sourceSize, bootLoaderSize);

    // Copy the protected mode kernel to the image file
    printf("[INFO] Copy protected mode kernel to image file.\n");
    if((sourceFd == open(argv[2], O_RDONLY)) == -1) {
        fprintf(stderr, "[ERROR] %s open failed.\n", argv[2]);
        exit(-1);
    }

    sourceSize = copyFile(sourceFd, targetFd);
    close(sourceFd);

    // Adjust the size of the protected mode kernel to the sector size(512 bytes)
    kernel32SectorCount = adjustInSectorSize(targetFd, sourceSize);
    printf("[INFO] %s size = [%d] and sector count = [%d]\n", argv[2], sourceSize, kernel32SectorCount);

    // Write the kernel information to the image file
    printf("[INFO] Start to write kernel information\n");
    writeKernelInfo(targetFd, kernel32SectorCount);
    printf("[INFO] Image file create complete\n");

    close(targetFd);
    return 0; 
}