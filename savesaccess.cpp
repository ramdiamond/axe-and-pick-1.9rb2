#include "savesaccess.h"
#include "settings.h"
#include "main.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>
#include <QDebug>

SavesAccess::SavesAccess(QObject *parent) :
    QObject(parent),
    savedGameModel(Q_NULLPTR),
    resourceModel(Q_NULLPTR),
    humanModel(Q_NULLPTR),
    neutralMobModel(Q_NULLPTR),
    violentMobModel(Q_NULLPTR),
    migrantModel(Q_NULLPTR)
{
}

// Sets the path and loads the game.
bool SavesAccess::loadSavedGame(QString gameName)
{
    bool ret;

    // Set the game name.
    selectedSaveName = gameName;

    // Pull the resources into the list.
    ret = loadResourceFile();
    ret |= loadUnitFile();

    return ret;
}

bool SavesAccess::saveSavedGame()
{
    if (g_pSettings->getAutoBackupShort()) {
        int numBackups = 0;
        QDir saveDir;

        saveDir.setFilter(QDir::Files | QDir::Hidden | QDir::Readable);
        saveDir = g_pSettings->getSavesDirectory() + "/" + selectedSaveName;
        if (!saveDir.exists()) {
            QString message = "Saving failed! Check to make sure the save game directory exists, and has proper permissions.";
            emit fileSaveStatusChanged(true, message);

            return true;
        }

        QFileInfoList list = saveDir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            QStringList fileNameList = fileInfo.fileName().split(".");
            if (fileNameList.length() == 3) {
                if (fileNameList.at(0) == "re" || fileNameList.at(0) == "un") {
                    if /*(fileNameList.at(1) == "sav") {
                        if (numBackups == 0) numBackups = 0;
                    } else*/ (fileNameList.at(2) == "sav") {
                        if (fileNameList.at(1).toInt() > numBackups) {
                            numBackups = fileNameList.at(1).toInt();
                        }
                    }
                }
            }
        }

        numBackups++; // We're the next auto backup short number.
        QString fileOffset, fileOffsetMinus;

        if (numBackups > g_pSettings->getMaxBackup()) {
            // Now we can go back a bit :)
            numBackups = g_pSettings->getMaxBackup();

            if (! (QFile::remove(saveDir.filePath("re.1.sav"))) ) {
                QString message = "I can't delete the re.1.sav file!";
                qDebug() << message;
                emit fileSaveStatusChanged(true, message);
            }

            if (! (QFile::remove(saveDir.filePath("un.1.sav"))) ) {
                QString message = "I can't delete the un.1.sav file!";
                qDebug() << message;
                emit fileSaveStatusChanged(true, message);
            }

            for (int i = 2; i<=numBackups; i++) {
                fileOffset.setNum(i);
                fileOffsetMinus.setNum(i-1);
                if (! (QFile::rename(saveDir.filePath("re." + fileOffset + ".sav"), saveDir.filePath("re." + fileOffsetMinus + ".sav"))) ) {
                    QString message = "I can't backup the source file \"re." + fileOffset + ".sav\" to the dest file \"re." + fileOffsetMinus + ".sav\"";
                    qDebug() << 1 << message;
                    emit fileSaveStatusChanged(true, message);
                }
                if (! (QFile::rename(saveDir.filePath("un." + fileOffset + ".sav"), saveDir.filePath("un." + fileOffsetMinus + ".sav"))) ) {
                    QString message = "I can't backup the source file \"un." + fileOffset + ".sav\" to the dest file \"un." + fileOffsetMinus + ".sav\"";
                    qDebug() << 2 << message;
                    emit fileSaveStatusChanged(true, message);
                }
            }
        }

        fileOffset.setNum(numBackups);

        if (! (QFile::copy(saveDir.filePath("re.sav"), saveDir.filePath("re." + fileOffset + ".sav"))) ) {
            QString message = "I can't backup the source file \"re.sav\" to the dest file \"re." + fileOffset + ".sav\"";
            qDebug() << message;
            emit fileSaveStatusChanged(true, message);
        }
        if (! (QFile::copy(saveDir.filePath("un.sav"), saveDir.filePath("un." + fileOffset + ".sav"))) ) {
            QString message = "I can't backup the source file \"un.sav\" to the dest file \"un." + fileOffset + ".sav\"";
            qDebug() << message;
            emit fileSaveStatusChanged(true, message);
        }
    }

    if (g_pSettings->getAutoBackupLong()) {
        // TODO: this :P
        QString message = "Auto-backup Long not supported atm.";
        qDebug() << message;
        emit fileSaveStatusChanged(true, message);
    }

    if (saveResourceFile() | saveUnitFile())
        return true;

    return false;
}

void SavesAccess::openFileDialog()
{

    // TODO: Set the parent of this dialog so it returns properly.
    // Not sure if this is the cause of the weird Windows behavior
    // or not. Something to try.
    QFileDialog fileDialog(0,"Timber and Stone saves.sav file");
    fileDialog.setDirectory(g_pSettings->getSavesDirectory());
    fileDialog.setNameFilter("Saves File (saves.sav)");

    // Open the file dialog so the user can select the saves.sav file.
    if (fileDialog.exec())
    {
        QDir rootSavesDirectory;
        rootSavesDirectory.setPath(fileDialog.selectedFiles().first());
        rootSavesDirectory.cdUp(); // This trims off the saves.sav file.
        g_pSettings->setSavesDirectory(rootSavesDirectory.absolutePath());
    }
}

bool SavesAccess::pathIsValid()
{
    // Return True if both the directory path is valid
    // and the saves.sav file exists.
    QFile savedGameFile(g_pSettings->getSavesDirectory() + "/saves.sav");

    if (savedGameFile.exists())
        return true;

    return false;
}

void SavesAccess::loadGamesList()
{
    // Save the file name.
    QFile savedGameFile(g_pSettings->getSavesDirectory() + "/saves.sav");

    if (savedGameModel == Q_NULLPTR)
    {
        qDebug() << "SavedAccess model hasn't been set up yet.";
        return;
    }

    // Open file and make sure it went okay.
    if (!savedGameFile.exists() || !savedGameFile.open(QFile::ReadOnly | QFile::Text))
    {
        // TODO: Make this error apparent on the interface somehow.
        qDebug() << "Can't open savedGameFile.";
        return;
    }
    else
    {
        QTextStream in(&savedGameFile);
        in.setCodec("UTF-8");
        QStringList strings;

        // Read the number of games in the file, but
        // don't do anything with it.
        in.readLine().toLongLong();

        // Remove all the games.
        savedGameModel->clear();

        // Add the games from the saves.sav file.
        while (!in.atEnd())
        {
            strings = in.readLine().split("/", QString::KeepEmptyParts);

            // Add the games to the list.
            savedGameModel->appendRow(new SavedGame(
                    strings[0],
                    strings[3],
                    strings[4],
                    strings[1].toLongLong(),
                    strings[2].toLongLong(),
                    true
                ));
        }
    }

    // It's important to always close the file after reading/writing, so this
    // app can remain running while Timber and Stone saves as it wishes.
    savedGameFile.close();

    /* TODO:
     *
     * Add code to check for proper save game directory structure
     */
}

void SavesAccess::setSavedGameListModel(SavedGameListModel * model)
{
    savedGameModel = model;
}

// =====================
// RESOURCES
// =====================

void SavesAccess::setResourceListModel(ResourceListModel * model)
{
    resourceModel = model;
}

bool SavesAccess::loadResourceFile()
{
    QFile resourceFile(g_pSettings->getSavesDirectory()
                       + "/" + selectedSaveName
                       + "/" + "re.sav");

    if (resourceModel == Q_NULLPTR)
    {
        qDebug() << "Resource model hasn't been set up yet.";
        return true;
    }

    // Open file and make sure it went okay.
    if (!resourceFile.exists() || !resourceFile.open(QFile::ReadOnly))
    {
        qDebug() << g_pSettings->getSavesDirectory()
                    + "/" + selectedSaveName
                    + "/" + "re.sav";

        QString message = "I can't load the re.sav file! Is it missing? Saving is disabled.";
        emit fileLoadStatusChanged(true, message);


        return true;
    }
    else
    {

        // Pull in the list of resource assets (name, group, etc)
        QFile assetFile(QCoreApplication::applicationDirPath() + "/resource_list.csv");

        if (assetFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // Clear the resource model
            resourceModel->clear();

            QTextStream assetStream(&assetFile);

            while (!assetStream.atEnd())
            {
                // Grab the next section and decide the size, etc.
                QString assetString;
                assetString = assetStream.readLine();
                QStringList assetData;
                assetData = assetString.split(',');

                // Temporary array to hold the bytes we read from the file.
                QByteArray byteArray;
                byteArray.clear();

                unsigned char byte; // Most Significant Byte
                resourceFile.read((char *)&byte, 1);

                if( byte >= 0xe0)
                {
                    byteArray.append(byte);

                    // Another byte exists!
                    resourceFile.read((char *)&byte, 1);
                    byteArray.append(byte);
                }
                else
                {
                    byteArray.append(byte);
                }

                resourceFile.read((char *)&byte, 1);
                byteArray.append(byte);

                resourceModel->appendRow(new Resource(assetData[0],
                                          assetData[1],
                                          Utils::toInt(byteArray))); // resource quantity
            }

            if (!resourceFile.atEnd())
            {
                // Read the rest of the file into an array, and keep it for later.
                resourceExtraData.clear();
                resourceExtraData = resourceFile.readAll();
            }
        }
        else
        {
            QString message = "I can't load the resource_list.csv file! Is it missing? Saving is disabled.";
            emit fileLoadStatusChanged(true, message);

            return true;
        }

        assetFile.close();
        resourceFile.close();
    }

    return false;
}


bool SavesAccess::saveResourceFile()
{
    //
    // Ha ha ha! Man, I'm hilarious.
    //
//    if (qrand()%1000 < 10) {
//        // NOTE: This doesn't mess things up. Just save again and chance are
//        // good that it'll save next time.
//        QString message = "Failed to save game. The only loss of resources is one saved game.";
//        emit fileSaveStatusChanged(true, message);

//        return;
//    }

    if (resourceModel == Q_NULLPTR
            || resourceModel->rowCount() <= 0)
    {
        qDebug() << "The resource model hasn't been populated.";
        return true;
    }

    QFile resourceFile(g_pSettings->getSavesDirectory()
                       + "/" + selectedSaveName
                       + "/" + "re.sav");

    // Open file for write, and make sure it went okay.
    if (!resourceFile.open(QIODevice::WriteOnly))
    {
        QString message = "Saving failed! Check to make sure the re.sav file isn't open in another program.";
        emit fileSaveStatusChanged(true, message);

        return true;
    }
    else
    {
        QByteArray binaryData;

        // Write the data to the resource file. Clobber the old one.
        for (int ndx = 0;
             ndx < resourceModel->rowCount();
             ndx++)
        {
            long quantity = resourceModel->index(ndx).data(Resource::QuantityRole).toLongLong();
            if (quantity > 60000)
            {
                qDebug() << "Resource quantities above 65,278 will fail to load in the game. Reduced to 60,000.";
                quantity = 60000;

                // Update the resource. setData() requires an "id", not an index.
                resourceModel->setData(ndx+1,60000);
            }
            binaryData = Utils::toBinary(quantity);
            resourceFile.write(binaryData);
        }

        // Write out the unexpected data to the file too.
        resourceFile.write(resourceExtraData);
        resourceFile.close();
    }

    return false;
}

// =====================
// UNITS (human, neutral mobs, violent mobs)
// =====================

void SavesAccess::setHumanModel(HumanListModel * model)
{
    humanModel = model;
}
void SavesAccess::setNeutralMobModel(NeutralMobListModel * model)
{
    neutralMobModel = model;
}
void SavesAccess::setViolentMobModel(ViolentMobListModel * model)
{
    violentMobModel = model;
}
void SavesAccess::setMigrantModel(HumanListModel * model)
{
    migrantModel = model;
}

bool SavesAccess::loadUnitFile()
{
    bool errorOccured(false);
    QString message;

    // Compose the file name.
    QFile unitFile(g_pSettings->getSavesDirectory()
                   + "/" + selectedSaveName
                   + "/" + "un.sav");

    if ((humanModel == Q_NULLPTR) |
        (neutralMobModel == Q_NULLPTR) |
        (violentMobModel == Q_NULLPTR) |
        (migrantModel == Q_NULLPTR))
    {
        qDebug() << "Human, neutral, violent mob models haven't been set up yet.";
        return false;
    }

    // Open file and make sure it went okay.
    if (!unitFile.exists() || !unitFile.open(QFile::ReadOnly))
    {
        // TODO: Make this error apparent on the interface somehow.
        QString message = "The unit file (un.sav) seems to be missing! Saving disabled.";
        emit fileLoadStatusChanged(true, message);
        return false;
    }
    else
    {
        QTextStream unitStream(&unitFile);

        QString unitString;
        QStringList unitData;

        //
        // Load in all the Humans
        //
        humanModel->clear();
        int numberOfHumans = unitStream.readLine().toInt();
        for (int i=0; i<numberOfHumans; i++)
        {
            unitString = unitStream.readLine();
            Human * newHuman = Human::build(unitString);
            if (newHuman != Q_NULLPTR)
            {
                humanModel->appendRow(newHuman);
            }
            else
            {
                errorOccured = true;
                qDebug() << "Note: There are" << unitData.size() << "fields in the Human string, but that's probably not the source of the problem2.";
            }
        }

        //
        // Load in all the Neutral Mobs
        //
        neutralMobModel->clear();
        int numberOfNeutralMobs = unitStream.readLine().toInt();
        for (int i=0; i<numberOfNeutralMobs; i++)
        {
            unitString = unitStream.readLine();
            NeutralMob * newMob = NeutralMob::build(unitString);
            if (newMob != Q_NULLPTR)
            {
                neutralMobModel->appendRow(newMob);
            }
            else
            {
                errorOccured = true;
                qDebug() << "Note: There are" << unitData.size() << "fields in the Neutral Mob string, but that's probably not the source of the problem.";
            }
        }

        //
        // Load in all the Violent Mobs
        //
        violentMobModel->clear();
        int numberOfViolentMobs = unitStream.readLine().toInt();
        for (int i=0; i<numberOfViolentMobs; i++)
        {
            unitString = unitStream.readLine();
            ViolentMob * newMob = ViolentMob::build(unitString);
            if (newMob != Q_NULLPTR)
            {
                violentMobModel->appendRow(newMob);
            }
            else
            {
                errorOccured = true;
                qDebug() << "Note: There are" << unitData.size() << "fields in the Violent Mob string, but that's probably not the source of the problem.";
            }
        }

        //
        // Load all migrants... hopefully anyways XD
        //
        migrantModel->clear();
        int numberOfMigrants = 0;
        while (!unitStream.atEnd())
        {
            unitString = unitStream.readLine();
            qDebug() << "Loaded extra data: \"" << unitString << "\"";
            Human * newHuman = Human::build(unitString);
            if (newHuman != Q_NULLPTR)
            {
                migrantModel->appendRow(newHuman);
                numberOfMigrants++;
            }
            else
            {
                errorOccured = true;
                qDebug() << "Note: There are" << unitData.size() << "fields in the Migrant (or Extra) string, but that's probably not the source of the problem.";
            }
        }

        qDebug() << "From" << selectedSaveName
                 << ": Loaded" << humanModel->rowCount() << "of" << numberOfHumans << "humans,"
                 << neutralMobModel->rowCount() << "of" << numberOfNeutralMobs << "animals,"
                 << violentMobModel->rowCount() << "of" << numberOfViolentMobs << "bad guys, and"
                 << migrantModel->rowCount() << "migrants.";

        if (errorOccured) {
            message = "This saved-game version is not supported! Saving is disabled and data may be innacurate.";
        } else {
            message = "Game \"" + selectedSaveName + "\" loaded properly.";
        }
        emit fileLoadStatusChanged(errorOccured, message);

        unitFile.close();
    }

    return errorOccured;
}

bool SavesAccess::saveUnitFile()
{
    // Compile all the units into the unit file.

    if (   humanModel == Q_NULLPTR
        || neutralMobModel == Q_NULLPTR
        || violentMobModel == Q_NULLPTR
        || migrantModel == Q_NULLPTR)
    {
        qDebug() << "The unit models haven't been set up.";

        return true;
    }

    QFile unitFile(g_pSettings->getSavesDirectory()
                       + "/" + selectedSaveName
                       + "/" + "un.sav");


    // Open file for write, and make sure it went okay.
    if (!unitFile.open(QIODevice::WriteOnly))
    {
        QString message = "Saving failed! Check to make sure the un.sav file isn't open in another program.";
        emit fileSaveStatusChanged(true, message);

        return true;
    }
    else
    {
        QTextStream unitStream(&unitFile);

        // Save the humans, then the neutral mobs, then the violent mobs then migrants.

        // Humans
        unitStream << humanModel->rowCount() << endl;
        unitStream.flush();
        for (QList<ListItem*>::iterator itr = humanModel->getList().begin(); itr != humanModel->getList().end(); itr++)
        {
            Human* human = (Human*)*itr;
            human->writeToFile(unitFile);
        }

        // Neutral Mobs
        unitStream << neutralMobModel->rowCount() << endl;
        unitStream.flush();
        for (QList<ListItem*>::iterator itr = neutralMobModel->getList().begin(); itr != neutralMobModel->getList().end(); itr++)
        {
            NeutralMob* mob = (NeutralMob*)*itr;
            mob->writeToFile(unitFile);
        }

        // Violent Mobs
        unitStream << violentMobModel->rowCount() << endl;
        unitStream.flush();
        for (QList<ListItem*>::iterator itr = violentMobModel->getList().begin(); itr != violentMobModel->getList().end(); itr++)
        {
            ViolentMob* mob = (ViolentMob*)*itr;
            mob->writeToFile(unitFile);
        }

        // Migrants
        unitStream.flush();
        for (QList<ListItem*>::iterator itr = migrantModel->getList().begin(); itr != migrantModel->getList().end(); itr++)
        {
            Human* human = (Human*)*itr;
            human->writeToFile(unitFile);
        }
    }

    QString message = "Saved.";
    emit fileSaveStatusChanged(false, message);

    unitFile.close();

    return false;
}

void SavesAccess::writeToMatlab(int squareSize)
{
    qDebug() << "Opening file...";

    // Create a new file for storing all these points. Comma seperated.
    QFile matlabFile(g_pSettings->getSavesDirectory()
                    + "/" + selectedSaveName
                    + "/" + "cd.dat");
    matlabFile.open(QFile::WriteOnly);

    QFile worldFile(g_pSettings->getSavesDirectory()
                    + "/" + selectedSaveName
                    + "/" + "cd.sav");
    worldFile.open(QFile::ReadOnly);

    // Go until you get all the blocks
    for( int blockNumber = 0; blockNumber < (squareSize * squareSize * 48); blockNumber++ )
    {
        // Grab a line in the file
        QByteArray lineOfBytes = worldFile.readLine();

        int i;
        i=0;
        while( i < lineOfBytes.length())
        {
            // Get the most significant byte
            unsigned char byte = lineOfBytes.at(i);
            i++;

            // Temporary array to hold the bytes we read from the file.
            QByteArray byteArray;
            byteArray.clear();
            if( byte >= 0xe0)
            {
                byteArray.append(byte);

                // Another byte exists!
                byte = lineOfBytes.at(i);
                i++;
                byteArray.append(byte);
            }
            else
            {
                byteArray.append(byte);
            }
            byte = lineOfBytes.at(i);
            i++;
            byteArray.append(byte);

            // Add the byte to the array.
            if( Utils::toInt(byteArray) != 125
             && Utils::toInt(byteArray) != 126
             && Utils::toInt(byteArray) != 714
             && Utils::toInt(byteArray) != 0 )
            {
                matlabFile.write(Utils::toBinary(1));
            }
            else
            {
                matlabFile.write(byteArray);
            }
        }
        // First Block done!
        //QByteArray newline("\n");
        //matlabFile.write(newline);
    }

    // The remaining lines are numbers.
    QByteArray tempBytes;
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);
    tempBytes = worldFile.readLine(); matlabFile.write(tempBytes);

    matlabFile.close();
    worldFile.close();

    qDebug() << "Closed file.";
}



Q_DECLARE_METATYPE(SavesAccess*)

