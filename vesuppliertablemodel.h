#ifndef VESUPPLIERTABLEMODEL_H
#define VESUPPLIERTABLEMODEL_H

#include <QAbstractListModel>

class VesupplierTableModel : public QAbstractListModel
{
public:
    explicit VesupplierTableModel(QObject *parent = nullptr);
};

#endif // VESUPPLIERTABLEMODEL_H
