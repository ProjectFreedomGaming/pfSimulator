// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_FLIP_H
#define PF_FLIP_H

#include "pfTypes.h"

// -- Forward Declarations
typedef struct PFSimulator PFSimulator;
typedef struct PFFlip PFFlip;

// -- Create and destroy pfFlip chip
PFFlip* pfFlipNew(PFSimulator* simulator);
void pfFlipDelete(PFFlip* this);

// -- Read and write registers
word pfFlipReadWord(PFFlip* this, pointer address);
void pfFlipWriteWord(PFFlip* this, pointer address, word value);

#endif
