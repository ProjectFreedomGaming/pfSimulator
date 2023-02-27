// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfMain.h"
#include "pfSimulator.h"

// -- This is our main simulator instance
static PFSimulator* _simulator = NULL;

// -- Setup up the simulator app
void pfMain(int argc, char* argv[])
{
    if (argc < 1) {
        return;
    }

    pfSimulatorInit();

    // -- Create our main window
    _simulator = pfSimulatorNew(argv[0]);
    if (_simulator == NULL) {
        return;
    }

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
    if (_simulator != NULL) {
        pfSimulatorDelete(_simulator);
        _simulator = NULL;
    }

    pfSimulatorShutdown();
}
