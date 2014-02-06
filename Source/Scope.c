///////////////////////////////////////////////////////////////////////////////
// This source file is part of Nominal.
//
// Copyright (c) 2014 Colin Hill
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include "Scope.h"
#include "HashTable.h"

#include <stdlib.h>

Hash HashStringId(UserData key)
{
    return (Hash)key;
}

int CompareStringId(UserData left, UserData right)
{
    if (left == right)
    {
        return 0;
    }
    else
    {
        return left < right ? -1 : 1;
    }
}

typedef struct _ScopeNode
{
    HashTable* hashTable;
    struct _ScopeNode* next;
} ScopeNode;

typedef struct _Scope
{
    ScopeNode* top;
} Scope;

ScopeNode* ScopeNode_Create()
{
    ScopeNode* n = (ScopeNode*)malloc(sizeof(ScopeNode));
    n->hashTable = HashTable_Create(HashStringId, CompareStringId, 10);
    n->next = NULL;
    return n;
}

void ScopeNode_Free(ScopeNode* n)
{
    HashTable_Free(n->hashTable, NULL, NULL);
    free(n);
}

Scope* Scope_Create()
{
    Scope* s = (Scope*)malloc(sizeof(Scope));
    s->top = ScopeNode_Create();
    return s;
}

void Scope_Free(Scope* s)
{
    ScopeNode* n = s->top;
    while (n)
    {
        ScopeNode* t = n->next;
        ScopeNode_Free(n);
        n = t;
    }
    free(s);
}

int Scope_Let(Scope* s, StringId id, NomValue value)
{
    return HashTable_Insert(s->top->hashTable, (UserData)id, (UserData)value.raw);
}

int Scope_Set(Scope* s, StringId id, NomValue value)
{
    return HashTable_Set(s->top->hashTable, (UserData)id, (UserData)value.raw);
}

NomValue Scope_Get(Scope* s, StringId id)
{
    NomValue    value = { 0 };
    ScopeNode*  n = s->top;
    while (n)
    {
        if (HashTable_Find(n->hashTable, (UserData)id, (UserData*)&value.raw))
        {
            return value;
        }
        n = n->next;
    }

    return NOM_NIL;
}

void Scope_Begin(Scope* s, StringId id)
{
    ScopeNode* n = ScopeNode_Create();
    n->next = s->top;
    s->top = n;
}

void Scope_End(Scope* s, StringId id)
{
    ScopeNode* n = s->top;
    s->top = n->next;
    ScopeNode_Free(n);
}