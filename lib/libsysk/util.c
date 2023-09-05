#include <sysk/sysk.h>

void syskmemcpy(char *dest, char *src, unsigned int size) {
        for (int i = 0; i<size; i++ ) {
                *dest = *src;
                dest++;
                src++;
        }
}
