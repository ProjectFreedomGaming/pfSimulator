// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_PFX1_H
#define PF_PFX1_H

#include "pfMmu.h"
#include "pfTypes.h"

// -- Forward Declarations
typedef struct PFSimulator PFSimulator;
typedef struct PFPfx1 PFPfx1;

// -- Create a new pfx-1
PFPfx1* pfPfx1New(PFSimulator* simulator, PFMmu* mmu);

// -- Destroy an existing pfx-1
void pfPfx1Delete(PFPfx1* this);

#endif
