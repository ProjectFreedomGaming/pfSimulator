// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_MEMORY_H
#define PF_MEMORY_H

#include "pfTypes.h"

// -- Calculates sizes based on a given number of items of a given size
// -- For safety, this returns 0 if the operation overflows
size pfMemoryCalculateSize(size nb_of_items, size item_size);

// -- Allocate memory
void* pfMemoryAlloc(size size);
void* pfMemoryCalloc(size size);
void* pfMemoryRealloc(void* mem, size size);

// -- Free alocated memory
void* pfMemoryFree(void* mem);

// -- Copy should only be used when source and dest do not overlap
void* pfMemoryCopy(const void* src, void* dest, size size);

// -- Move can be used when source and dest overlap
void* pfMemoryMove(const void* src, void* dest, size size);

#endif
