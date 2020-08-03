#ifndef _PWNLIB_
#define _PWNLIB_

enum LOG_LEVEL {SILENT, INFO, ERROR};

#if defined(LOGLEVEL_SILENT)
#define LOGLEVEL SILENT
#elif defined(LOGLEVEL_ERROR)
#define LOGLEVEL ERROR
#else
#define LOGLEVEL INFO
#endif

struct PwnRemote {
    int socket;
    char* inputBuffer;
    size_t inputBufferLength;
    char** lines;
    size_t lineCount;
};

typedef struct PwnRemote PwnS;

#define ERROR -1
#define BLOCK_SIZE 4096

char *pwnread(struct PwnRemote *pr, int length);
char *pwnreadline(struct PwnRemote *pr);
struct PwnRemote *pwnremote(char* ip, int port);
void pwnsend(struct PwnRemote *pr, char* fmt, ...);
void pwnsendline(struct PwnRemote *pr, char* fmt, ...);

#include "pwnlib.c"
#endif