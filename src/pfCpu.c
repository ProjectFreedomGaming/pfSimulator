// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfCpu.h"
#include "pfMemory.h"
#include "pfTypes.h"
#include "pfAssert.h"
#include "pfRam.h"
#include "pfFlip.h"
#include "pfIO.h"

// -- This requires the pfSDK folder to be in the same directory as pfSimulator
#include <pfSDK/Hardware/Registers.h>
#include <pfSDK/Hardware/Memory.h>

#include <SDL2/SDL.h>

#include "Musashi/m68k.h"
#include "Musashi/m68kcpu.h"

// -- Types
typedef struct PFCpu
{
    SDL_Thread* thread;
    bool thread_should_exit;
    
    PFRam* ram;
    PFFlip* flip;
} PFCpu;

// -- Single instance of our CPU
static PFCpu* _singleton = NULL;

// -- Macros
#define _MEMORY_ACCESS_LOG_ENABLED      0
#if _MEMORY_ACCESS_LOG_ENABLED
    #define _MEMORY_ACCESS_LOG(format, ...)         printf((format), ##__VA_ARGS__); m68k_disassemble_log()
#else
    #define _MEMORY_ACCESS_LOG(format, args...)     do { } while(0)
#endif

// -- Functions
unsigned int m68k_read_memory_8(unsigned int address)
{
    _MEMORY_ACCESS_LOG("Read8  at 0x%08x ", address);
    PF_ASSERT(_singleton != NULL);

    // -- Custom chip access is only done via word operations
    PF_ASSERT(address < PF_CUSTOM_CHIPS_BASE);
        
    return pfRamReadByte(_singleton->ram, address);
}

unsigned int m68k_read_memory_16(unsigned int address)
{
    _MEMORY_ACCESS_LOG("Read16 at 0x%08x ", address);
    PF_ASSERT(_singleton != NULL);

    if (address >= (PF_CUSTOM_CHIPS_BASE + PF_FLIP_BASE)) {
        return pfFlipReadWord(_singleton->flip, address);
    }
    else if (address >= (PF_CUSTOM_CHIPS_BASE + PF_IO_BASE)) {
        return pfIOReadWord(address);
    }
    else {
        return pfRamReadWord(_singleton->ram, address);
    }
}

unsigned int m68k_read_memory_32(unsigned int address)
{
    _MEMORY_ACCESS_LOG("Read32 at 0x%08x ", address);
    PF_ASSERT(_singleton != NULL);

    // -- Custom chip access is only done via word operations
    PF_ASSERT(address < (PF_CUSTOM_CHIPS_BASE - 3));
    
    return pfRamReadLong(_singleton->ram, address);
}

void m68k_write_memory_8(unsigned int address, unsigned int value)
{
    _MEMORY_ACCESS_LOG("Write8  at 0x%08x (%d) ", address, value);
    PF_ASSERT(_singleton != NULL);

    // -- Custom chip register access is only done via word operations
    PF_ASSERT(address < PF_CUSTOM_CHIPS_BASE);

    pfRamWriteByte(_singleton->ram, address, value);
}

void m68k_write_memory_16(unsigned int address, unsigned int value)
{
    _MEMORY_ACCESS_LOG("Write16 at 0x%08x (%d) ", address, value);
    PF_ASSERT(_singleton != NULL);

    if (address >= (PF_CUSTOM_CHIPS_BASE + PF_FLIP_BASE)) {
        pfFlipWriteWord(_singleton->flip, address, value);
    }
    else if (address >= (PF_CUSTOM_CHIPS_BASE + PF_IO_BASE)) {
        pfIOWriteWord(address, value);
    }
    else {
        pfRamWriteWord(_singleton->ram, address, value);
    }
}

void m68k_write_memory_32(unsigned int address, unsigned int value)
{
    _MEMORY_ACCESS_LOG("Write32 at 0x%08x (%d) ", address, value);
    PF_ASSERT(_singleton != NULL);
    
    // -- Custom chip register access is only done via word operations
    PF_ASSERT(address < (PF_CUSTOM_CHIPS_BASE - 3));

    pfRamWriteLong(_singleton->ram, address, value);
}

unsigned int m68k_read_disassembler_8(unsigned int address)
{
    PF_ASSERT(_singleton != NULL);
    PF_ASSERT(address < pfRamGetRomSize(_singleton->ram));
    
    return pfRamReadByte(_singleton->ram, address);
}

unsigned int m68k_read_disassembler_16(unsigned int address)
{
    PF_ASSERT(_singleton != NULL);
    PF_ASSERT(address < (pfRamGetRomSize(_singleton->ram) - 1));

    return pfRamReadWord(_singleton->ram, address);
}

unsigned int m68k_read_disassembler_32(unsigned int address)
{
    PF_ASSERT(_singleton != NULL);
    PF_ASSERT(address < (pfRamGetRomSize(_singleton->ram) - 3));

    return pfRamReadLong(_singleton->ram, address);
}

static int _threadFunction(void* data)
{
    PFCpu* this = (PFCpu*)data;
    
    while (!this->thread_should_exit) {
        // Values to execute determine the interleave rate.
        // Smaller values allow for more accurate interleaving with multiple
        // devices/CPUs but is more processor intensive.
        // 100000 is usually a good value to start at, then work from there.
        m68k_execute(1000);

        SDL_Delay(1);
    }
    
    return 0;
}

PFCpu* pfCpuNew(PFRam* ram, PFFlip* flip)
{
    PF_ASSERT_DEBUG(_singleton == NULL);
    PF_ASSERT_DEBUG(ram != NULL);
    PF_ASSERT_DEBUG(flip != NULL);

    PFCpu* this = pfMemoryCalloc(sizeof(PFCpu));
    PF_ASSERT(this != NULL);

    _singleton = this;
    
    this->ram = ram;
    this->flip = flip;

    m68k_init();
    m68k_set_cpu_type(M68K_CPU_TYPE_68000);
    m68k_pulse_reset();

    this->thread = SDL_CreateThread(_threadFunction, "pfCpu", this);
    PF_ASSERT(this->thread != NULL);
    
    return this;
}

void pfCpuDelete(PFCpu* this)
{
    if (this->thread != NULL) {
        this->thread_should_exit = true;
        
        SDL_WaitThread(this->thread, NULL);

        this->thread = NULL;
    }

    this->ram = NULL;
    this->flip = NULL;

    pfMemoryFree(this);
}
