#include "qtquick2applicationviewer.h"

#include "main.h"
#include "mainapp.h"
#include "guimainwindow.h"
#include "guisettingswindow.h"

MainApp::MainApp()
{
    m_bInitialized = false;

    m_pGuiMainWindow = NULL;
    m_pGuiSettingsWindow = NULL;

    m_pqtApp = NULL;
    m_pqtGuiMainViewer = NULL;
    m_pqtGuiOpenSavedGameViewer = NULL;
    m_pqtGuiSettingsDialogViewer = NULL;

    m_pSettings = NULL;
    m_pSavesAccess = NULL;

    m_pSavedGameModel = NULL;
    m_pqtProxySavedGameModel = NULL;

    m_pResourceModel = NULL;
    m_pqtProxyResourceModel = NULL;

    m_pHumanModel = NULL;
    m_pqtProxyHumanModel = NULL;

    m_pNeutralMobModel = NULL;
    m_pqtProxyNeutralMobModel = NULL;

    m_pViolentMobModel = NULL;
    m_pqtProxyViolentMobModel = NULL;

    m_pMigrantModel = NULL;
    m_pqtProxyMigrantModel = NULL;
}

/****************************************
 *
 * Inits and Cleans
 *
 ****************************************/
int MainApp::Initialize(int argc, char *argv[])
{
    if (m_bInitialized) return 0;

    int ret = Clean(); // Just good form, shouldn't be neccessary, but why not?
    if (ret) return ret;

    // Seed the random number generator
    qsrand( (uint)( QTime::currentTime().msec() ) );

#if TESTING
    QByteArray binary;
    unsigned int result;
    bool failed(false);
    for( unsigned int value=0; value<65000; value++ )
    {
        binary = SavesAccess::toBinary(value);
        result = SavesAccess::toInt(binary);
        if( result != value)
        {
            qDebug() << "Value" << value << "returned" << result;
            failed = true;
            break;
        }
    }
    if( failed )
    {
        qDebug() << "[FAILURE] The tests did NOT succeed!";
    }
    else
    {
        qDebug() << "[PASSED]  All tests passed.";
    }
#endif

    m_pqtApp = new QApplication(argc, argv);

    QStringList libPaths = QCoreApplication::libraryPaths();
    libPaths.append(QCoreApplication::applicationDirPath() + "/plugins");
    m_pqtApp->setLibraryPaths(libPaths);

    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setOrganizationDomain(APP_ORG_DOMAIN);
    QCoreApplication::setOrganizationName(APP_ORG_NAME);

    m_pqtApp->setWindowIcon(QIcon());

    // Register the list item types for the enumerations in QML
    qmlRegisterType<Resource>("Resource", 1,0, "Resource");
    qmlRegisterType<SavedGame>("SavedGame", 1,0, "SavedGame");

    // Setup Main view (after registering types)
    m_pGuiMainWindow = new GUIMainWindow();
    m_pqtGuiMainViewer = new QtQuick2ApplicationViewer();

    // Load and create the settings. Give QML access.
    m_pSettings = new Settings();
    m_pqtGuiMainViewer->rootContext()->setContextProperty("settings", m_pSettings);

    //
    // SAVED GAMES
    //
    // This holds the info for all the saved games.
    m_pSavesAccess = new SavesAccess();
    m_pqtGuiMainViewer->rootContext()->setContextProperty("savesAccess", m_pSavesAccess);

    m_pSavedGameModel = new SavedGameListModel(new SavedGame, qApp);
    m_pSavesAccess->setSavedGameListModel(m_pSavedGameModel);
    m_pSavesAccess->loadGamesList();

    m_pqtGuiMainViewer->rootContext()->setContextProperty("savedGameModel", m_pSavedGameModel);

    //
    // RESOURCES
    //
    // Create a model that holds our resource data, and
    // add the data to it. Then make it available to QML.
    m_pResourceModel = new ResourceListModel(new Resource, qApp);
    m_pSavesAccess->setResourceListModel(m_pResourceModel);
    m_pqtGuiMainViewer->rootContext()->setContextProperty("resourceModel", m_pResourceModel);

    // Create the proxy model that contains the results of the filter.
    m_pqtProxyResourceModel = new QSortFilterProxyModel();
    m_pqtProxyResourceModel->setSourceModel(m_pResourceModel);
    m_pqtProxyResourceModel->setFilterRole(Resource::FilterStringRole);
    // This prevents unknown items from showing up in the list.
    m_pqtProxyResourceModel->setFilterRegExp("^(?!unknown).*");
    m_pqtGuiMainViewer->rootContext()->setContextProperty("resourceModelProxy", m_pqtProxyResourceModel);

    // Enable the case insensitivity
    m_pqtProxyResourceModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // Setup sorting the resources
    //proxyResourceModel->setSortRole(Resource::TypeRole);
    //proxyResourceModel->sort(0);

    //
    // UNITS
    //
    // Humans
    m_pHumanModel = new HumanListModel(new Human, qApp);
    m_pSavesAccess->setHumanModel(m_pHumanModel);
    m_pqtGuiMainViewer->rootContext()->setContextProperty("humanModel", m_pHumanModel);
    m_pqtProxyHumanModel = new QSortFilterProxyModel();
    m_pqtProxyHumanModel->setSourceModel(m_pHumanModel);
    m_pqtProxyHumanModel->setFilterRole(Human::FilterStringRole);
    m_pqtProxyHumanModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_pqtGuiMainViewer->rootContext()->setContextProperty("humanModelProxy", m_pqtProxyHumanModel);
    m_pqtProxyHumanModel->setFilterRegExp("^(?!unknown).*");

    // Neutral Mobs
    m_pNeutralMobModel = new NeutralMobListModel(new NeutralMob, qApp);
    m_pSavesAccess->setNeutralMobModel(m_pNeutralMobModel);
    m_pqtGuiMainViewer->rootContext()->setContextProperty("neutralMobModel", m_pNeutralMobModel);
    m_pqtProxyNeutralMobModel = new QSortFilterProxyModel();
    m_pqtProxyNeutralMobModel->setSourceModel(m_pNeutralMobModel);
    m_pqtProxyNeutralMobModel->setFilterRole(NeutralMob::FilterStringRole);
    m_pqtProxyNeutralMobModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_pqtGuiMainViewer->rootContext()->setContextProperty("neutralMobModelProxy", m_pqtProxyNeutralMobModel);

    // Violent Mobs
    m_pViolentMobModel = new ViolentMobListModel(new ViolentMob, qApp);
    m_pSavesAccess->setViolentMobModel(m_pViolentMobModel);
    m_pqtGuiMainViewer->rootContext()->setContextProperty("violentMobModel", m_pViolentMobModel);
    m_pqtProxyViolentMobModel = new QSortFilterProxyModel();
    m_pqtProxyViolentMobModel->setSourceModel(m_pViolentMobModel);
    m_pqtProxyViolentMobModel->setFilterRole(ViolentMob::FilterStringRole);
    m_pqtProxyViolentMobModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_pqtGuiMainViewer->rootContext()->setContextProperty("violentMobModelProxy", m_pqtProxyViolentMobModel);

    // Migrants
    m_pMigrantModel = new HumanListModel(new Human, qApp);
    m_pSavesAccess->setMigrantModel(m_pMigrantModel);
    m_pqtGuiMainViewer->rootContext()->setContextProperty("migrantModel", m_pMigrantModel);
    m_pqtProxyMigrantModel = new QSortFilterProxyModel();
    m_pqtProxyMigrantModel->setSourceModel(m_pMigrantModel);
    m_pqtProxyMigrantModel->setFilterRole(Human::FilterStringRole);
    m_pqtProxyMigrantModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_pqtGuiMainViewer->rootContext()->setContextProperty("migrantModelProxy", m_pqtProxyMigrantModel);

    // Setup the other dialogs
    m_pGuiSettingsWindow = new GUISettingsWindow();
    m_pqtGuiSettingsDialogViewer = new QtQuick2ApplicationViewer();

    /* Now that our BS, i meen QML types and junk are settup for our javascript qml interpreter BS, we can init the windows :P
     *
     * Do window init here
     */
    m_pqtGuiMainViewer->setMinimumWidth(400);
    m_pqtGuiMainViewer->setMinimumHeight(350);
    m_pqtGuiMainViewer->setTitle(APP_NAME" v"APP_VERSION);

    /* Set our handler, to communicate with that BS qml crap...
     *
     * Why make a C compiler, and library, and then implement an entire
     * javascript interpreter, and force C developers to write in javascript
     * ... I dono... It's like saying java is OS independant... I mean, sure, it is...
     * but only with the OS dependant runtime installed...
     *
     * C is just as OS independant.
     */
    m_pqtGuiMainViewer->rootContext()->setContextProperty("myHandler", m_pGuiMainWindow);

    // Load the QML last
    m_pqtGuiMainViewer->setMainQmlFile(QStringLiteral("qml/AxeAndPick/GuiMainWindow.qml"));

    m_pqtGuiSettingsDialogViewer = new QtQuick2ApplicationViewer();
    m_pqtGuiSettingsDialogViewer->setMinimumWidth(400);
    m_pqtGuiSettingsDialogViewer->setMinimumHeight(350);
    m_pqtGuiSettingsDialogViewer->setTitle(APP_NAME" v"APP_VERSION" - Settings");

    // This makes the window a 'child' with 'forced focus' of main view, but also an independant window within the app.
    m_pqtGuiSettingsDialogViewer->setTransientParent(m_pqtGuiMainViewer);
    m_pqtGuiSettingsDialogViewer->setModality(Qt::ApplicationModal);

    m_pqtGuiSettingsDialogViewer->rootContext()->setContextProperty("myHandler", m_pGuiSettingsWindow);

    // Load qml last
    m_pqtGuiSettingsDialogViewer->setMainQmlFile(QStringLiteral("qml/AxeAndPick/GuiSettingsWindow.qml"));

    m_bInitialized = true;

    return ret;
}

int MainApp::Clean()
{
    int ret = 0;

    // Clean up
    if (m_pSavedGameModel)              delete m_pSavedGameModel;
    m_pSavedGameModel = NULL;
    if (m_pqtProxySavedGameModel)       delete m_pqtProxySavedGameModel;
    m_pqtProxySavedGameModel = NULL;

    if (m_pResourceModel)               delete m_pResourceModel;
    m_pResourceModel = NULL;
    if (m_pqtProxyResourceModel)        delete m_pqtProxyResourceModel;
    m_pqtProxyResourceModel = NULL;

    if (m_pHumanModel)                  delete m_pHumanModel;
    m_pHumanModel = NULL;
    if (m_pqtProxyHumanModel)           delete m_pqtProxyHumanModel;
    m_pqtProxyHumanModel = NULL;

    if (m_pNeutralMobModel)             delete m_pNeutralMobModel;
    m_pNeutralMobModel = NULL;
    if (m_pqtProxyNeutralMobModel)      delete m_pqtProxyNeutralMobModel;
    m_pqtProxyNeutralMobModel = NULL;

    if (m_pViolentMobModel)             delete m_pViolentMobModel;
    m_pViolentMobModel = NULL;
    if (m_pqtProxyViolentMobModel)      delete m_pqtProxyViolentMobModel;
    m_pqtProxyViolentMobModel = NULL;

    if (m_pMigrantModel)                delete m_pMigrantModel;
    m_pMigrantModel = NULL;
    if (m_pqtProxyMigrantModel)         delete m_pqtProxyMigrantModel;
    m_pqtProxyMigrantModel = NULL;

    // These don't like to be first XD
    if (m_pqtGuiSettingsDialogViewer)   delete m_pqtGuiSettingsDialogViewer;
    m_pqtGuiSettingsDialogViewer = NULL;
    if (m_pGuiSettingsWindow)           delete m_pGuiSettingsWindow;
    m_pGuiSettingsWindow = NULL;

    if (m_pqtGuiOpenSavedGameViewer)    delete m_pqtGuiOpenSavedGameViewer;
    m_pqtGuiOpenSavedGameViewer = NULL;
    // TODO: Add m_pGuiOpen...

    if (m_pqtGuiMainViewer)             delete m_pqtGuiMainViewer;
    m_pqtGuiMainViewer = NULL;
    if (m_pGuiMainWindow)               delete m_pGuiMainWindow;
    m_pGuiMainWindow = NULL;

    if (m_pSettings)                    delete m_pSettings;
    m_pSettings = NULL;
    if (m_pSavesAccess)                 delete m_pSavesAccess;
    m_pSavesAccess = NULL;

    // This really likes to be last to be deleted.
    // Probably something to do with parent/sibling relations in QT libs.
    if (m_pqtApp)                       delete m_pqtApp;
    m_pGuiMainWindow = NULL;

    m_bInitialized = false;

    return ret;
}

/*************************************
 *
 * Main loop (Mostly done in the QT libs)
 *
 *************************************/
int MainApp::Main()
{
    int ret = 0;

    // Show the GUI
    m_pqtGuiMainViewer->showExpanded();

    // Start the main event loop.
    ret = m_pqtApp->exec();

    return ret;
}
