#include "vesuppliermodel.h"

/*!
 * \brief VesupplierModel::VesupplierModel
 * \param parent
 *
 * Конструктор модели. Выделение памяти под QList
 */
VesupplierModel::VesupplierModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_values = new QList<Vesupplier*>();
}

/*!
 * \brief VesupplierModel::~VesupplierModel
 *
 * Деструктор модели. Удаление данных из QList
 */
VesupplierModel::~VesupplierModel()
{
    qDeleteAll(m_values->begin(), m_values->end());
    delete m_values;
}

int VesupplierModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 8;
}

int VesupplierModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_values->size();
}

/*!
 * \brief VesupplierModel::data
 * \param index
 * \param role
 * \return
 *
 * Метод получения данных в зависимости от роли
 */
QVariant VesupplierModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    switch(role)
    {
        case Qt::DisplayRole:
        {
            switch(index.column())
            {
                case 0:
                {
                   value = m_values->at(index.row())->getNameOrganization();
                   break;
                }
                case 1:
                {
                   value = m_values->at(index.row())->getOGRN();
                   break;
                }
                case 2:
                {
                   value = m_values->at(index.row())->getAddress();
                   break;
                }
                case 3:
                {
                   value = m_values->at(index.row())->getNameOwner();
                   break;
                }
                case 4:
                {
                   value = m_values->at(index.row())->getPhone();
                   break;
                }
                case 5:
                {
                   value = m_values->at(index.row())->getCountProduct();
                   break;
                }
                case 6:
                {
                   value = m_values->at(index.row())->getCountPost();
                   break;
                }
                case 7:
                {
                   value = m_values->at(index.row())->getPrice();
                   break;
                }
            }
            break;
        }
    }
    return value;
}

/*!
 * \brief VesupplierModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 *
 * Метод получение заголовков таблицы
 */
QVariant VesupplierModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return tr("Name");
            case 1:
                return tr("OGRN");
            case 2:
                return tr("Address");
            case 3:
                return tr("Owner name");
            case 4:
                return tr("Phone");
            case 5:
                return tr("Product count");
            case 6:
                return tr("Post count");
            case 7:
                return tr("Price");
        }
    }
    if (role == Qt::DisplayRole && orientation == Qt::Vertical)
    {
        return section + 1;
    }
    return QVariant();
}

/*!
 * \brief VesupplierModel::append
 * \param value
 *
 * Метод добавления записи
 */
void VesupplierModel::append(Vesupplier* value)
{
    int newRow = rowCount(QModelIndex());
    beginInsertRows(QModelIndex(), newRow, newRow);
        m_values->append(value);
    endInsertRows();
}

/*!
 * \brief VesupplierModel::deleteRow
 * \param idx
 *
 * Метод удаления записи
 */
void VesupplierModel::deleteRow(int idx)
{
    beginRemoveRows(QModelIndex(), idx, idx);
        delete m_values->at(idx);
        m_values->removeAt(idx);
    endRemoveRows();
    m_values->squeeze();
}

/*!
 * \brief VesupplierModel::index
 * \param row
 * \param column
 * \return
 *
 * Метод получения индекса
 */
QModelIndex VesupplierModel::index(int row, int column) const
{
    if (row < 0 or row >= rowCount(QModelIndex()) or column < 0 or column >= columnCount(QModelIndex()))
    {
        return QModelIndex();
    }

    return createIndex(row, column);
}

/*!
 * \brief VesupplierModel::setData
 * \param index
 * \param value
 * \param role
 * \return
 *
 * Метод изменения данных
 */
bool VesupplierModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() or role != Qt::EditRole)
    {
        return false;
    }

    int row = index.row();
    if (row < 0 or row >= rowCount(QModelIndex()))
    {
        return false;
    }

    switch(index.column())
    {
        case 0: m_values->at(row)->setNameOrganization(value.toString()); break;
        case 1: m_values->at(row)->setOGRN(value.toString()); break;
        case 2: m_values->at(row)->setAddress(value.toString()); break;
        case 3: m_values->at(row)->setNameOwner(value.toString()); break;
        case 4: m_values->at(row)->setPhone(value.toString()); break;
        case 5: m_values->at(row)->setCountProduct(value.toInt()); break;
        case 6: m_values->at(row)->setCountPost(value.toInt()); break;
        case 7: m_values->at(row)->setPrice(value.toDouble());
    }

    emit dataChanged(index, index);
    return true;
}

/*!
 * \brief VesupplierModel::find
 * \param criteria
 * \param column
 * \return
 *
 * Метод поиска данных
 */
QList<Vesupplier*> VesupplierModel::find(QString criteria, int column) const
{
    QList<Vesupplier*> result;
    int rows = rowCount(QModelIndex());
    for (int i = 0; i < rows; ++i)
    {
        QVariant value;
        switch(column)
        {
            case 0:
            {
               value = m_values->at(i)->getNameOrganization();
               break;
            }
            case 1:
            {
               value = m_values->at(i)->getOGRN();
               break;
            }
            case 2:
            {
               value = m_values->at(i)->getAddress();
               break;
            }
            case 3:
            {
               value = m_values->at(i)->getNameOrganization();
               break;
            }
            case 4:
            {
               value = m_values->at(i)->getPhone();
               break;
            }
            case 5:
            {
               value = m_values->at(i)->getCountProduct();
               break;
            }
            case 6:
            {
               value = m_values->at(i)->getCountPost();
               break;
            }
            case 7:
            {
               value = m_values->at(i)->getPrice();
               break;
            }
        }
        if (value.toString() == criteria)
        {
            result.append(m_values->at(i));
        }
    }
    return result;
}
