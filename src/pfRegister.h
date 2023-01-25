// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_REGISTER_H
#define PF_REGISTER_H

#include "pfTypes.h"

// -- Macros
#define PF_REGISTER_MASK_ALL     0xFFFF
#define PF_REGISTER_MASK_NONE    0x0000

// -- Forward Declarations
typedef struct PFRegister PFRegister;

// -- Create a new register
PFRegister* pfRegisterNew(uint16 read_mask, uint16 write_mask);

// -- Destroy an existing register
void pfRegisterDelete(PFRegister* this);

// -- Read and write to the register
uint16 pfRegisterRead(PFRegister* this);
uint16 pfRegisterWrite(PFRegister* this, uint16 value);

uint16 pfRegisterSetBits(PFRegister* this, uint16 value);
uint16 pfRegisterClearBits(PFRegister* this, uint16 value);

#endif
