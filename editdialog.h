#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include "vesuppliertablemodel.h"
#include <QRegularExpressionValidator>

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
    QRegularExpressionValidator *m_nameOrganizationValidator;
    QRegularExpressionValidator *m_OGRNValidator;
    QRegularExpressionValidator *m_addressValidator;
    QRegularExpressionValidator *m_nameOwnerValidator;
    QRegularExpressionValidator *m_phoneValidator;
    QIntValidator *m_intValidator;
    QDoubleValidator *m_doubleValidator;
    int m_row;
};

#endif // EDITDIALOG_H
