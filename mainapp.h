#ifndef MAINAPP_H
#define MAINAPP_H

#include <QCoreApplication>
#include <QApplication>
#include <QDebug>
#include <QAbstractListModel>
#include <QBitArray>
#include <QList>
#include <QObject>
#include <QSortFilterProxyModel>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QTime>
#include <sstream>

#include <QQmlContext>
#include <QQmlComponent>

#include "qtquick2applicationviewer.h"

#include "settings.h"

#include "guimainwindow.h"
#include "guisettingswindow.h"

#include "listmodel.h"
#include "savedgamelistmodel.h"
#include "resourcelistmodel.h"
#include "humanlistmodel.h"
#include "neutralmoblistmodel.h"
#include "violentmoblistmodel.h"

#include "savesaccess.h"

class MainApp
{
public:
    MainApp();

    int Initialize(int argc, char *argv[]);
    int Clean();

    int Main();

    void ShowSettingsWindow();

    // Almost get's
    inline void UpdateSettingsDisplay()                               { return m_pGuiSettingsWindow->UpdateDisplay(); }
    // Get's
    inline QtQuick2ApplicationViewer* GetGuiMainViewer()              { return m_pqtGuiMainViewer; }
    inline QtQuick2ApplicationViewer* GetGuiSettingsDialogViewer()    { return m_pqtGuiSettingsDialogViewer; }
    inline Settings* GetSettings()                                    { return m_pSettings; }
    inline SavesAccess* GetSavesAccess()                              { return m_pSavesAccess; }

private:
    bool                        m_bInitialized;

    GUIMainWindow               *m_pGuiMainWindow;
    GUISettingsWindow           *m_pGuiSettingsWindow;

    QApplication                *m_pqtApp;
    QtQuick2ApplicationViewer   *m_pqtGuiMainViewer;
    QtQuick2ApplicationViewer   *m_pqtGuiOpenSavedGameViewer;
    QtQuick2ApplicationViewer   *m_pqtGuiSettingsDialogViewer;

    Settings                    *m_pSettings;
    SavesAccess                 *m_pSavesAccess;

    SavedGameListModel          *m_pSavedGameModel;
    QSortFilterProxyModel       *m_pqtProxySavedGameModel;

    ResourceListModel           *m_pResourceModel;
    QSortFilterProxyModel       *m_pqtProxyResourceModel;

    HumanListModel              *m_pHumanModel;
    QSortFilterProxyModel       *m_pqtProxyHumanModel;

    NeutralMobListModel         *m_pNeutralMobModel;
    QSortFilterProxyModel       *m_pqtProxyNeutralMobModel;

    ViolentMobListModel         *m_pViolentMobModel;
    QSortFilterProxyModel       *m_pqtProxyViolentMobModel;

    HumanListModel              *m_pMigrantModel;
    QSortFilterProxyModel       *m_pqtProxyMigrantModel;
};

#endif // MAINAPP_H
