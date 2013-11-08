#ifndef GUIMAIN_H
#define GUIMAIN_H

#include <QtGui/QGuiApplication>
#include <QtWidgets/QApplication>
#include <QSortFilterProxyModel>

#include "qtquick2applicationviewer.h"

#include "guisettingswindow.h"

#include "settings.h"
#include "savesaccess.h"
#include "humanlistmodel.h"
#include "neutralmoblistmodel.h"
#include "violentmoblistmodel.h"

class GUIMainWindow : QObject
{
private:
    Q_OBJECT

public:
    GUIMainWindow();

    int Initialize(int argc, char *argv[]);
    int Clean();

    int Main();

    // Q_INVOKABLE's for buttons
    Q_INVOKABLE void OpenSettingsButton();

    // Q_INVOKABLE's for get's

    // Cpp Get's
    Settings* GetSettings()          { return m_pSettings; }
    SavesAccess* GetSavesAccess()    { return m_pSavesAccess; }

private:
    bool                        m_bInitialized;

    GUIMainWindow               *m_pGuiMainWindow;
    GUISettingsWindow           *m_pGuiSettingsWindow;

    QApplication                *m_pqtApp;
    QtQuick2ApplicationViewer   *m_pqtMainViewer;
    QtQuick2ApplicationViewer   *m_pqtOpenSavedGameViewer;
    QtQuick2ApplicationViewer   *m_pqtSettingsDialogViewer;

    Settings                    *m_pSettings;
    SavesAccess                 *m_pSavesAccess;

    SavedGameListModel          *m_pSavedGameModel;

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

extern GUIMainWindow *g_pGuiMainWindow;

#endif // GUIMAIN_H
