#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QPushButton>

#include "vesuppliermodel.h"

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr, VesupplierModel *model = nullptr);
    ~FindDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::FindDialog *ui;
    VesupplierModel *m_model;
    QButtonGroup m_group;
    QTableView *m_newTable;
    VesupplierModel *m_newModel;
    QSortFilterProxyModel *m_newSortModel;
};

#endif // FINDDIALOG_H
