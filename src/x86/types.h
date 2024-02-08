#ifndef __TYPES_H__
#define __TYPES_H__

#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#define BOOL unsigned char

#define TRUE 1
#define FALSE 0
#define NULL 0

#pragma pack(push, 1)

typedef struct {
    BYTE ch;
    BYTE attr;
} CHARACTER;

#pragma pack(pop)

#endif // !1