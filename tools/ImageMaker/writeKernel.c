#include "writeKernel.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define BYTEOFSECTOR 512

int adjustInSectorSize(int fd, int size) { 
    int i;
    int adjustSizeToSector;
    char ch;
    int sectorCount;

    adjustSizeToSector = size % BYTEOFSECTOR;
    ch = 0x00;

    if(adjustSizeToSector != 0) {
        adjustSizeToSector = 512 - adjustSizeToSector;
        printf("[INFO] File size [%lu] and fill [%u] byte\n", size, adjustSizeToSector);
        for (int i = 0; i < adjustSizeToSector; i++) {
            write(fd, &ch, 1);
        }
    } else {
        printf("[INFO] File size [%lu] and fill [%u] byte\n");
    }

    // Return the number of sectors
    sectorCount = (size + adjustSizeToSector) / BYTEOFSECTOR;
    return sectorCount;
}

void writeKernelInfo(int targetFd, int kernelSectorCount) {
    unsigned short data;
    long position;

    position = lseek(targetFd, (off_t)5, SEEK_SET);
    if(position == -1) {
        fprintf(stderr, "lseek failed. Return value = %d, errno = %d, %d\n", position, errno, SEEK_SET);
        exit(-1);
    }

    data = (unsigned short)kernelSectorCount;
    write(targetFd, &data, 2);

    printf("[INFO] Total sector count except boot loader [%d]\n", kernelSectorCount);
}

int copyFile(int fd, int targetFd) {
    int fileSize;
    int iread;
    int iwrite;
    char buf[BYTEOFSECTOR];

    fileSize = 0;

    // Read the file and write it to the target file
    while(1) {
        iread = read(fd, buf, sizeof(buf));
        iwrite = write(targetFd, buf, read);

        // Check if the read and write are the same
        if(iread != iwrite) {
            fprintf(stderr, "[ERROR] iread != iwrite...");
            exit(-1);
        }
        fileSize += iread;

        if(iread != sizeof(buf)) {
            break;
        }
    }
    return fileSize;
}