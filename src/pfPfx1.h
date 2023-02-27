// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_PFX1_H
#define PF_PFX1_H

#include "pfTypes.h"

// -- Forward Declarations
typedef struct PFSimulator PFSimulator;
typedef struct PFPfx1 PFPfx1;

// -- Create and destroy pfx-1 chip
PFPfx1* pfPfx1New(PFSimulator* simulator);
void pfPfx1Delete(PFPfx1* this);

// -- Read and write registers
word pfPfx1ReadWord(PFPfx1* this, pointer address);
void pfPfx1WriteWord(PFPfx1* this, pointer address, word value);

#endif
