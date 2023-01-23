// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_SIMULATOR_H
#define PF_SIMULATOR_H

#include "pfTypes.h"

// -- Simulator C type
typedef struct PFSimulator PFSimulator;

// -- Constants
typedef enum PFSimulatorUpdateStatus
{
    kNone,
    kMustQuit
} PFSimulatorUpdateStatus;

// -- Init, shutdown and update the simulator environment
bool pfSimulatorInit(void);
void pfSimulatorShutdown(void);
PFSimulatorUpdateStatus pfSimulatorUpdate(void);

// -- Create a new simulator
PFSimulator* pfSimulatorNew(void);

// -- Destroy an existing simulator
void pfSimulatorDelete(PFSimulator*);

// -- Get the current VSync count
size pfSimulatorGetVSyncCount(void);

// -- Wait for the next VSync
void pfSimulatorVSyncWait(void);

// -- Debug function to set the background color to something
void pfSimulatorClearDisplay(PFSimulator* this, byte r, byte g, byte b);

// -- Swap the drawing and display buffers
void pfSimulatorSwapDisplayBuffer(PFSimulator*);

#endif
