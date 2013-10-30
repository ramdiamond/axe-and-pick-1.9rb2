#include "settings.h"

#include <QCoreApplication>

Settings::Settings(QObject *parent)
        : QSettings(QSettings::IniFormat,
                    QSettings::UserScope,
                    QCoreApplication::instance()->organizationName(),
                    QCoreApplication::instance()->applicationName(),
                    parent)
{
    m_bAutoBackupShort = value("TimberAndStone/AutoBackupShort", true).toBool();
    m_bAutoBackupLong = value("TimberAndStone/AutoBackupLong", false).toBool();

    m_iMaxBackup = value("TimberAndStone/MaxBackup", 10).toInt();

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
    setValue("TimberAndStone/AutoBackupLong", m_bAutoBackupLong);

    setValue("TimberAndStone/MaxBackup", m_iMaxBackup);

    setValue("TimberAndStone/SavesDirectory", m_sSavesDirectory);
}

Q_DECLARE_METATYPE(Settings*)
