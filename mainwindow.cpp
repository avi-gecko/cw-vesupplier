#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QTableWidget>

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
                     , QString(tr("О программе"))
                     , QString(tr("Создано Алексеем Ильиным ИЦТМС 2-5")));
}

void MainWindow::open()
{
    // Получение имени файла
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
                            , QString(tr("Ошибка!"))
                            , QString(tr("Таблица не может не иметь столбцов.")));
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
                            , QString(tr("Ошибка!"))
                            , QString(tr("Неверный формат файла.")));
        file.close();
        return;
    }

    QTableWidget* new_table =  new QTableWidget(this);
    ui->tabWidget->addTab(new_table, file_name);

    //Создание таблицы
    new_table->setColumnCount(labels.size());
    new_table->setHorizontalHeaderLabels(labels);
  /*  while (!in.atEnd())
    {
         result += in.readLine();
    }*/
   file.close();
}

void MainWindow::close()
{
    QWidget* tab = ui->tabWidget->currentWidget();
    if (tab != nullptr)
        delete tab;
}


void MainWindow::on_addButton_clicked()
{

}

