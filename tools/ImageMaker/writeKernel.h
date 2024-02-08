#ifndef __WRITE_KERNEL_H__
#define __WRITE_KERNEL_H__

#define BYTEOFSECTOR 512

int adjustInSectorSize(int fd, int size);
void writeKernelInfo(int targetFd, int kernelSectorCount);
int copyFile(int fd, int targetFd);

#endif