#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>
#include "adddialog.h"
#include "editdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void about();
    void open();
    void close();
    void save();
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_editButton_clicked();
};
#endif // MAINWINDOW_H
