#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>
#include "vesuppliertablemodel.h"

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = nullptr, VesupplierTableModel *model = nullptr);
    ~AddDialog();

private:
    Ui::AddDialog *ui;
    VesupplierTableModel *m_model;
private slots:
    void on_buttonBox_accepted();
};

#endif // ADDDIALOG_H
