#include "vesuppliertablemodel.h"
#include <QApplication>
#include <QTranslator>

VesupplierTableModel::~VesupplierTableModel()
{
    qDeleteAll(m_values->begin(), m_values->end());
    delete m_values; 
}

VesupplierTableModel::VesupplierTableModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_values = new QList<Vesupplier*>();
}

int VesupplierTableModel::columnCount(const QModelIndex &parent) const
{
    return 8;
}

int VesupplierTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_values->size();
}

QVariant VesupplierTableModel::data(const QModelIndex &index, int role) const
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
                   value = m_values->at(index.row())->getNameOrganization();
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

QVariant VesupplierTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return QString(tr("Name"));
            case 1:
                return QString(tr("OGRN"));
            case 2:
                return QString(tr("Address"));
            case 3:
                return QString(tr("Owner name"));
            case 4:
                return QString(tr("Phone"));
            case 5:
                return QString(tr("Product count"));
            case 6:
                return QString(tr("Post count"));
            case 7:
                return QString(tr("Price"));
        }
    }
    return QVariant();
}

void VesupplierTableModel::append(Vesupplier* value)
{
    int newRow = rowCount(QModelIndex());
    beginInsertRows(QModelIndex(), newRow, newRow);
        m_values->append(value);
    endInsertRows();
    emit dataChanged(index(0, 0), index(rowCount(QModelIndex())-1, columnCount(QModelIndex())-1));
    emit layoutChanged();
}

void VesupplierTableModel::deleteRow(int idx)
{
    beginRemoveRows(QModelIndex(), idx, idx);
        delete m_values->at(idx);
        m_values->removeAt(idx);
    endRemoveRows();
    m_values->squeeze();
}

QModelIndex VesupplierTableModel::index(int row, int column) const
{
    if (row < 0 || row >= rowCount(QModelIndex()) || column < 0 || column >= columnCount(QModelIndex()))
    {
        return QModelIndex();
    }

    return createIndex(row, column);
}

bool VesupplierTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
        case 5: m_values->at(row)->setCountProduct(value.toUInt()); break;
        case 6: m_values->at(row)->setCountPost(value.toUInt()); break;
        case 7: m_values->at(row)->setPrice(value.toDouble());
    }

    emit dataChanged(index, index);
    return true;
}
