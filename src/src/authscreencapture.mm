#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include <AppKit/AppKit.h>
//#include <libproc.h>
#include "authscreencapture.h"
#include <QObject>

NSString * runCommand(NSString *commandToRun);

bool canRecordScreen()
{
    if (@available(macos 10.15, *)) {
        bool isCanRecordScreen = true;
        if (@available(macOS 10.15, *)) {
            isCanRecordScreen = false;
            NSRunningApplication *runningApplication = NSRunningApplication.currentApplication;
            NSNumber *ourProcessIdentifier = [NSNumber numberWithInteger:runningApplication.processIdentifier];

            CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
            NSUInteger numberOfWindows = CFArrayGetCount(windowList);
            for (int index = 0; index < numberOfWindows; index++) {
                // get information for each window
                NSDictionary *windowInfo = (NSDictionary *)CFArrayGetValueAtIndex(windowList, index);
                NSString *windowName = windowInfo[(id)kCGWindowName];
                NSNumber *processIdentifier = windowInfo[(id)kCGWindowOwnerPID];

                // don't check windows owned by this process
                if (! [processIdentifier isEqual:ourProcessIdentifier]) {
                    // get process information for each window
                    pid_t pid = processIdentifier.intValue;
                    NSRunningApplication *windowRunningApplication = [NSRunningApplication runningApplicationWithProcessIdentifier:pid];
                    if (! windowRunningApplication) {
                        // ignore processes we don't have access to, such as WindowServer, which manages the windows named "Menubar" and "Backstop Menubar"
                    }
                    else {
                        NSString *windowExecutableName = windowRunningApplication.executableURL.lastPathComponent;
                        if (windowName) {
                            if ([windowExecutableName isEqual:@"Dock"]) {
                                // ignore the Dock, which provides the desktop picture
                            }
                            else {
                                isCanRecordScreen = true;
                                break;
                            }
                        }
                    }
                }
            }
            CFRelease(windowList);
        }
        return isCanRecordScreen;
    } else {
        return true;
    }
}

void openScreenCaptureSetting(){
    NSString *urlString = @"x-apple.systempreferences:com.apple.preference.security?Privacy_ScreenCapture";
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:urlString]];
}

NSString * runCommand(NSString *commandToRun)
{
    NSTask *task = [[NSTask alloc] init];
    [task setLaunchPath:@"/bin/sh"];
    
    NSArray *arguments = [NSArray arrayWithObjects:
                          @"-c" ,
                          [NSString stringWithFormat:@"%@", commandToRun],
                          nil];
    NSLog(@"run command:%@", commandToRun);
    [task setArguments:arguments];
    
    NSPipe *pipe = [NSPipe pipe];
    [task setStandardOutput:pipe];
    
    NSFileHandle *file = [pipe fileHandleForReading];
    
    [task launch];
    
    NSData *data = [file readDataToEndOfFile];
    
    NSString *output = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    return output;
}

void openTipsDialog(void) {
    NSAlert *alert = [[NSAlert alloc] init];
    char* confirmText = QObject::tr("OK").toUtf8().data();
    char* messageText = QObject::tr("Set permissions").toUtf8().data();
    char* informativeText = QObject::tr("Please set screen capture jurisdiction").toUtf8().data();
    NSString *confirmTextStr = [NSString stringWithUTF8String:confirmText];
    NSString *messageTextStr = [NSString stringWithUTF8String:messageText];
    NSString *informativeTextStr = [NSString stringWithUTF8String:informativeText];
    [alert addButtonWithTitle:confirmTextStr];
    [alert setMessageText:messageTextStr];
    [alert setInformativeText:informativeTextStr];
    [alert setAlertStyle:NSWarningAlertStyle];
    
    if ([alert runModal] == NSAlertFirstButtonReturn) {
        openScreenCaptureSetting();
    }
}

bool authRecordScreen()
{
    if (!canRecordScreen()) {
        runCommand(@"tccutil reset ScreenCapture com.netease.nmc.NERTCDemo-Mac");
        openTipsDialog();
        return false;
    }
    return true;
}

int menuHeight()
{
    const int menuHeight = [[[NSApplication sharedApplication] mainMenu] menuBarHeight];
    return menuHeight;
}
