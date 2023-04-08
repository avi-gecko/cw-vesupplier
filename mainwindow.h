#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>
#include <QTranslator>
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
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dropEvent(QDropEvent *e) override;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTranslator m_translator;
    QString m_lang;
    QLocale m_locale;

private slots:
    void about();
    void open();
    void close();
    void save();
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void on_findButton_clicked();
    void changeLangToRussian();
    void changeLangToEnglish();
};
#endif // MAINWINDOW_H
