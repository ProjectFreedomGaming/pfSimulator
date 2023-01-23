// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfMemory.h"

#include <memory.h>

// -- Functions
size pfMemoryCalculateSize(size nb_of_items, size item_size)
{
    if (item_size && (nb_of_items > (SIZE_MAX / item_size))) {
        return 0;
    }
    
    return nb_of_items * item_size;
}

void* pfMemoryAlloc(size size)
{
    if (size == 0) {
        return NULL;
    }
    
    return realloc(NULL, size);
}

void* pfMemoryCalloc(size size)
{
    if (size == 0) {
        return NULL;
    }
    
    void* memory = realloc(NULL, size);
    if(memory != NULL) {        
        memset(memory, 0, size);
    }
    
    return memory;
}

void* pfMemoryRealloc(void* mem, size size)
{
    if (size == 0) {
        return NULL;
    }

    return realloc(mem, size);
}

void* pfMemoryFree(void* mem)
{
    return realloc(mem, 0);
}

void* pfMemoryCopy(const void* src, void* dest, size size)
{
    if (size == 0) {
        return NULL;
    }

    return memcpy(dest, src, size);
}

void* pfMemoryMove(const void* src, void* dest, size size)
{
    if (size == 0) {
        return NULL;
    }

    return memmove(dest, src, size);
}
