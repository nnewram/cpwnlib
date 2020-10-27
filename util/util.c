#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#define _ASM(N,NE, C) asm(".global "#N"\n"#N":\n\t"C".global "#NE"\n"#NE":\n\t"); extern char N[], NE[];
#define ASM(N, C) _ASM(N,end_##N, C)

char* pwncyclic(size_t length, int width) {
    char* pattern = malloc(length + 2);
    char buf[16];
    int i;
    int blk_cntr = 0;
    char fmt[6] = width == 4 ? "|%03X" : "|%07X";

    LOGASSERT(pattern != 0, "") 

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
        while (*buf && (*++buf != '0'));
        *buf = '\0';
    }

    return strtol(marker, NULL, 16);
}
