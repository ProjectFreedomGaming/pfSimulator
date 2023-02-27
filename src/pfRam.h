// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_PFRAM_H
#define PF_PFRAM_H

#include "pfTypes.h"

// -- Forward Declarations
typedef struct PFRam PFRam;

// -- Create and destroy RAM devices
PFRam* pfRamNew(void);
void pfRamDelete(PFRam* this);

// -- Create a new RAM
bool pfRamLoadPfxRom(PFRam* this, const char* rom_file_path);

// -- Read and write to RAM
byte pfRamReadByte(PFRam* this, pointer address);
word pfRamReadWord(PFRam* this, pointer address);
longword pfRamReadLong(PFRam* this, pointer address);
void pfRamWriteByte(PFRam* this, pointer address, byte value);
void pfRamWriteWord(PFRam* this, pointer address, word value);
void pfRamWriteLong(PFRam* this, pointer address, longword value);

// -- Utility functions
size pfRamGetRomSize(PFRam* this);
size pfRamGetTotalSize(PFRam* this);

#endif
