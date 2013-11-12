#include "main.h"
#include "mainapp.h"
#include "settings.h"

Settings::Settings(QObject *parent)
        : QSettings(QSettings::IniFormat,
                    QSettings::UserScope,
                    QCoreApplication::instance()->organizationName(),
                    QCoreApplication::instance()->applicationName(),
                    parent)
{
    m_bAutoBackupShort = value("TimberAndStone/AutoBackupShort", true).toBool();
    m_iMaxBackupShort = value("TimberAndStone/MaxBackupShort", 10).toInt();
    m_bAutoBackupLong = value("TimberAndStone/AutoBackupLong", false).toBool();
    m_iMaxBackupLong = value("TimberAndStone/MaxBackupLong", 10).toInt();


    m_sSavesDirectory = value("TimberAndStone/GameInstallationDirectory", "").toString();

    // Keep the settings file clean, but don't lose old data :)
    if (m_sSavesDirectory.toStdString() != "") {
        setValue("TimberAndStone/SavesDirectory", m_sSavesDirectory);
        remove("TimberAndStone/GameInstallationDirectory");
    } else {
        m_sSavesDirectory = value("TimberAndStone/SavesDirectory", "Timber and Stone 'saves.sav' file").toString();
    }
}

Settings::~Settings() {
    setValue("TimberAndStone/AutoBackupShort", m_bAutoBackupShort);
    setValue("TimberAndStone/MaxBackupShort", m_iMaxBackupShort);
    setValue("TimberAndStone/AutoBackupLong", m_bAutoBackupLong);
    setValue("TimberAndStone/MaxBackupLong", m_iMaxBackupLong);

    setValue("TimberAndStone/SavesDirectory", m_sSavesDirectory);
}
