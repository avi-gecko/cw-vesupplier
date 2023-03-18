#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

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

