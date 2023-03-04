// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfFlip.h"
#include "pfSimulator.h"
#include "pfRegister.h"
#include "pfMemory.h"
#include "pfAssert.h"

// -- This requires the pfSDK folder to be in the same directory as pfSimulator
#include <pfSDK/Hardware/Registers.h>

// -- Types
typedef struct PFFlip
{
    PFSimulator* simulator;

    PFRegister* vsync_count;
    PFRegister* control;
    PFRegister* color_rg;
    PFRegister* color_ba;
} PFFlip;

// -- Functions
PFFlip* pfFlipNew(PFSimulator* simulator)
{
    PF_ASSERT_DEBUG(simulator != NULL);
    
    PFFlip* this = pfMemoryCalloc(sizeof(PFFlip));
    PF_ASSERT(this != NULL);
    
    this->simulator = simulator;

    this->vsync_count = pfRegisterNew(PF_REGISTER_MASK_ALL, PF_REGISTER_MASK_NONE);
    this->control = pfRegisterNew(PF_REGISTER_MASK_NONE, 0x0003);
    this->color_rg = pfRegisterNew(PF_REGISTER_MASK_ALL, PF_REGISTER_MASK_ALL);
    this->color_ba = pfRegisterNew(PF_REGISTER_MASK_ALL, PF_REGISTER_MASK_ALL);

    return this;
}

void pfFlipDelete(PFFlip* this)
{
    pfRegisterDelete(this->vsync_count);
    this->vsync_count = NULL;

    pfRegisterDelete(this->control);
    this->control = NULL;

    pfRegisterDelete(this->color_rg);
    this->color_rg = NULL;

    pfRegisterDelete(this->color_ba);
    this->color_ba = NULL;

    this->simulator = NULL;

    pfMemoryFree(this);
}

word pfFlipReadWord(PFFlip* this, pointer address)
{
    PF_ASSERT_DEBUG(this != NULL);
    
    switch (address - PF_CUSTOM_CHIPS_BASE) {
        case PF_FLIP_VSYNC_COUNT: {
            // -- We downcast to word here but for the sake of comparing frame numbers, that's ok
            return (word)pfSimulatorGetVSyncCount();
        }
        case PF_FLIP_COLOR_RG: {
            return pfRegisterRead(this->color_rg);
        }
        case PF_FLIP_COLOR_BA: {
            return pfRegisterRead(this->color_ba);
        }
        default:
            // -- Illegal read address
            PF_ASSERT(false);
    }
    
    return 0;
}

void pfFlipWriteWord(PFFlip* this, pointer address, word value)
{
    PF_ASSERT_DEBUG(this != NULL);
    
    switch (address - PF_CUSTOM_CHIPS_BASE) {
        case PF_FLIP_CONTROL: {
            uint16 result = pfRegisterWrite(this->control, value);
            
            if (result & PF_FLIP_CONTROL_CLEAR_SCREEN) {
                pfRegisterClearBits(this->control, PF_FLIP_CONTROL_CLEAR_SCREEN);
                
                uint16 rg = pfRegisterRead(this->color_rg);
                uint16 ba = pfRegisterRead(this->color_ba);
                pfSimulatorClearDisplay(this->simulator, rg >> 8, rg & 0xFF, ba >> 8);
            }
            
            if (result & PF_FLIP_CONTROL_SWAP_BUFFER) {
                pfRegisterClearBits(this->control, PF_FLIP_CONTROL_SWAP_BUFFER);
                
                pfSimulatorSwapDisplayBuffer(this->simulator);
            }
            break;
        }
        case PF_FLIP_COLOR_RG: {
            pfRegisterWrite(this->color_rg, value);
            break;
        }
        case PF_FLIP_COLOR_BA: {
            pfRegisterWrite(this->color_ba, value);
            break;
        }
        default:
            // -- Illegal read address
            PF_ASSERT(false);
    }
}
