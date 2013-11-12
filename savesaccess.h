#ifndef SAVESACCESS_H
#define SAVESACCESS_H

#include <QObject>
#include <QFile>
#include <QDir>

#include "savedgamelistmodel.h"
#include "resourcelistmodel.h"
#include "humanlistmodel.h"
#include "neutralmoblistmodel.h"
#include "violentmoblistmodel.h"
#include "utils.h"

class SavesAccess : public QObject
{
    Q_OBJECT

public:
    explicit SavesAccess(QObject *parent = 0);

    Q_INVOKABLE bool loadSavedGame(QString gameName);
    Q_INVOKABLE bool saveSavedGame();
private:
    QString selectedSaveName;

signals:
    void fileLoadStatusChanged(bool errorOccured, QString message);
    void fileSaveStatusChanged(bool errorOccured, QString message);

    //
    // Saved Game List
    //
public:
    void setSavedGameListModel(SavedGameListModel * model);
    Q_INVOKABLE void openFileDialog();
    Q_INVOKABLE bool pathIsValid();
    Q_INVOKABLE void loadGamesList();

private:
    SavedGameListModel * savedGameModel;


    //
    // Resource List
    //
public:
    void setResourceListModel(ResourceListModel * model);

private:
    bool loadResourceFile();
    bool saveResourceFile();

    ResourceListModel * resourceModel;

    QByteArray resourceExtraData;   // This is for holding the unexpected data


    //
    // Unit Lists
    //
public:
    void setHumanModel(HumanListModel * model);
    void setNeutralMobModel(NeutralMobListModel * model);
    void setViolentMobModel(ViolentMobListModel * model);
    void setMigrantModel(HumanListModel * model);

private:
    bool loadUnitFile();
    bool saveUnitFile();

    HumanListModel * humanModel;
    NeutralMobListModel * neutralMobModel;
    ViolentMobListModel * violentMobModel;
    HumanListModel * migrantModel;

    // World/Ground File
public:
    Q_INVOKABLE void writeToMatlab(int squareSize);
};

#endif // SAVESACCESS_H
