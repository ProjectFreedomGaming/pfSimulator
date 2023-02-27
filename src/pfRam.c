// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfRam.h"
#include "pfMemory.h"
#include "pfAssert.h"

// -- This requires the pfSDK folder to be in the same directory as pfSimulator
#include <pfSDK/Hardware/Registers.h>
#include <pfSDK/Hardware/Memory.h>

// -- Types
typedef struct PFRam
{
    void* data;

    size total_size;
    size rom_file_size;

} PFRam;

// -- Functions
PFRam* pfRamNew(void)
{
    PFRam* this = pfMemoryCalloc(sizeof(PFRam));
    PF_ASSERT(this != NULL);

    // -- RAM is only the memory before the custom chip registers
    this->total_size = PF_CUSTOM_CHIPS_BASE;
    
    this->data = pfMemoryCalloc(this->total_size);
    PF_ASSERT(this->data != NULL);

    return this;
}

void pfRamDelete(PFRam* this)
{
    if (this->data != NULL) {
        pfMemoryFree(this->data);
        this->data = NULL;
    }

    pfMemoryFree(this);
}

bool pfRamLoadPfxRom(PFRam* this, const char* rom_file_path)
{
    FILE* file_pointer = fopen(rom_file_path,"rb");
    if (file_pointer == NULL) {
        return false;
    }
    
    fseek(file_pointer, 0, SEEK_END);
    this->rom_file_size = ftell(file_pointer);
    fseek(file_pointer, 0, SEEK_SET);
    
    PF_ASSERT(this->rom_file_size <= this->total_size);
    
    fread(this->data, 1, this->rom_file_size, file_pointer);
    fclose(file_pointer);
    
    return true;
}

byte pfRamReadByte(PFRam* this, pointer address)
{
    PF_ASSERT(address < this->total_size);
        
    return *(byte*)(this->data + address);
}

word pfRamReadWord(PFRam* this, pointer address)
{
    PF_ASSERT(address < (this->total_size - 1));
    
    byte* data = (byte*)(this->data + address);
    return (data[0]<<8) | data[1];
}

longword pfRamReadLong(PFRam* this, pointer address)
{
    PF_ASSERT(address < (this->total_size - 3));

    byte* data = (byte*)(this->data + address);
    return (data[0]<<24) | (data[1]<<16) | (data[2]<<8) | data[3];
}

void pfRamWriteByte(PFRam* this, pointer address, byte value)
{
    PF_ASSERT((address >= this->rom_file_size) && (address < this->total_size));

    *(byte*)(this->data + address) = value;
}

void pfRamWriteWord(PFRam* this, pointer address, word value)
{
    PF_ASSERT((address >= this->rom_file_size) && (address < (this->total_size - 1)));
    
    byte* data = (byte*)(this->data + address);
    data[1] = value & 0xff;
    data[0] = value >> 8;
}

void pfRamWriteLong(PFRam* this, pointer address, longword value)
{
    PF_ASSERT((address >= this->rom_file_size) && (address < (this->total_size - 3)));

    byte* data = (byte*)(this->data + address);
    data[3] = value & 0xff;
    value >>= 8;
    data[2] = value & 0xff;
    value >>= 8;
    data[1] = value & 0xff;
    data[0] = value >> 8;
}

size pfRamGetRomSize(PFRam* this)
{
    return this->rom_file_size;
}

size pfRamGetTotalSize(PFRam* this)
{
    return this->total_size;
}
