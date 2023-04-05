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
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void append(Vesupplier* value);
    void deleteRow(int idx);
    QModelIndex index(int row, int column) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QList<Vesupplier*> find(QString criteria, int column) const;
private:
    QList<Vesupplier*> *m_values;
};

#endif // VESUPPLIERTABLEMODEL_H
