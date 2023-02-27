// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfPfx1.h"
#include "pfPfx1Registers.h"
#include "pfSimulator.h"
#include "pfRegister.h"
#include "pfMemory.h"
#include "pfAssert.h"

// -- Types
typedef struct PFPfx1
{
    PFSimulator* simulator;

    PFMmu* mmu;
    
    PFRegister* vsync_count;
    PFRegister* control;
    PFRegister* color_rg;
    PFRegister* color_ba;
} PFPfx1;

// -- Functions
uint16 _read(void* argument, pointer address)
{
    PF_ASSERT_DEBUG(argument != NULL);
    
    PFPfx1* this = (PFPfx1*)argument;
    
    switch (address & 0xFFFF) {
        case PF_PFX1_VSYNC_COUNT: {
            // -- We downcast to uint16 here but for the sake of comparing frame numbers, that's ok
            return (uint16)pfSimulatorGetVSyncCount();
        }
        case PF_PFX1_COLOR_RG: {
            return pfRegisterRead(this->color_rg);
        }
        case PF_PFX1_COLOR_BA: {
            return pfRegisterRead(this->color_ba);
        }
        default:
            // -- Illegal read address
            PF_ASSERT(false);
    }
    
    return 0;
}

void _write(void* argument, pointer address, uint16 value)
{
    PF_ASSERT_DEBUG(argument != NULL);
    
    PFPfx1* this = (PFPfx1*)argument;
    
    switch (address & 0xFFFF) {
        case PF_PFX1_CONTROL: {
            uint16 result = pfRegisterWrite(this->control, value);
            
            if (result & PF_PFX1_CONTROL_CLEAR_SCREEN) {
                pfRegisterClearBits(this->control, PF_PFX1_CONTROL_CLEAR_SCREEN);
                
                uint16 rg = pfRegisterRead(this->color_rg);
                uint16 ba = pfRegisterRead(this->color_ba);
                pfSimulatorClearDisplay(this->simulator, rg >> 8, rg & 0xFF, ba >> 8);
            }
            
            if (result & PF_PFX1_CONTROL_SWAP_BUFFER) {
                pfRegisterClearBits(this->control, PF_PFX1_CONTROL_SWAP_BUFFER);
                
                pfSimulatorSwapDisplayBuffer(this->simulator);
            }
            break;
        }
        case PF_PFX1_COLOR_RG: {
            pfRegisterWrite(this->color_rg, value);
            break;
        }
        case PF_PFX1_COLOR_BA: {
            pfRegisterWrite(this->color_ba, value);
            break;
        }
        default:
            // -- Illegal read address
            PF_ASSERT(false);
    }
}

PFPfx1* pfPfx1New(PFSimulator* simulator, PFMmu* mmu)
{
    PF_ASSERT_DEBUG(simulator != NULL);
    PF_ASSERT_DEBUG(mmu != NULL);
    
    PFPfx1* this = pfMemoryCalloc(sizeof(PFPfx1));
    PF_ASSERT(this != NULL);
    
    this->simulator = simulator;
    this->mmu = mmu;

    this->vsync_count = pfRegisterNew(PF_REGISTER_MASK_ALL, PF_REGISTER_MASK_NONE);
    this->control = pfRegisterNew(PF_REGISTER_MASK_NONE, 0x0003);
    this->color_rg = pfRegisterNew(PF_REGISTER_MASK_ALL, PF_REGISTER_MASK_ALL);
    this->color_ba = pfRegisterNew(PF_REGISTER_MASK_ALL, PF_REGISTER_MASK_ALL);

    pfMmuAddReadMemory(this->mmu, PF_PFX1_BASE >> 16, _read, this);
    pfMmuAddWriteMemory(this->mmu, PF_PFX1_BASE >> 16, _write, this);

    return this;
}

void pfPfx1Delete(PFPfx1* this)
{
    pfMmuRemoveReadMemory(this->mmu, PF_PFX1_BASE >> 16);
    pfMmuRemoveWriteMemory(this->mmu, PF_PFX1_BASE >> 16);

    pfRegisterDelete(this->vsync_count);
    this->vsync_count = NULL;

    pfRegisterDelete(this->control);
    this->control = NULL;

    pfRegisterDelete(this->color_rg);
    this->color_rg = NULL;

    pfRegisterDelete(this->color_ba);
    this->color_ba = NULL;

    this->simulator = NULL;
    this->mmu = NULL;

    pfMemoryFree(this);
}
