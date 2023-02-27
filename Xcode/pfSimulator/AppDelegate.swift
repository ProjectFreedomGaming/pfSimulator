// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

import Cocoa
import UniformTypeIdentifiers

@main
class AppDelegate: NSObject, NSApplicationDelegate {

    @IBOutlet var window: NSWindow!

    func getRomFilePath() -> String? {
        // -- Pathname of the rom file to load
        var chosen_file_path: String? = nil

        if CommandLine.argc > 1 {
            var index: Int = 1
            while index < CommandLine.argc {
                let argument = CommandLine.arguments[index]
                if argument.starts(with: "-") {
                    index += 1
                }
                else if FileManager.default.fileExists(atPath: argument) {
                    // -- We only accept the first file path on the command line for now
                    chosen_file_path = argument
                    break
                }

                index += 1
            }
        }
        
        if chosen_file_path == nil {
            let dialog = NSOpenPanel()
            dialog.title = "Choose a .rom file"
            dialog.showsResizeIndicator = true
            dialog.showsHiddenFiles = false
            dialog.canChooseDirectories = false
            dialog.canCreateDirectories = false
            dialog.allowsMultipleSelection = false
            dialog.allowedContentTypes = [UTType("io.projectfreedom.pfxrom")!]
            dialog.makeKeyAndOrderFront(dialog)
            dialog.center()
            
            if (dialog.runModal() != NSApplication.ModalResponse.OK) {
                // -- User clicked on "Cancel".
                NSApp.terminate(self)
            }
            
            chosen_file_path = dialog.url?.path
        }

        return chosen_file_path
    }
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // -- We don't need the app's Window since the simultor will open its own.
        window.close()
        
        let result = self.getRomFilePath()
        if (result == nil) {
            // -- Something went wrong.
            NSApp.terminate(self)
        }
        
        let arguments = [result]
        
        // == Create [UnsafeMutablePointer<Int8>]:
        var cargs = arguments.map { strdup($0) }

        // -- Launch the simulator.
        pfMain(Int32(arguments.count), &cargs);

        // -- Free the duplicated strings:
        for ptr in cargs { free(ptr) }
        
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
