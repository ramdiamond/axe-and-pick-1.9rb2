#ifndef GUISETTINGSWINDOW_H
#define GUISETTINGSWINDOW_H

#include <QtGui/QGuiApplication>
#include <QtWidgets/QApplication>

#include "qtquick2applicationviewer.h"

class GUISettingsWindow : QObject
{
private:
    Q_OBJECT

public:
    GUISettingsWindow();

    int Initialize();
    int Clean();

    // Q_INVOKABLE buttons
    Q_INVOKABLE void OnClickAutoBackupShort();
    Q_INVOKABLE void OnClickAutoBackupLong();

    // Q_INVOKABLE get's
    Q_INVOKABLE QVariant GetAutoBackupShort();
    Q_INVOKABLE QVariant GetAutoBackupLong();

private:
    bool                        m_bInitialized;
};

#endif // GUISETTINGSWINDOW_H
