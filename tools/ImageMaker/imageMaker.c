#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h> /* windows 에서는 io.h */
#include <unistd.h>

#define BYTESOFSECTOR 512

int adjustInSectorSize(int fd, int sourceSize);
void writeKernelInfo(int targetFd, int kernelSectorCount);
int copyFile(int sourceFd, int targetFd);

// main 함수
int main(int argc, char* argv[]) {
    int sourceFd;
    int targetFd;
    int iBootLoaderSize;
    int iKernel32SectorCount;
    int sourceSize;

    // process argument
    if (argc < 3) {
        fprintf(stderr, "[ERROR] ImageMaker.exe BootLoader.bin Kernel32.bin\n");
        exit(-1);
    }

    // create disk image
    if ((targetFd = open("build/OS.img", O_RDWR | O_CREAT | O_TRUNC | S_IREAD | S_IWRITE)) == -1) {
        fprintf(stderr, "[ERROR] Disk.img open fail.\n");
        exit(-1);
    }


    printf("[INFO] Copy boot loader to image file\n");
    if ((sourceFd = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "[ERROR] %s open fail\n", argv[1]);
        exit(-1);
    }

    sourceSize = copyFile(sourceFd, targetFd);
    close(sourceFd);

    iBootLoaderSize = adjustInSectorSize(targetFd, sourceSize);
    printf("[INFO] %s size = [%d] and sector count = [%d]\n", argv[1],
           sourceSize, iBootLoaderSize);

    // copy protected mode kernel to image file
    printf("[INFO] Copy protected mode kernel to image file\n");
    if ((sourceFd = open(argv[2], O_RDONLY)) == -1) {
        fprintf(stderr, "[ERROR] %s open fail\n", argv[2]);
        exit(-1);
    }

    sourceSize = copyFile(sourceFd, targetFd);
    close(sourceFd);

    // fill the rest of sector with 0x00
    iKernel32SectorCount = adjustInSectorSize(targetFd, sourceSize);
    printf("[INFO] %s size = [%d] and sector count = [%d]\n", argv[2],
           sourceSize, iKernel32SectorCount);

    // refresh the disk image file
    printf("[INFO] Start to write kernel information\n");
    // 부트섹터의 5번째 바이트부터 커널에 대한 정보를 넣음
    writeKernelInfo(targetFd, iKernel32SectorCount);
    printf("[INFO] Image file create complete\n");

    close(targetFd);
    return 0;
}

int adjustInSectorSize(int fd, int sourceSize) {
    int i;
    int adjustSizeToSector;
    char ch;
    int iSectorCount;

    adjustSizeToSector = sourceSize % BYTESOFSECTOR;
    ch = 0x00;

    if (adjustSizeToSector != 0) {
        adjustSizeToSector = 512 - adjustSizeToSector;
        printf("[INFO] File size [%lu] and fill [%u] byte\n", sourceSize,
               adjustSizeToSector);
        for (i = 0; i < adjustSizeToSector; i++) {
            write(fd, &ch, 1);
        }
    } else {
        printf("[INFO] File size is aligned 512 byte\n");
    }

    iSectorCount = (sourceSize + adjustSizeToSector) / BYTESOFSECTOR;
    return iSectorCount;
}

void writeKernelInfo(int targetFd, int kernelSectorCount) {
    unsigned short usData;
    long lPosition;

    lPosition = lseek(targetFd, 5, SEEK_SET);
    if (lPosition == -1) {
        fprintf(stderr, "lseek fail. Return value = %d, errno = %d, %d\n",
                lPosition, errno, SEEK_SET);
        exit(-1);
    }

    usData = (unsigned short)kernelSectorCount;
    write(targetFd, &usData, 2);

    printf("[INFO] Total sector count except boot loader [%d]\n",
           kernelSectorCount);
}

int copyFile(int sourceFd, int targetFd) {
    int iSourceFileSize;
    int iRead;
    int iWrite;
    char vcBuffer[BYTESOFSECTOR];

    iSourceFileSize = 0;
    while (1) {
        iRead = read(sourceFd, vcBuffer, sizeof(vcBuffer));
        iWrite = write(targetFd, vcBuffer, iRead);

        if (iRead != iWrite) {
            fprintf(stderr, "[ERROR] iRead != iWrite.. \n");
            exit(-1);
        }
        iSourceFileSize += iRead;

        if (iRead != sizeof(vcBuffer)) {
            break;
        }
    }
    return iSourceFileSize;
}
