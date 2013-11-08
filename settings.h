#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
    Q_OBJECT

public:
    explicit Settings(QObject *parent = 0);
    ~Settings();

    // Get's
    Q_INVOKABLE bool GetAutoBackupShort()           { return m_bAutoBackupShort; }
    Q_INVOKABLE short GetMaxBackupShort()           { return m_iMaxBackupShort; }
    Q_INVOKABLE bool GetAutoBackupLong()            { return m_bAutoBackupLong; }
    Q_INVOKABLE short GetMaxBackupLong()            { return m_iMaxBackupLong; }

    Q_INVOKABLE QString GetSavesDirectory()         { return m_sSavesDirectory; }

    // Set's
    Q_INVOKABLE void SetAutoBackupShort(bool v)     { m_bAutoBackupShort = v; }
    Q_INVOKABLE void InvertAutoBackupShort()        { m_bAutoBackupShort = !m_bAutoBackupShort; }
    Q_INVOKABLE void SetMaxBackupShort(int v)       { m_iMaxBackupShort = v; }

    Q_INVOKABLE void SetAutoBackupLong(bool v)      { m_bAutoBackupLong = v; }
    Q_INVOKABLE void InvertAutoBackupLong()         { m_bAutoBackupLong = !m_bAutoBackupLong; }
    Q_INVOKABLE void SetMaxBackupLong(int v)        { m_iMaxBackupLong = v; }

    Q_INVOKABLE void SetSavesDirectory(QString v)   { m_sSavesDirectory = v; }

private:
    bool m_bAutoBackupShort;
    int m_iMaxBackupShort;
    bool m_bAutoBackupLong;
    int m_iMaxBackupLong;

    QString m_sSavesDirectory;
};

#endif // SETTINGS_H
