#ifndef GUISETTINGSWINDOW_H
#define GUISETTINGSWINDOW_H

#include <QtGui/QGuiApplication>
#include <QtWidgets/QApplication>

#include "qtquick2applicationviewer.h"

class GUISettingsWindow : public QObject
{
private:
    Q_OBJECT

public:
    GUISettingsWindow(QObject* parent = 0) : QObject(parent) { }

    void UpdateDisplay();

    // Q_INVOKABLE buttons
    Q_INVOKABLE void onClickOpenSavesDirectoryFile();
    Q_INVOKABLE void onTextChangedSavesDirectory(QVariant text);
    Q_INVOKABLE void onClickAutoBackupShort();
    Q_INVOKABLE void onClickAutoBackupLong();

    // Q_INVOKABLE get's
/*    Q_INVOKABLE QVariant getSavesDirectory();
    Q_INVOKABLE QVariant isSavesDirectoryValid();
    Q_INVOKABLE QVariant getAutoBackupShort();
    Q_INVOKABLE QVariant getAutoBackupLong();*/
};

#endif // GUISETTINGSWINDOW_H
