#include "savedgamelistmodel.h"

#include <QDebug>
#include <sstream>

long SavedGame::id_counter = 0;

SavedGame::SavedGame(const QString &gameName,
                     const QString &lastPlayed,
                     const QString &worldSize,
                     const long &day,
                     const long &unitNumber,
                     const bool &storeInSaves,
                     QObject * parent)
    : ListItem(parent),
      m_name(gameName),
      m_date(lastPlayed),
      m_worldSize(worldSize),
      m_day(day),
      m_unitNumber(unitNumber),
      m_bStoreInSaves(storeInSaves),
      m_id(id_counter) // Gawd dang, don't forget about this variable in other list objects, or the QML scripts will give you random values for id!!!
{
    id_counter++;
}

void SavedGame::setDay(long newDay)
{
    if(m_day != newDay)
    {
        m_day = newDay;
        emit dataChanged();
    }
}
void SavedGame::setUnitNumber(long newUnitNumber)
{
    if(m_unitNumber != newUnitNumber)
    {
        m_unitNumber = newUnitNumber;
        emit dataChanged();
    }
}
void SavedGame::setStoreInSaves(bool newStoreInSaves)
{
    // TODO: store this into saves.sav or remove from saves.sav
    if(m_bStoreInSaves != newStoreInSaves)
    {
        m_bStoreInSaves = newStoreInSaves;
        emit dataChanged();
    }
}

QHash<int, QByteArray> SavedGame::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[DateRole] = "date";
    names[WorldSize] = "worldSize";
    names[DayRole] = "day";
    names[UnitNumberRole] = "unitNumber";
    names[StoreInSavesRole] = "storeInSaves";
    names[IdRole] = "id";
    names[FilterStringRole] = "filterString";

    return names;
}

QVariant SavedGame::data(int role) const
{
    switch(role) {
    case NameRole:
        return name();
    case DateRole:
        return date();
    case WorldSize:
        return worldSize();
    case DayRole:
        return (int)day();
    case UnitNumberRole:
        return (int)unitNumber();
    case StoreInSavesRole:
        return (bool)storeInSaves();
    case IdRole:
        return (int)id();
    case FilterStringRole:
        return filterString();
    default:
        return QVariant();
    }
}

QString SavedGame::filterString() const
{
    std::stringstream completeString;

    completeString << name().toStdString()
                   << date().toStdString()
                   << worldSize().toStdString()
                   << day()
                   << unitNumber();
    return (QString(completeString.str().c_str()));
}

SavedGameListModel::SavedGameListModel(ListItem * prototype, QObject * parent )
    : ListModel(prototype, parent)
{
}

void SavedGameListModel::setData(const long identification, const QVariant &value, int role)
{
    switch (role)
    {
    case SavedGame::DayRole:
    {
        SavedGame * item = (SavedGame *)find(identification);
        item->setDay(value.toLongLong());
        break;
    }
    case SavedGame::UnitNumberRole:
    {
        SavedGame * item = (SavedGame *)find(identification);
        item->setUnitNumber(value.toLongLong());
        break;
    }
    case SavedGame::StoreInSavesRole:
    {
        SavedGame * item = (SavedGame *)find(identification);
        item->setStoreInSaves(value.toBool());
        break;
    }
    default:
        qWarning() << "SavedGameListModel::setData does not understand what role" << role << "is.";
        break;
    }
}

void SavedGameListModel::invertStoreInSaves(const long id) {
    SavedGame * item = (SavedGame *)find(id);
    item->setStoreInSaves(!item->getStoreInSaves());
}
