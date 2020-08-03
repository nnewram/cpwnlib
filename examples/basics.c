#include <stdio.h>
#include <unistd.h>
#define LOGLEVEL_SILENT // no output from the library :)

#include "../pwnlib.h"

void printGet(PwnS* p) { // small little helper function
    char* buf;
    buf = pwnreadline(p); // get one line.
    printf("%s\n", buf);
    free(buf);
}

int main() {
    char *buf;
    
    PwnS* r = pwnremote("127.0.0.1", 1337);

    buf = pwnreaduntil(r, ":"); // read up and until the dilimitre : is reached, the delim is a string.
    printf("got %s\n", buf);

    pwnsendline(r, "lmao cool %d", 69); // send a line with formatting to remote r
    printGet(r);
    printGet(r);
}