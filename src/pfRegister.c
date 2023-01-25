// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfRegister.h"
#include "pfMemory.h"
#include "pfAssert.h"

// -- Types
typedef struct PFRegister
{
    uint16 value;

    uint16 read_mask;

    uint16 write_mask;
} PFRegister;

// -- Functions
PFRegister* pfRegisterNew(uint16 read_mask, uint16 write_mask)
{
    PFRegister* this = pfMemoryCalloc(sizeof(PFRegister));
    PF_ASSERT(this != NULL);

    this->read_mask = read_mask;
    this->write_mask = write_mask;

    return this;
}

void pfRegisterDelete(PFRegister* this)
{
    pfMemoryFree(this);
}

uint16 pfRegisterRead(PFRegister* this)
{
    return this->value & this->read_mask;
}

uint16 pfRegisterWrite(PFRegister* this, uint16 value)
{
    return this->value = (this->value & ~this->write_mask) | (value & this->write_mask);
}

uint16 pfRegisterSetBits(PFRegister* this, uint16 value)
{
    return this->value = this->value | (value & this->write_mask);
}

uint16 pfRegisterClearBits(PFRegister* this, uint16 value)
{
    return this->value = this->value & ~(value & this->write_mask);
}
