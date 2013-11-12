#include <QQuickItem>

#include "qtquick2applicationviewer.h"

#include "main.h"
#include "mainapp.h"

/************************************
 *
 * Q_INVOKABLE's for buttons
 *
 ************************************/
void GUIMainWindow::OpenSettingsButton()
{
    // Update the displayed info
    g_pMainApp->UpdateSettingsDisplay();

    // This will set the ApplicationModal, TransientParent (not Parent) settings window as the app focused window.
    g_pMainApp->GetGuiSettingsDialogViewer()->showExpanded();
}
