#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->about, &QMenu::aboutToShow, this, &MainWindow::about);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about()
{
    QMessageBox::about(this, QString(tr("О программе")), QString(tr("Создано Алексеем Ильиным ИЦТМС 2-5")));
}


void MainWindow::on_addButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName();
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString header;
    QStringList labels;
    header = in.readLine();
    if (header.startsWith(QString("<DB;")) and header.endsWith(QString(">")))
    {
        header = header.sliced(4, header.size() - 5);
        labels = header.split(";");
    }
    else
    {

        QMessageBox::critical(  this
                              , QString(tr("Ошибка!"))
                              , QString(tr("Неверный формат файла.")));
        file.close();
        return;
    }
    ui->textEdit->setText(header);

    ui->tableWidget->setColumnCount(labels.size());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
  /*  while (!in.atEnd())
    {
         result += in.readLine();
    }*/
   file.close();
}

