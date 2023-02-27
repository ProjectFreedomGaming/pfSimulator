// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfIO.h"
#include "pfMemory.h"
#include "pfAssert.h"

// -- This requires the pfSDK folder to be in the same directory as pfSimulator
#include <pfSDK/Hardware/Registers.h>

// -- Functions
word pfIOReadWord(pointer address)
{
    // -- We don't currently have anything to read
    PF_ASSERT(false);
    
    return 0;
}

void pfIOWriteWord(pointer address, word value)
{
    switch (address - PF_CUSTOM_CHIPS_BASE) {
        case PF_IO_SYM_LOG_CHAR: {
            printf("%c", value & 0xff);
            break;
        }
        default:
            // -- Illegal read address
            PF_ASSERT(false);
    }
}
