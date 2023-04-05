#include "editdialog.h"
#include "ui_editdialog.h"
#include "vesuppliertablemodel.h"
#include <QPushButton>

EditDialog::EditDialog(QWidget *parent, VesupplierTableModel* model, int row) :
    QDialog(parent),
    ui(new Ui::EditDialog),
    m_model(model),
    m_row(row)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(QString(tr("Ok")));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QString(tr("Cancel")));

    ui->nameLineEdit->setText(model->data(m_model->index(row, 0), Qt::DisplayRole).toString());
    ui->OGRNLineEdit->setText(model->data(m_model->index(row, 1), Qt::DisplayRole).toString());
    ui->addressLineEdit->setText(model->data(m_model->index(row, 2), Qt::DisplayRole).toString());
    ui->nameOwnerLineEdit->setText(model->data(m_model->index(row, 3), Qt::DisplayRole).toString());
    ui->phoneLineEdit->setText(model->data(m_model->index(row, 4), Qt::DisplayRole).toString());
    ui->countProductLineEdit->setText(model->data(m_model->index(row, 5), Qt::DisplayRole).toString());
    ui->countPostLineEdit->setText(model->data(m_model->index(row, 6), Qt::DisplayRole).toString());
    ui->priceLineEdit->setText(model->data(m_model->index(row, 7), Qt::DisplayRole).toString());
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::on_buttonBox_accepted()
{
    m_model->setData(m_model->index(m_row, 0), QVariant(ui->nameLineEdit->text()),         Qt::EditRole);
    m_model->setData(m_model->index(m_row, 1), QVariant(ui->OGRNLineEdit->text()),         Qt::EditRole);
    m_model->setData(m_model->index(m_row, 2), QVariant(ui->addressLineEdit->text()),      Qt::EditRole);
    m_model->setData(m_model->index(m_row, 3), QVariant(ui->nameOwnerLineEdit->text()),    Qt::EditRole);
    m_model->setData(m_model->index(m_row, 4), QVariant(ui->phoneLineEdit->text()),        Qt::EditRole);
    m_model->setData(m_model->index(m_row, 5), QVariant(ui->countProductLineEdit->text()), Qt::EditRole);
    m_model->setData(m_model->index(m_row, 6), QVariant(ui->countPostLineEdit->text()),    Qt::EditRole);
    m_model->setData(m_model->index(m_row, 7), QVariant(ui->priceLineEdit->text()),        Qt::EditRole);
}


void EditDialog::on_buttonBox_rejected()
{
    this->close();
}

