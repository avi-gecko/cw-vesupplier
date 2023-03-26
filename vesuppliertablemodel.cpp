#include "vesuppliertablemodel.h"
#include <QApplication>
#include <QTranslator>

VesupplierTableModel::~VesupplierTableModel()
{
    delete m_values; 
    throw std::exception();
}

VesupplierTableModel::VesupplierTableModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_values = new QList<Vesupplier>();
    m_values->append(Vesupplier(QString("Test")
                              , QString("Test")
                              , QString("Test")
                              , QString("Test")
                              , QString("Test")
                              , 100
                              , 100
                              , 100.0));
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
                   value = m_values->at(index.row()).getOGRN();
                   break;
                }
                case 2:
                {
                   value = m_values->at(index.row()).getAddress();
                   break;
                }
                case 3:
                {
                   value = m_values->at(index.row()).getNameOrganization();
                   break;
                }
                case 4:
                {
                   value = m_values->at(index.row()).getPhone();
                   break;
                }
                case 5:
                {
                   value = m_values->at(index.row()).getCountProduct();
                   break;
                }
                case 6:
                {
                   value = m_values->at(index.row()).getCountPost();
                   break;
                }
                case 7:
                {
                   value = m_values->at(index.row()).getPrice();
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
                return QString(tr("Name owner"));
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
