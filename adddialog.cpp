#include "adddialog.h"
#include "ui_adddialog.h"

/*!
 * \brief AddDialog::AddDialog
 * \param parent
 * \param model
 *
 * Конктрустор окна добавление. Создание валидаторов и регулярных выражений
 */
AddDialog::AddDialog(QWidget *parent, VesupplierModel *model) :
    QDialog(parent),
    ui(new Ui::AddDialog),
    m_model(model)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(QString(tr("Ok")));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QString(tr("Cancel")));

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
    ui->ownerNameLineEdit->setValidator(m_nameOwnerValidator);

    QRegularExpression phoneReg(R"(^\+?\d+\(\d{3}\)\d{3}-\d{2}-\d{2}$)");
    m_phoneValidator = new QRegularExpressionValidator(phoneReg, this);
    ui->phoneLineEdit->setValidator(m_phoneValidator);

    m_intValidator = new QIntValidator(0, INT_MAX, this);
    ui->productCountLineEdit->setValidator(m_intValidator);
    ui->postCountlineEdit->setValidator(m_intValidator);

    m_doubleValidator = new QDoubleValidator(0., __DBL_MAX__, 2, this);
    ui->priceLineEdit->setValidator(m_doubleValidator);
}

AddDialog::~AddDialog()
{
    delete ui;
}

/*!
 * \brief AddDialog::on_buttonBox_accepted
 *
 * Проверка вводимых данных. Добавление, если проверки пройдены
 */
void AddDialog::on_buttonBox_accepted()
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
    QString nameOwner = ui->ownerNameLineEdit->text();
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
    int productCount = ui->productCountLineEdit->text().toInt();
    int postCount = ui->postCountlineEdit->text().toInt();
    QSettings settings("MGSU", "Database");
    settings.beginGroup("Language");
        QLocale locale(settings.value("locale").toLocale());
    settings.endGroup();
    bool ok;
    double price = locale.toDouble(ui->priceLineEdit->text(), &ok);

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

