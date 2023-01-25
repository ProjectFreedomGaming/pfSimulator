// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfMmu.h"
#include "pfMemory.h"
#include "pfAssert.h"

// -- Types
typedef struct PFReadHandler
{
    MmuReadHandlerFuncPtr function;
    void* argument;
} PFReadHandler;

typedef struct PFWriteHandler
{
    MmuWriteHandlerFuncPtr function;
    void* argument;
} PFWriteHandler;

typedef struct PFMmu
{
    PFReadHandler read_handlers[256];
    PFWriteHandler write_handlers[256];
} PFMmu;

// -- Functions
PFMmu* pfMmuNew(void)
{
    PFMmu* this = pfMemoryCalloc(sizeof(PFMmu));
    PF_ASSERT(this != NULL);
    
    return this;
}

void pfMmuDelete(PFMmu* this)
{
    pfMemoryFree(this);
}

void pfMmuAddReadMemory(PFMmu* this, byte range, MmuReadHandlerFuncPtr function, void* argument)
{
    PFReadHandler* handler = this->read_handlers + range;
    
    // -- Check that we are not adding the same range twice
    PF_ASSERT_DEBUG(handler->function == NULL);

    handler->function = function;
    handler->argument = argument;
}

void pfMmuRemoveReadMemory(PFMmu* this, byte range)
{
    PFReadHandler* handler = this->read_handlers + range;
    
    handler->function = NULL;
    handler->argument = NULL;
}

void pfMmuAddWriteMemory(PFMmu* this, byte range, MmuWriteHandlerFuncPtr function, void* argument)
{
    PFWriteHandler* handler = this->write_handlers + range;
    
    // -- Check that we are not adding the same range twice
    PF_ASSERT_DEBUG(handler->function == NULL);

    handler->function = function;
    handler->argument = argument;
}

void pfMmuRemoveWriteMemory(PFMmu* this, byte range)
{
    PFWriteHandler* handler = this->write_handlers + range;

    handler->function = NULL;
    handler->argument = NULL;
}

uint16 pfMmuRead(PFMmu* this, pointer address)
{
    byte range = (address & 0x00ff0000) >> 16;

    PFReadHandler* handler = this->read_handlers + range;

    // -- Check that we're not accessing an illegal memory range
    PF_ASSERT_DEBUG(handler->function != NULL);

    return handler->function(handler->argument, address & 0xFFFF);
}

void pfMmuWrite(PFMmu* this, pointer address, uint16 value)
{
    byte range = (address & 0x00ff0000) >> 16;

    PFWriteHandler* handler = this->write_handlers + range;

    // -- Check that we're not accessing an illegal memory range
    PF_ASSERT_DEBUG(handler->function != NULL);

    return handler->function(handler->argument, address & 0xFFFF, value);
}
