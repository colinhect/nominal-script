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

Hash HashStringId(
    UserData    key
    )
{
    return (Hash)key;
}

int CompareStringId(
    UserData    left,
    UserData    right
    )
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
    HashTable*          hashTable;
    struct _ScopeNode*  next;
} ScopeNode;

typedef struct _Scope
{
    ScopeNode*  top;
} Scope;

ScopeNode* ScopeNode_Create(
    void
    )
{
    ScopeNode* node = (ScopeNode*)malloc(sizeof(ScopeNode));
    node->hashTable = HashTable_Create(HashStringId, CompareStringId, 10);
    node->next = NULL;
    return node;
}

void ScopeNode_Free(
    ScopeNode* node
    )
{
    HashTable_Free(node->hashTable, NULL, NULL);
    free(node);
}

Scope* Scope_Create(
    void
    )
{
    Scope* scope = (Scope*)malloc(sizeof(Scope));
    scope->top = ScopeNode_Create();
    return scope;
}

void Scope_Free(
    Scope*  scope
    )
{
    ScopeNode* node = scope->top;
    while (node)
    {
        ScopeNode* t = node->next;
        ScopeNode_Free(node);
        node = t;
    }
    free(scope);
}

bool Scope_Let(
    Scope*      scope,
    StringId    id,
    NomValue    value
    )
{
    return HashTable_Insert(scope->top->hashTable, (UserData)id, (UserData)value.raw);
}

bool Scope_Set(
    Scope*      scope,
    StringId    id,
    NomValue    value
    )
{
    return HashTable_Set(scope->top->hashTable, (UserData)id, (UserData)value.raw);
}

NomValue Scope_Get(
    Scope*      scope,
    StringId    id
    )
{
    NomValue value = { 0 };
    ScopeNode* node = scope->top;
    while (node)
    {
        if (HashTable_Find(node->hashTable, (UserData)id, (UserData*)&value.raw))
        {
            return value;
        }
        node = node->next;
    }

    return NOM_NIL;
}

void Scope_Begin(
    Scope*      scope,
    StringId    id
    )
{
    ScopeNode* node = ScopeNode_Create();
    node->next = scope->top;
    scope->top = node;
}

void Scope_End(
    Scope*      scope,
    StringId    id
    )
{
    ScopeNode* node = scope->top;
    scope->top = node->next;
    ScopeNode_Free(node);
}