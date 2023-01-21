// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

import Cocoa

@main
class AppDelegate: NSObject, NSApplicationDelegate {

    @IBOutlet var window: NSWindow!

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // -- We don't need the app's Window since the simultor will open its own.
        window.close()
        
        // -- Launch the simulator.
        pfMain()
        
        // -- If the simulator exited, then signal the app that it needs to shutown.
        NSApp.terminate(self)
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // -- Clean up any leftovers before exiting.
        pfTerminate()
    }

    func applicationSupportsSecureRestorableState(_ app: NSApplication) -> Bool {
        return true
    }
}
