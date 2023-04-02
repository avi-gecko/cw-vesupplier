#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vesupplier.h"
#include "vesuppliertablemodel.h"
#include "adddialog.h"

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
    while (!in.atEnd())
    {
         QString result = in.readLine();
         QList splitted = result.split(QString(";"));

         Vesupplier new_item(splitted.at(0)
                           , splitted.at(1)
                           , splitted.at(2)
                           , splitted.at(3)
                           , splitted.at(4)
                           , splitted.at(5).toInt()
                           , splitted.at(6).toInt()
                           , splitted.at(7).toInt());
         model->append(new_item);
    }
    sort_model->setSourceModel(model);
    new_table->setModel(sort_model);
    ui->tabWidget->addTab(new_table, file_name);
    file.close();
}

void MainWindow::close()
{
    QTableView* tab = dynamic_cast<QTableView*>(ui->tabWidget->currentWidget());
    QAbstractItemModel* model = tab->model();
    if (tab != nullptr)
        delete tab;
}


void MainWindow::on_addButton_clicked()
{
    AddDialog dialog(this);
    dialog.exec();
}

