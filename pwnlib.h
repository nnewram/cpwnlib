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

#define logInfo(fmt, ...) \
    do { \
        if (LOGLEVEL == INFO) { \
            fprintf(stderr, fmt "\n", __VA_ARGS__); \
        } \
    } while(0)

#define logError(fmt, ...) \
    do { \
        if (LOGLEVEL == ERROR || LOGLEVEL == INFO) { \
            fprintf(stderr, "Error in file %s, line %d, function %s: " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        } \
    } while(0)

#define LOGASSERT(statement, fmt, ...) \
    do { \
        if (statement) { \
            logError(fmt, __VA_ARGS__) \
        } \
    } while(0)

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

#include "remote/remote_process.c"

char* pwncyclic(int length, int width);
int pwncyclicfind(char* marker);

//#include "util/util.c"

#endif
