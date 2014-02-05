#ifndef STATE_H
#define STATE_H

#include "Heap.h"

typedef struct _NomState
{
    NomValue stack[1024];
    size_t sp;
    unsigned char byteCode[4096];
    size_t ip;
    char error[1024];
    int errorFlag;
    Heap* heap;
} NomState;

void NomState_SetError(NomState* s, const char* fmt, ...);

#endif