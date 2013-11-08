#include "guisettingswindow.h"
#include "guimainwindow.h"

GUISettingsWindow::GUISettingsWindow()
{
}


int GUISettingsWindow::Initialize()
{
    int ret = 0;

    return ret;
}

int GUISettingsWindow::Clean()
{
    int ret = 0;

    return ret;
}



/******************************************
 *
 * Q_INVOKABLE buttons
 *
 ******************************************/



void GUISettingsWindow::OnClickAutoBackupShort()
{

}

void GUISettingsWindow::OnClickAutoBackupLong()
{

}



/******************************************
 *
 * Q_INVOKABLE get's
 *
 ******************************************/



QVariant GUISettingsWindow::GetAutoBackupShort()
{
    return g_pGuiMainWindow->GetSettings()->GetAutoBackupShort();
}

QVariant GUISettingsWindow::GetAutoBackupLong()
{
    return g_pGuiMainWindow->GetSettings()->GetAutoBackupLong();
}
