#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include "vesuppliertablemodel.h"

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = nullptr, VesupplierTableModel* model = nullptr, int row = 0);
    ~EditDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::EditDialog *ui;
    VesupplierTableModel *m_model;
    int m_row;
};

#endif // EDITDIALOG_H
