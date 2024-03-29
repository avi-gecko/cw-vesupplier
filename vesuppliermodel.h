#ifndef VESUPPLIERMODEL_H
#define VESUPPLIERMODEL_H

#include <QAbstractListModel>
#include <QMimeData>
#include "vesupplier.h"

/*!
 * \brief The VesupplierModel class
 *
 * Класс главной модели данных
 */
class VesupplierModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit VesupplierModel(QObject *parent = nullptr);
    ~VesupplierModel() override;

    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QModelIndex index(int row, int column) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    void append(Vesupplier* value);
    void deleteRow(int idx);
    QList<Vesupplier*> find(QString criteria, int column) const;

private:
    QList<Vesupplier*> *m_values;
};

#endif // VESUPPLIERMODEL_H
