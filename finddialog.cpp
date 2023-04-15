#include "finddialog.h"
#include "ui_finddialog.h"

/*!
 * \brief FindDialog::FindDialog
 * \param parent
 * \param model
 *
 * Конструктор окна поиска. Добавление флагов RadioButton в ассоциативную группу
 */
FindDialog::FindDialog(QWidget *parent, VesupplierModel *model) :
    QDialog(parent),
    ui(new Ui::FindDialog),
    m_model(model),
    m_group(QButtonGroup()),
    m_newTable(nullptr),
    m_newModel(nullptr),
    m_newSortModel(nullptr)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(QString(tr("Ok")));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QString(tr("Cancel")));

    m_group.addButton(ui->nameRadioButton, 0);
    m_group.addButton(ui->OGRNRadioButton, 1);
    m_group.addButton(ui->addressRadioButton, 2);
    m_group.addButton(ui->nameOwnerRadioButton, 3);
    m_group.addButton(ui->phoneRadioButton, 4);
    m_group.addButton(ui->countProductRadioButton, 5);
    m_group.addButton(ui->countPostRadioButton, 6);
    m_group.addButton(ui->priceRadioButton, 7);
}

FindDialog::~FindDialog()
{
    if (m_newTable)
        delete m_newTable;
    delete ui;
}

/*!
 * \brief FindDialog::on_buttonBox_accepted
 *
 * Поиск данных и создание таблицы результатов
 */
void FindDialog::on_buttonBox_accepted()
{
    if (m_newTable)
    {
        delete m_newTable;
        m_newTable = nullptr;
    }
    int column = m_group.checkedId();
    if (column == -1)
    {
        QMessageBox::about(this
                         , QString(tr("Warning!"))
                         , QString(tr("You should choose column before finding.")));
        return;
    }
    QString criteria = ui->enterLineEdit->text();
    QList<Vesupplier*> result = m_model->find(criteria, column);
    if (result.isEmpty())
    {
        QMessageBox::about(this
                         , QString(tr("Not found!"))
                         , QString(tr("There is none of results.")));
        return;
    }
    m_newTable =  new QTableView(this);
    m_newTable->setSortingEnabled(true);
    m_newModel = new VesupplierModel(m_newTable);
    m_newSortModel = new QSortFilterProxyModel(m_newTable);
    m_newSortModel->setSourceModel(m_newModel);
    int rows = result.size();
    for (int i = 0; i < rows; ++i)
        m_newModel->append(new Vesupplier(result[i]));

    m_newTable->setModel(m_newSortModel);
    ui->tableLayout->insertWidget(2, m_newTable);

}

