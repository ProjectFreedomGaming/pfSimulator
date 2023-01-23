// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
