#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QRegularExpressionValidator>
#include <QPushButton>
#include <QSettings>
#include <QMessageBox>

#include "vesuppliermodel.h"

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = nullptr, VesupplierModel* model = nullptr, int row = 0);
    ~EditDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::EditDialog *ui;
    VesupplierModel *m_model;
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
