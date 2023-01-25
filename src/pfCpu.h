// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_CPU_H
#define PF_CPU_H

#include "pfMmu.h"

// -- Forward Declarations
typedef struct PFCpu PFCpu;

// -- Create a new CPU
PFCpu* pfCpuNew(PFMmu* mmu);

// -- Destroy an existing CPU
void pfCpuDelete(PFCpu* this);

#endif
