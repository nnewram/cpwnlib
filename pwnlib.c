#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdarg.h>

#define logInfo(fmt, ...) \
    do { \
        if (LOGLEVEL == INFO) {\
            fprintf(stderr, fmt "\n", __VA_ARGS__); \
        } \
    } while(0)

#define logError(fmt, ...) \
    do { \
        if (LOGLEVEL == ERROR || LOGLEVEL == INFO) { \
            fprintf(stderr, "Error in file %s, line %d, function %s: " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        } \
    } while(0)

char *pwnread(struct PwnRemote *pr, int length) {
    size_t totalRead = 0;
    char *buffer = malloc(length);
    read(pr->socket, buffer, length);
    return buffer;
}

char *pwnreadline(struct PwnRemote *pr) {
    char *leftover;
    char *line;
    char *lastString;
    char *buf = pr->inputBuffer;
    char *tmp;

    _Bool hasMoreLines = 1;
    _Bool firstLine = 1;
    int nread;
    
    if (pr->lineCount > 0) {
        pr->lineCount--;
        char *buf = strdup(pr->lines[pr->lineCount]);
        bzero(pr->lines[pr->lineCount], strlen(pr->lines[pr->lineCount]));
        free(pr->lines[pr->lineCount]);
        return buf;
    }

    nread = read(pr->socket, pr->inputBuffer, BLOCK_SIZE-1);
    pr->inputBuffer[nread] = '\0';
    
    while (hasMoreLines) {
        if (strchr(buf, '\n') != NULL) {
            
            lastString = strrchr(buf, '\n');
            *lastString = '\0';
            lastString++;

            line = strtok(buf, "\n");
            while (line != NULL) {
                pr->lines = realloc(pr->lines, (pr->lineCount+1) * sizeof *(pr->lines));
                pr->lines[pr->lineCount++] = strdup(line);
                line = strtok(NULL, "\n");
            }
        }
        
        else {
            leftover = malloc(BLOCK_SIZE);
            tmp = leftover;
            
            if (lastString) {
                strcpy(leftover, lastString);
                tmp = leftover + strlen(lastString);
            }
            
            nread = recv(pr->socket, tmp, BLOCK_SIZE-1, MSG_DONTWAIT);
            
            if (*leftover == '\0' || nread == -1) {
                hasMoreLines = 0;
                return strdup(pr->inputBuffer);
            }
            
            else {
                leftover[nread] = '\0';
                pr->inputBuffer = realloc(pr->inputBuffer, BLOCK_SIZE + strlen(leftover));
                strcat(pr->inputBuffer, leftover);
                free(leftover);
            }
        }
    }

    return pwnreadline(pr);
}

char *pwnreaduntil(struct PwnRemote *pr, char* delim) {
    char *buf = malloc(BLOCK_SIZE);
    size_t delim_length = strlen(delim);
    int i;

    if (delim_length == 0) {
        logError("Delim has length 0.");
    }

    for (i = 0; i < BLOCK_SIZE; i++) {
        read(pr->socket, buf + i, 1);
        if (i >= delim_length-1) {
            if (strcmp(buf + i - delim_length + 1, delim) == 0) {
                buf = realloc(buf, i+2);
                return buf;    
            }
        }
    }

    return buf;
}

void pwnsend(struct PwnRemote *pr, char* fmt, ...) {
    char *buf;
    buf = malloc(BLOCK_SIZE);
    va_list arglist;
    va_start(arglist, fmt);
    vsnprintf(buf, BLOCK_SIZE, fmt, arglist);
    send(pr->socket, buf, strlen(buf), MSG_CONFIRM);
    va_end(arglist);
    free(buf);
}

void pwnsendline(struct PwnRemote *pr, char* fmt, ...) {
    char *newfmt;
    char *buf;
    int fmtlen = strlen(fmt);
    va_list arglist;
    va_start(arglist, fmt);
    
    newfmt = malloc(fmtlen + 2);
    strcpy(newfmt, fmt);
    
    newfmt[fmtlen] = '\n';
    newfmt[fmtlen + 1] = '\0';

    buf = malloc(BLOCK_SIZE);

    vsnprintf(buf, BLOCK_SIZE, newfmt, arglist);
    send(pr->socket, buf, strlen(buf), MSG_CONFIRM);
    
    va_end(arglist);

    free(newfmt);
    free(buf);
}

struct PwnRemote *pwnremote(char* ip, int port) {
    int clientSocket;
    struct sockaddr_in serverAddr;

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        logError("Could not create socket");
    }

    serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	
    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0) {
        logError("Invalid IP address: %s", ip);
        return ERROR;
    }
    
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof serverAddr) >= 0) {
        logInfo("Accepted connection to ip %s with port %d", ip, port);
    }

    else {
        logError("Could not accept connection to ip %s with port %d", ip, port);
        return ERROR;
    }

    struct PwnRemote *pwnremote = calloc(1, sizeof *pwnremote);
    pwnremote->socket = clientSocket;
    pwnremote->inputBuffer = malloc(BLOCK_SIZE);
    return pwnremote;
}