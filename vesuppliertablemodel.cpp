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
                   value = m_values->at(index.row()).getNameOrganization();
                   break;
                }
                case 1:
                {
                   value = m_values->at(index.row()).getNameOrganization();
                   break;
                }
                case 2:
                {
                   value = m_values->at(index.row()).getNameOrganization();
                   break;
                }
                case 3:
                {
                   value = m_values->at(index.row()).getNameOrganization();
                   break;
                }
                case 4:
                {
                   value = m_values->at(index.row()).getNameOrganization();
                   break;
                }
                case 5:
                {
                   value = m_values->at(index.row()).getNameOrganization();
                   break;
                }
                case 6:
                {
                   value = m_values->at(index.row()).getNameOrganization();
                   break;
                }
                case 7:
                {
                   value = m_values->at(index.row()).getNameOrganization();
                   break;
                }
            }
            break;
        }
    }
    return value;
}
