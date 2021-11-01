#include <pipes.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <semaphore.h>

#define MAX_PIPES 10
#define SEM_ID_SIZE 3 // Should be able to contain itoa(MAX_PIPES*2)
#define TOTAL_FDS   (MAX_PIPES * 2 + 3)
#define MAX_BUFFER 256

typedef struct Pipe {
    unsigned int fdIn;
    unsigned int fdOut;
    char buffer[MAX_BUFFER];
    unsigned int readPos;
    unsigned int writePos;
    char writeSem[SEM_ID_SIZE], readSem[SEM_ID_SIZE];
} Pipe;

static Pipe * pipes[MAX_PIPES];
static int FDToPipe[TOTAL_FDS];

int initPipes(void) {
    for (uint8_t i = 0; i < 3; i++)
        FDToPipe[i] = MAX_PIPES;
    for (uint8_t i = 3; i < TOTAL_FDS; i++)
        FDToPipe[i] = -1;
    return 0;
}

static int isValidFD(int fd) {
    return fd >= 0 && fd < TOTAL_FDS;
}

static int getFreePipeIndex(void) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i] == NULL)
            return i;
    }
    return -1;
}

static int getNextFd() {
    for (int i = 0; i < TOTAL_FDS; i++) {
        if (FDToPipe[i] == -1)
            return i;
    }
    return -1;
}

// Writes the string value of i into buffer, backwards: i(10) -> buffer("01")
static void itoa(char * buffer, int i) {
    buffer[0] = '0';
    while(i > 0) {
        buffer[0] = '0' + i%10;
        i /= 10;
        buffer++;
    }
    buffer[0] = '\0';
}

int openPipe(int fd[2]) {
    int pipeIndex = getFreePipeIndex();
    if (pipeIndex == -1)
        return -1;
    
    pipes[pipeIndex] = alloc(sizeof(Pipe));

    int fd1 = getNextFd();
    if (fd1 == -1) {
        free(pipes[pipeIndex]);
        return -1;
    }
    pipes[pipeIndex]->fdIn = fd1;
    FDToPipe[fd1] = pipeIndex;

    int fd2 = getNextFd();
    if (fd2 == -1) {
        free(pipes[pipeIndex]);
        FDToPipe[fd1] = -1;
        return -1;
    }

    pipes[pipeIndex]->fdOut = fd2;
    FDToPipe[fd2] = pipeIndex;

    pipes[pipeIndex]->readPos = 0;
    pipes[pipeIndex]->writePos = 0;

    itoa(pipes[pipeIndex]->readSem, pipes[pipeIndex]->fdIn);
    itoa(pipes[pipeIndex]->writeSem, pipes[pipeIndex]->fdOut);
    int ans = openSemaphore(pipes[pipeIndex]->readSem, 0);
    if (ans != SEM_SUCCESS) {
        free(pipes[pipeIndex]);
        FDToPipe[fd1] = -1;
        FDToPipe[fd2] = -1;
        return -1;
    }
    ans = openSemaphore(pipes[pipeIndex]->writeSem, MAX_BUFFER);
    if (ans != SEM_SUCCESS) {
        closeSemaphore(pipes[pipeIndex]->readSem);
        free(pipes[pipeIndex]);
        FDToPipe[fd1] = -1;
        FDToPipe[fd2] = -1;
        return -1;
    }
    
    fd[0] = pipes[pipeIndex]->fdIn;
    fd[1] = pipes[pipeIndex]->fdOut;
    return 0;
}

int writePipe(int fd, char * buffer, int count) {
    if (!isValidFD(fd) || count < 0 || buffer == NULL)
        return -1;
    int pipeIndex = FDToPipe[fd];
    if (pipeIndex == -1 || pipes[pipeIndex]->fdOut != fd)
        return -1;
        
    
    for (int i = 0; i < count; i++) {
        if (waitSemaphore(pipes[pipeIndex]->writeSem) != SEM_SUCCESS)
            return i;
        pipes[pipeIndex]->buffer[pipes[pipeIndex]->writePos++] = buffer[i];
        pipes[pipeIndex]->writePos %= MAX_BUFFER;
        if (postSemaphore(pipes[pipeIndex]->readSem) != SEM_SUCCESS)
            return i;
    }
    return count;
}

// fd[0]-->[  ]-->fd[1]

int readPipe(int fd, char * buffer, int count) {
    if (!isValidFD(fd) || count < 0 || buffer == NULL)
        return -1;
    int pipeIndex = FDToPipe[fd];
    if (pipeIndex == -1 || pipes[pipeIndex]->fdIn != fd)
        return -1;

    for (int i = 0; i < count; i++) {
        if (waitSemaphore(pipes[pipeIndex]->readSem) != SEM_SUCCESS)
            return i;
        buffer[i] = pipes[pipeIndex]->buffer[pipes[pipeIndex]->readPos++];
        pipes[pipeIndex]->readPos %= MAX_BUFFER;
        if (postSemaphore(pipes[pipeIndex]->writeSem) != SEM_SUCCESS)
            return i;
    }
    return count;
}

int closePipe(int fd) {
    int index = FDToPipe[fd];
    if (index == -1)
        return -1;
    
    if (pipes[index]->fdIn == fd) {
        pipes[index]->fdIn = -1;
        FDToPipe[pipes[index]->fdIn] = -1;
    } else {
        pipes[index]->fdOut = -1;
        FDToPipe[pipes[index]->fdOut] = -1;
    }
    
    if (pipes[index]->fdIn == -1 && pipes[index]->fdIn == -1) {
        free(pipes[index]);
    }
    return 0;
}
