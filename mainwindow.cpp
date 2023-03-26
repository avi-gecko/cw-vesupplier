#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vesupplier.h"
#include "vesuppliertablemodel.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QTableView>

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
    QStringList labels;
    header = in.readLine();

    // Проверка на формат
    if (header == QString("<DB;>"))
    {
        QMessageBox::critical(this
                            , QString(tr("Error!"))
                            , QString(tr("Table can't has columns.")));
        file.close();
        return;
    }
    if (header.startsWith(QString("<DB;")) and header.endsWith(QString(">")))
    {
        header = header.sliced(4, header.size() - 6);
        labels = header.split(";");
    }
    else
    {

        QMessageBox::critical(this
                            , QString(tr("Error!"))
                            , QString(tr("Wrong file format.")));
        file.close();
        return;
    }

    QTableView* new_table =  new QTableView(ui->tabWidget);
    ui->tabWidget->addTab(new_table, file_name);

    //Создание таблицы
    VesupplierTableModel* model = new VesupplierTableModel();
    new_table->setModel(model);
  /*  while (!in.atEnd())
    {
         result += in.readLine();
    }*/
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

}

