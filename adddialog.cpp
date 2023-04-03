#include "adddialog.h"
#include "ui_adddialog.h"
#include "vesupplier.h"
#include "vesuppliertablemodel.h"

AddDialog::AddDialog(QWidget *parent, VesupplierTableModel *model) :
    QDialog(parent),
    ui(new Ui::AddDialog),
    m_model(model)
{
    ui->setupUi(this);
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::on_buttonBox_accepted()
{
    QString name = ui->nameLineEdit->text();
    QString OGRN = ui->OGRNLineEdit->text();
    QString address = ui->addressLineEdit->text();
    QString nameOwner = ui->ownerNameLineEdit->text();
    QString phone = ui->phoneLineEdit->text();
    unsigned int productCount = ui->productCountLineEdit->text().toUInt();
    unsigned int postCount = ui->postCountlineEdit->text().toUInt();
    double price = ui->priceLineEdit->text().toDouble();
    Vesupplier* new_item = new Vesupplier(name
                                        , OGRN
                                        , address
                                        , nameOwner
                                        , phone
                                        , productCount
                                        , postCount
                                        , price);
    m_model->append(new_item);
}


void AddDialog::on_buttonBox_rejected()
{
    this->close();
}

