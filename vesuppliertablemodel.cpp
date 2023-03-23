#include "vesuppliertablemodel.h"

VesupplierTableModel::VesupplierTableModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_values = new QList<Vesupplier>();
}

int VesupplierTableModel::columnCount(const QModelIndex &parent) const
{
    return 8;
}

int VesupplierTableModel::rowCount(const QModelIndex &parent) const
{
    return m_values->count();
}
