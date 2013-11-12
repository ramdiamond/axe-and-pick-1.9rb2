#include "main.h"
#include "mainapp.h"
#include "neutralmoblistmodel.h"

// Initialize the counter
long NeutralMob::id_counter = 0;

NeutralMob::NeutralMob(const QString &type,
                       const float &posX,
                       const float &posY,
                       const float &posZ,
                       const float &rotation,
                       const QList<float> &unknown_floats,
                       const QBitArray &options,
                       const signed short &dirty,
                       const QString &rawData,
                       QObject * parent)
    : ListItem(parent),
      m_id(id_counter),
      m_type(type),
      m_posX(posX),
      m_posY(posY),
      m_posZ(posZ),
      m_rotation(rotation),
      m_unknown(unknown_floats),
      m_options(options),
      m_dirty(dirty),
      m_rawData(rawData)
{
    id_counter++;
}

NeutralMob * NeutralMob::build(QString & unitString)
{
    QStringList unitData = unitString.split('/');
    signed short dirty = 0;

    // Sanity, to see if we can edit this entry without problems
    // The "read-open" dirty flag of -1 will make this app a little more resistante to bugs caused by save file changes
    if (unitData.size() != 13)
        dirty = -1;

    // Load in the unknown floats
    QList<float> unknown_floats;
    for (int i=0; i<4; i++)
    {
        unknown_floats.append(unitData[i+5].toFloat());
    }

    // Load in the options
    QBitArray options(3, false);
    for (int i=0; i<3; i++)
    {
        options.setBit(i, unitData[i+9].compare("True") ? false : true);
    }

    const QString rawData = unitString;

    return (new NeutralMob( unitData[0],
                            unitData[1].toFloat(),
                            unitData[2].toFloat(),
                            unitData[3].toFloat(),
                            unitData[4].toFloat(),
                            unknown_floats,
                            options,
                            dirty,
                            rawData // Dono if new is needed here, but let's be safe and slow and mostly lazy :)
            ));

    // Won't need this anymore... I hope :)
    //return Q_NULLPTR;
}

/**
 * @brief Write the neutral mob data back to a file.
 */
void NeutralMob::writeToFile( QFile &unitFile )
{
    QTextStream unitStream(&unitFile);

    if ( (dirty() > 0) || (rawData() == Q_NULLPTR) ) { // dirty = 0 means not dirty, dirty < 0 means uneditable, dirty > 0 means we can probably edit this entry without issue
        unitStream << type() << "/"
                   << posX() << "/"
                   << posY() << "/"
                   << posZ() << "/"
                   << rotation() << "/";

        // Write all the unknown floats
        for (int i=0; i<4; i++)
        {
            unitStream << unknown_float(i) << "/";
        }

        // Write all the options
        for (int i=0; i<3; i++)
        {
            unitStream << QString(option(i)?"True":"False") << "/";
        }

    } else {
        unitStream << rawData();
    }

    unitStream << endl;
    unitStream.flush();
}

void NeutralMob::setType(QString type)
{
    m_type = type;

    if (m_dirty >= 0) m_dirty = 1;
    else {
        // TODO:
        // Let the user know his changes arn't going to be saved
    }
}

QHash<int, QByteArray> NeutralMob::roleNames() const
{
    QHash<int, QByteArray> names;

    names[IdRole] = "id";
    names[FilterStringRole] = "filterString";

    names[TypeRole] = "type";
    names[PosXRole] = "posX";
    names[PosYRole] = "posY";
    names[PosZRole] = "posZ";
    names[RotationRole] = "rotation";

    return names;
}

QVariant NeutralMob::data(int role) const
{
    switch(role) {
    case IdRole:
        return (unsigned int)id();
    case FilterStringRole:
        return filterString();

    case TypeRole:
        return type();
    case PosXRole:
        return posX();
    case PosYRole:
        return posY();
    case PosZRole:
        return posZ();
    case RotationRole:
        return rotation();
    default:
        return QVariant();
    }
}

QString NeutralMob::filterString() const
{
    std::stringstream completeString;

    completeString << type().toStdString();
    return (QString(completeString.str().c_str()));
}

void NeutralMob::print()
{
    qDebug() << type()
             << "x" << posX()
             << "y" << posY()
             << "z" << posZ()
             << "rotation" << rotation();
}

NeutralMobListModel::NeutralMobListModel(ListItem * prototype, QObject * parent )
    : ListModel(prototype, parent)
{
}

void NeutralMobListModel::setData(const long id, const QVariant &value, int role)
{
    switch (role)
    {
    case NeutralMob::TypeRole:
    {
        NeutralMob * item = (NeutralMob *)find( id );
        item->setType(value.toString());
        break;
    }
    default:
        qWarning() << "NeutralMobListModel::setData does not understand what role" << role << "is.";
        break;
    }
}

void NeutralMobListModel::remove(const long id)
{
    // Find the id of the item you want to delete,
    // get the index of that item, and remove it.
    removeRow(indexFromItem(find( id )).row());
}

// This function only exists because we can't parse the map file.
// TODO: change how units are placed.
float NeutralMobListModel::getFirstPosition(const char label)
{
    if (getList().size() != 0) {
        if (label==0) {
            return ((NeutralMob*)getList().first())->posX();
        }
        else if (label==1) {
            return ((NeutralMob*)getList().first())->posY();
        }
        else if (label==2) {
            return ((NeutralMob*)getList().first())->posZ();
        }
    }
    return 0.0;
}

void NeutralMobListModel::add(const QString type, float x, float y, float z)
{
    QList<float> unknown_floats;
    unknown_floats.append(0.4f);
    unknown_floats.append(0.0f);
    unknown_floats.append(0.0f);
    unknown_floats.append(0.0f);

    QBitArray options(3, false);

    appendRow(new NeutralMob(
                  type,
                  x, y, z,  // position
                  0.0,      // rotation
                  unknown_floats,
                  options,
                  1,
                  Q_NULLPTR
               ));

    qDebug() << "Added a mob of type" << type;
}

