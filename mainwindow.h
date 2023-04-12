#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>
#include <QTranslator>
#include <QMenu>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
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
    void contextMenuEvent(QContextMenuEvent *e) override;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTranslator m_translator;
    QString m_lang;
    QLocale m_locale;
    QMenu m_menu;

private slots:
    void about();
    void open();
    void close();
    void save();
    void print();
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void on_findButton_clicked();
    void changeLangToRussian();
    void changeLangToEnglish();
    void on_graphicsButton_clicked();
};
#endif // MAINWINDOW_H
