#ifndef GUIMAIN_H
#define GUIMAIN_H

#include <QtGui/QGuiApplication>
#include <QtWidgets/QApplication>

#include "qtquick2applicationviewer.h"

class GUIMainWindow : public QObject
{
private:
    Q_OBJECT

public:
    GUIMainWindow(QObject* parent = 0) : QObject(parent) { }

    // Q_INVOKABLE's for buttons
    Q_INVOKABLE void OpenSettingsButton();

    // Q_INVOKABLE's for get's
};

#endif // GUIMAIN_H
