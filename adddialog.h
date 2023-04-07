#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>
#include "vesuppliertablemodel.h"
#include <QValidator>

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
    QRegularExpressionValidator *m_nameOrganizationValidator;
    QRegularExpressionValidator *m_OGRNValidator;
    QRegularExpressionValidator *m_addressValidator;
    QRegularExpressionValidator *m_nameOwnerValidator;
    QRegularExpressionValidator *m_phoneValidator;
    QIntValidator *m_intValidator;
    QDoubleValidator *m_doubleValidator;
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // ADDDIALOG_H
