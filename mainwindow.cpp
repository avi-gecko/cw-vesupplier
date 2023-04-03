#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vesupplier.h"
#include "vesuppliertablemodel.h"
#include "adddialog.h"
#include "editdialog.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QTableView>
#include <QSortFilterProxyModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
    connect(ui->about, &QMenu::aboutToShow, this, &MainWindow::about);
    connect(ui->open, &QAction::triggered, this, &MainWindow::open);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::close);
    connect(ui->close, &QAction::triggered, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about()
{
    QMessageBox::about(this
                     , QString(tr("About"))
                     , QString(tr("Created by Alexey Ilin ICTMS 2-5")));
}

void MainWindow::open()
{
    // Открытие файла
    QString file_name = QFileDialog::getOpenFileName();
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString header;
    header = in.readLine();


    if (header != QString("<DB>"))
    {

        QMessageBox::critical(this
                            , QString(tr("Error!"))
                            , QString(tr("Wrong file format.")));
        file.close();
        return;
    }


    //Создание таблицы
    QTableView* new_table =  new QTableView(ui->tabWidget);
    new_table->setSortingEnabled(true);
    VesupplierTableModel* model = new VesupplierTableModel(new_table);
    QSortFilterProxyModel* sort_model = new QSortFilterProxyModel(new_table);
    sort_model->setSourceModel(model);
    while (!in.atEnd())
    {
         QString result = in.readLine();
         QList splitted = result.split(QString(";"));

         Vesupplier* new_item = new Vesupplier(splitted.at(0)
                                             , splitted.at(1)
                                             , splitted.at(2)
                                             , splitted.at(3)
                                             , splitted.at(4)
                                             , splitted.at(5).toUInt()
                                             , splitted.at(6).toUInt()
                                             , splitted.at(7).toDouble());
         model->append(new_item);
    }
    new_table->setModel(sort_model);
    ui->tabWidget->addTab(new_table, file_name);
    file.close();
}

void MainWindow::close()
{
    QTableView* tab = qobject_cast<QTableView*>(ui->tabWidget->currentWidget());
    if (tab != nullptr)
        delete tab;
}


void MainWindow::on_addButton_clicked()
{

   QTableView* tab = qobject_cast<QTableView*>(ui->tabWidget->currentWidget());
    if (tab == nullptr)
    {
        QMessageBox::about(this
                         , QString(tr("Warning!"))
                         , QString(tr("You should open document before adding.")));
        return;
    }
    QSortFilterProxyModel* sort_model = qobject_cast<QSortFilterProxyModel*>(tab->model());
    VesupplierTableModel* model = dynamic_cast<VesupplierTableModel*>(sort_model->sourceModel());
    AddDialog addDialog(this, model);
    addDialog.exec();
}


void MainWindow::on_deleteButton_clicked()
{
    QTableView* tab = qobject_cast<QTableView*>(ui->tabWidget->currentWidget());
     if (tab == nullptr)
     {
         QMessageBox::about(this
                          , QString(tr("Warning!"))
                          , QString(tr("You should open document before deleting.")));
         return;
     }
     QSortFilterProxyModel* sort_model = qobject_cast<QSortFilterProxyModel*>(tab->model());
     VesupplierTableModel* model = dynamic_cast<VesupplierTableModel*>(sort_model->sourceModel());
     QModelIndex index = tab->currentIndex();
     QModelIndex source_index = sort_model->mapToSource(index);
     if (!source_index.isValid())
         return;
     int row = source_index.row();
     model->deleteRow(row);
}


void MainWindow::on_editButton_clicked()
{
     QTableView* tab = qobject_cast<QTableView*>(ui->tabWidget->currentWidget());
     if (tab == nullptr)
     {
         QMessageBox::about(this
                          , QString(tr("Warning!"))
                          , QString(tr("You should open document before deleting.")));
         return;
     }
     QSortFilterProxyModel* sort_model = qobject_cast<QSortFilterProxyModel*>(tab->model());
     VesupplierTableModel* model = dynamic_cast<VesupplierTableModel*>(sort_model->sourceModel());
     QModelIndex index = tab->currentIndex();
     QModelIndex source_index = sort_model->mapToSource(index);
     if (!source_index.isValid())
         return;
     int row = source_index.row();
     EditDialog editDialog(this, model, row);
     editDialog.exec();
}

