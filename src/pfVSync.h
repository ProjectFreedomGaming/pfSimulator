// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_VSYNC_H
#define PF_VSYNC_H

#include "pfTypes.h"

// -- Init and shutdown the VSync subsystem
void pfVsyncInit(void);
void pfVsyncShutdown(void);

// -- Get the current VSync count
size pfGetVSyncCount(void);

// -- Wait for the next VSync
void pfVSyncWait(void);

#endif
