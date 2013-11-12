#include <QCoreApplication>

#include "main.h"
#include "mainapp.h"

MainApp *g_pMainApp = NULL;

int main(int argc, char *argv[])
{
    int ret = 0;
    g_pMainApp = new MainApp();
    if (! g_pMainApp) return RET_ERROR;

    if ( (ret = g_pMainApp->Initialize(argc, argv)) ) return ret;
    if ( (ret = g_pMainApp->Main()) ) return ret;
    if ( (ret = g_pMainApp->Clean()) ) return ret;

    delete g_pMainApp;
    g_pMainApp = NULL;

    return ret;
}
