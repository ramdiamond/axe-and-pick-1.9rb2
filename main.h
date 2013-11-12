#ifndef MAIN_H
#define MAIN_H

#define APP_NAME        "Axe and Pick"
#define APP_ORG_DOMAIN  "potatominingcorp.com"
#define APP_ORG_NAME    "Potato Mining Corporation"
// Version History
// ---------------------
// 0.4 - first release, only resource editing
// 0.5 - second release, only tweaked resource list
// 1.0 - units: add and remove, added error display
// 1.1 - updated for T&S 1.0.
// 1.2 - fixed the animal life toggling on load
// 1.3 - added coffee, gave new units coffee
// 1.4 - updated for T&S 1.1.
// 1.5 - fixed oddly-infrequent crash
// 1.6 - added support for patrol points
// 1.7 - fixed adventurer (T&S v1.2 didn't change anything)
// 1.8 - updated for T&S 1.3 (and some other tweaks)
// 1.9 - updated for T&S 1.4
// 1.9r - updated for T&S 1.42 by ramdiamond
// 1.43.1 - Start of major rewrites, and updated for T&S 1.43 by ramdiamond
#define APP_VERSION     "1.43.2"

// Set this to 1 your debug build settings to enable extra testing
#ifndef TESTING
    #define TESTING 0
#endif

// Set this to which build environment you're building for
// WIN is windows 7 or the likes
// OSX is mac
// NIX is linux
#ifndef NIX
    #ifndef OSX
        #ifndef WIN
            #define WIN
        #endif
    #endif
#endif

#define RET_ERROR 255

class MainApp;

extern MainApp *g_pMainApp;

#endif // MAIN_H
