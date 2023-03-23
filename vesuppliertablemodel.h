#ifndef VESUPPLIERTABLEMODEL_H
#define VESUPPLIERTABLEMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "vesupplier.h"

class VesupplierTableModel : public QAbstractListModel
{
public:
    explicit VesupplierTableModel(QObject *parent = nullptr);

private:
    QList<Vesupplier> *m_values;
};

#endif // VESUPPLIERTABLEMODEL_H
