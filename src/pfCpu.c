// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfCpu.h"
#include "pfPfx1Registers.h"
#include "pfMemory.h"
#include "pfTypes.h"
#include "pfAssert.h"

#include <SDL2/SDL.h>

// -- Types
typedef struct PFCpu
{
    SDL_Thread* thread;
    bool thread_should_exit;
    
    PFMmu* mmu;
} PFCpu;

// -- Functions
static int _threadFunction(void* data)
{
    PFCpu* this = (PFCpu*)data;
    
    byte r = 0;
    byte g = 0;
    byte b = 0;

    while (!this->thread_should_exit) {
        pfMmuWrite(this->mmu, PF_PFX1_BASE + PF_PFX1_COLOR_RG, (r << 8) | g);
        pfMmuWrite(this->mmu, PF_PFX1_BASE + PF_PFX1_COLOR_BA, (b << 8) | 255);
        pfMmuWrite(this->mmu, PF_PFX1_BASE + PF_PFX1_CONTROL, PF_PFX1_CONTROL_CLEAR_SCREEN);

        r += 12;
        g += 4;
        b += 34;

        uint16 previous_vsync = pfMmuRead(this->mmu, PF_PFX1_BASE + PF_PFX1_VSYNC_COUNT);
        while (pfMmuRead(this->mmu, PF_PFX1_BASE + PF_PFX1_VSYNC_COUNT) == previous_vsync) {
            if (this->thread_should_exit) {
                return 0;
            }

            SDL_Delay(1);
        }

        pfMmuWrite(this->mmu, PF_PFX1_BASE + PF_PFX1_CONTROL, PF_PFX1_CONTROL_SWAP_BUFFER);
    }
    
    return 0;
}

PFCpu* pfCpuNew(PFMmu* mmu)
{
    PF_ASSERT_DEBUG(mmu != NULL);

    PFCpu* this = pfMemoryCalloc(sizeof(PFCpu));
    PF_ASSERT(this != NULL);

    this->thread = SDL_CreateThread(_threadFunction, "pfCpu", this);
    PF_ASSERT(this->thread != NULL);
    
    this->mmu = mmu;
    
    return this;
}

void pfCpuDelete(PFCpu* this)
{
    if (this->thread != NULL) {
        this->thread_should_exit = TRUE;
        
        SDL_WaitThread(this->thread, NULL);

        this->thread = NULL;
    }

    this->mmu = NULL;

    pfMemoryFree(this);
}
