
#include "main.h"
#include "guimainwindow.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    GUIMainWindow guimainwindow;

    // We don't really know when g_pGuiMainWindow starts to exist, or get's deleted, but we do know it exists in-between our main function.
    // Globals ~might~ be bad form, but they make sence to an old C guy :) Long as we don't have too many globals, but one should be plenty :)
    g_pGuiMainWindow = &guimainwindow;

    if ( (ret = guimainwindow.Initialize(argc, argv)) ) return ret;
    if ( (ret = guimainwindow.Main()) ) return ret;
    if ( (ret = guimainwindow.Clean()) ) return ret;

    g_pGuiMainWindow = NULL;

    return ret;
}
