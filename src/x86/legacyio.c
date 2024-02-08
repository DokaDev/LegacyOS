#include "legacyio.h"
#include "types.h"

void printStr(const char* str, BYTE color, int x, int y) {
    CHARACTER* video = (CHARACTER*)0xB8000;

    video += y * 80 + x;
    for (int i = 0; str[i] != 0; i++) {
        video->ch = str[i];
        video->attr = color;
    }
}