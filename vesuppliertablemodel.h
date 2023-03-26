#ifndef VESUPPLIERTABLEMODEL_H
#define VESUPPLIERTABLEMODEL_H

#include <QAbstractListModel>
#include "vesupplier.h"

class VesupplierTableModel : public QAbstractListModel
{
public:
    ~VesupplierTableModel() override;
    explicit VesupplierTableModel(QObject *parent = nullptr);
    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void append(const Vesupplier value);
private:
    QList<Vesupplier> *m_values;
};

#endif // VESUPPLIERTABLEMODEL_H
