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
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QSettings>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QLibraryInfo>

#include "adddialog.h"
#include "editdialog.h"
#include "vesupplier.h"
#include "vesuppliermodel.h"
#include "finddialog.h"
#include "graphicsdialog.h"

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
    QTranslator m_qtTranslator;
    QString m_lang;
    QLocale m_locale;
    QMenu m_menu;

private slots:
    void about();
    void open();
    void close();
    void save();
    void print();
    void binaryOpen();
    void binarySave();
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void on_findButton_clicked();
    void changeLangToRussian();
    void changeLangToEnglish();
    void on_graphicsButton_clicked();
};
#endif // MAINWINDOW_H
