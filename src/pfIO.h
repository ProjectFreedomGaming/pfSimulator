// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_PFIO_H
#define PF_PFIO_H

#include "pfTypes.h"

// -- Read and write to IO
word pfIOReadWord(pointer address);
void pfIOWriteWord(pointer address, word value);

#endif
