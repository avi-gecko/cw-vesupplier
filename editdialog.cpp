#include "editdialog.h"
#include "ui_editdialog.h"
#include "vesuppliertablemodel.h"
#include <QPushButton>
#include <QSettings>
#include <QMessageBox>

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

    QRegularExpression nameOrganizationReg(R"(^[^\s].*[^\s]$)");
    m_nameOrganizationValidator = new QRegularExpressionValidator(nameOrganizationReg, this);
    ui->nameLineEdit->setValidator(m_nameOrganizationValidator);

    QRegularExpression OGRNReg(R"(^\d{13}$)");
    m_OGRNValidator = new QRegularExpressionValidator(OGRNReg, this);
    ui->OGRNLineEdit->setValidator(m_OGRNValidator);

    QRegularExpression addressReg(R"(^[^\s^,][^,]*[^\s^,],\s[^,]*[^\s^,],\s[^,]*[^\s^,],\s[^,]*[^\s^,]$)");
    m_addressValidator = new QRegularExpressionValidator(addressReg, this);
    ui->addressLineEdit->setValidator(m_addressValidator);

    QRegularExpression nameOwnerReg(R"(^[A-ZА-Я][a-zа-я]*\s[A-ZА-Я][a-zа-я]*(\s[A-ZА-Я])?[a-zа-я]*?$)");
    m_nameOwnerValidator = new QRegularExpressionValidator(nameOwnerReg, this);
    ui->nameOwnerLineEdit->setValidator(m_nameOwnerValidator);

    QRegularExpression phoneReg(R"(^\+?\d+\(\d{3}\)\d{3}-\d{2}-\d{2}$)");
    m_phoneValidator = new QRegularExpressionValidator(phoneReg, this);
    ui->phoneLineEdit->setValidator(m_phoneValidator);

    m_intValidator = new QIntValidator(0, INT_MAX, this);
    ui->countProductLineEdit->setValidator(m_intValidator);
    ui->countPostLineEdit->setValidator(m_intValidator);

    m_doubleValidator = new QDoubleValidator(0., __DBL_MAX__, 2, this);
    ui->priceLineEdit->setValidator(m_doubleValidator);
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::on_buttonBox_accepted()
{
    int pos = 0;
    QString name = ui->nameLineEdit->text();
    if (m_nameOrganizationValidator->validate(name, pos) != QValidator::Acceptable)
    {
        QMessageBox::about(this
                         , QString(tr("Organization name"))
                         , QString(tr("Organization name is required.")));
        return;
    }
    QString OGRN = ui->OGRNLineEdit->text();
    if (m_OGRNValidator->validate(OGRN, pos) != QValidator::Acceptable)
    {
        QMessageBox::about(this
                         , QString(tr("OGRN"))
                         , QString(tr("OGRN has to contain 13 digits.")));
        return;
    }
    QString address = ui->addressLineEdit->text();
    if (m_addressValidator->validate(address, pos) != QValidator::Acceptable)
    {
        QMessageBox::about(this
                         , QString(tr("Address"))
                         , QString(tr("Address should be matched like: city, street, house, office. Without spaces in the end")));
        return;
    }
    QString nameOwner = ui->nameOwnerLineEdit->text();
    if (m_nameOwnerValidator->validate(nameOwner, pos) != QValidator::Acceptable)
    {
        QMessageBox::about(this
                         , QString(tr("Owner name"))
                         , QString(tr("Owner name should be matched like: Last name *space* First name *space* Middle name <-- optional. Without spaces in the end")));
        return;
    }
    QString phone = ui->phoneLineEdit->text();
    if (m_phoneValidator->validate(phone, pos) != QValidator::Acceptable)
    {
        QMessageBox::about(this
                         , QString(tr("Phone"))
                         , QString(tr("Phone should be matched like: + or without X(XXX)XXX-XX-XX. Without spaces in the end")));
        return;
    }
    int countProduct = ui->countProductLineEdit->text().toInt();
    int countPost = ui->countPostLineEdit->text().toInt();
    QSettings settings("MGSU", "Database");
    settings.beginGroup("Language");
        QLocale locale(settings.value("locale").toLocale());
    settings.endGroup();
    bool ok;
    double price = locale.toDouble(ui->priceLineEdit->text(), &ok);


    m_model->setData(m_model->index(m_row, 0), QVariant(name),         Qt::EditRole);
    m_model->setData(m_model->index(m_row, 1), QVariant(OGRN),         Qt::EditRole);
    m_model->setData(m_model->index(m_row, 2), QVariant(address),      Qt::EditRole);
    m_model->setData(m_model->index(m_row, 3), QVariant(nameOwner),    Qt::EditRole);
    m_model->setData(m_model->index(m_row, 4), QVariant(phone),        Qt::EditRole);
    m_model->setData(m_model->index(m_row, 5), QVariant(countProduct), Qt::EditRole);
    m_model->setData(m_model->index(m_row, 6), QVariant(countPost),    Qt::EditRole);
    m_model->setData(m_model->index(m_row, 7), QVariant(price),        Qt::EditRole);
}


void EditDialog::on_buttonBox_rejected()
{
    this->close();
}

