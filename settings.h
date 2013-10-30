#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
    Q_OBJECT

public:
    explicit Settings(QObject *parent = 0);
    ~Settings();

    // Get's and Set's
    Q_INVOKABLE bool getAutoBackupShort()           { return m_bAutoBackupShort; }
    Q_INVOKABLE bool getAutoBackupLong()            { return m_bAutoBackupLong; }
    Q_INVOKABLE QString getSavesDirectory()         { return m_sSavesDirectory; }

    Q_INVOKABLE void setAutoBackupShort(bool v)     { m_bAutoBackupShort = v; }
    Q_INVOKABLE void invertAutoBackupShort()        { m_bAutoBackupShort = !m_bAutoBackupShort; }
    Q_INVOKABLE void setAutoBackupLong(bool v)      { m_bAutoBackupLong = v; }
    Q_INVOKABLE void invertAutoBackupLong()         { m_bAutoBackupLong = !m_bAutoBackupLong; }
    Q_INVOKABLE void setSavesDirectory(QString v)   { m_sSavesDirectory = v; }

private:
    bool m_bAutoBackupShort;
    bool m_bAutoBackupLong;
    QString m_sSavesDirectory;
};



#endif // SETTINGS_H
