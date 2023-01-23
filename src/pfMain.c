// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfMain.h"
#include "pfSimulator.h"

// -- This is our main simulator instance
static PFSimulator* simulator = NULL;

// -- Setup up the simulator app
void pfMain(void)
{
    pfSimulatorInit();

    // -- Create our main window
    simulator = pfSimulatorNew();

    // -- Then do nothing until the app quits
    while (1) {
        if (pfSimulatorUpdate() == kMustQuit) {
            return;
        }
    }
}

// -- Clean up our mess before exiting
void pfTerminate(void)
{
    if (simulator != NULL) {
        pfSimulatorDelete(simulator);
        simulator = NULL;
    }

    pfSimulatorShutdown();
}
