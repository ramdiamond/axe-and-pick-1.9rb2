#include <QQuickItem>

#include "qtquick2applicationviewer.h"

#include "main.h"
#include "mainapp.h"

void GUISettingsWindow::UpdateDisplay()
{
    // Update the file info
    QObject *pqo = g_pMainApp->GetGuiSettingsDialogViewer()->rootObject()->findChild<QObject*>("SavesDirectoryText");
    if (! pqo) return;
    pqo->setProperty("text", g_pMainApp->GetSettings()->GetSavesDirectory());
    pqo->setProperty("border.color", g_pMainApp->GetSavesAccess()->pathIsValid() ? "green" : "red");

    // Update the checkboxs
    pqo = g_pMainApp->GetGuiSettingsDialogViewer()->rootObject()->findChild<QObject*>("AutoBackupShortCheckboxId");
    if (! pqo) return;
    pqo->setProperty("checked", g_pMainApp->GetSettings()->GetAutoBackupShort());

    // Update the checkboxs
    pqo = g_pMainApp->GetGuiSettingsDialogViewer()->rootObject()->findChild<QObject*>("AutoBackupLongCheckboxId");
    if (! pqo) return;
    pqo->setProperty("checked", g_pMainApp->GetSettings()->GetAutoBackupLong());
}

/******************************************
 *
 * Q_INVOKABLE buttons
 *
 ******************************************/
void GUISettingsWindow::onClickOpenSavesDirectoryFile()
{
    g_pMainApp->GetSavesAccess()->openFileDialog();
    UpdateDisplay();
}

void GUISettingsWindow::onTextChangedSavesDirectory(QVariant text)
{
    g_pMainApp->GetSettings()->SetSavesDirectory(text.toString());
    UpdateDisplay();
}

void GUISettingsWindow::onClickAutoBackupShort()
{
    g_pMainApp->GetSettings()->InvertAutoBackupShort();
    UpdateDisplay();
}

void GUISettingsWindow::onClickAutoBackupLong()
{
    g_pMainApp->GetSettings()->InvertAutoBackupLong();
    UpdateDisplay();
}
