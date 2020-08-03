#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

// cyclic is a retarded shit function that is completely fucking worthless and bloated to hell, just show hex instead
char* pwncyclic(int length, int width) {
    char* pattern = malloc(length + 2);
    char buf[16];
    int i;
    int blk_cntr = 0;
    char* fmt = width == 4 ? "|%03X" : "|%07X";

    for (i = 0; i < length; i += width, blk_cntr++) {
        sprintf(buf, fmt, blk_cntr);
        strcat(pattern, buf);
    }

    return pattern;
}

int pwncyclicfind(char* marker) {
    char *buf;

    if (*marker == '|') {
        marker++;
        buf = marker;
        while (*++buf != '0')
        *buf = '\0';
    }

    return strtol(marker, NULL, 16);
}
