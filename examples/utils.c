#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LOGLEVEL_SILENT

#include "../pwnlib.h"

int main() {
    int remove, i;
    char* buf = pwncyclic(0x100, 8); // generate a "cyclic" pattern with length 0x100 and width 8

    srand(time(0));
    remove = (rand()%0x100+1);
    for (i = 0; i < remove; i++, buf++); // remove some of it to simulate a bof
    
    printf("Cyclic: %s\n", buf);
    printf("Found cyclic at position: %d\n", pwncyclicfind(buf)); // pwncyclicfind(marker) (does not have to specify width or be fancy) give the position of the leak
}