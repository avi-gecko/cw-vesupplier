#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include "vesuppliertablemodel.h"
#include <QButtonGroup>
#include <QTableView>
#include <QSortFilterProxyModel>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr, VesupplierTableModel *model = nullptr);
    ~FindDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::FindDialog *ui;
    VesupplierTableModel *m_model;
    QButtonGroup m_group;
    QTableView *m_newTable;
    VesupplierTableModel *m_newModel;
    QSortFilterProxyModel *m_newSortModel;
};

#endif // FINDDIALOG_H
