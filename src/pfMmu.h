// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_MMU_H
#define PF_MMU_H

#include "pfTypes.h"

// -- Forward Declarations
typedef struct PFMmu PFMmu;

// -- Handler functions
typedef uint16 (*MmuReadHandlerFuncPtr)(void*, pointer);
typedef void (*MmuWriteHandlerFuncPtr)(void*, pointer, uint16);

// -- Create a new MMU
PFMmu* pfMmuNew(void);

// -- Destroy an existing MMU
void pfMmuDelete(PFMmu* this);

// -- Add/Remove read and write memory at a given addresses range
void pfMmuAddReadMemory(PFMmu* this, byte range, MmuReadHandlerFuncPtr function, void* argument);
void pfMmuRemoveReadMemory(PFMmu* this, byte range);

void pfMmuAddWriteMemory(PFMmu* this, byte range, MmuWriteHandlerFuncPtr function, void* argument);
void pfMmuRemoveWriteMemory(PFMmu* this, byte range);

// -- Read and write memory at a given addresses
uint16 pfMmuRead(PFMmu* this, pointer address);
void pfMmuWrite(PFMmu* this, pointer address, uint16 value);

#endif
