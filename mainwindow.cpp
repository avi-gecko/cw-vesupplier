#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vesupplier.h"
#include "vesuppliertablemodel.h"
#include "adddialog.h"
#include "editdialog.h"
#include "finddialog.h"
#include "graphicsdialog.h"

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("MGSU", "Database");
    settings.beginGroup("MainWindowGeometry");
        resize(settings.value("size").toSize());
        move(settings.value("position").toPoint());
    settings.endGroup();
    settings.beginGroup("Language");
        m_lang = settings.value("lang", QVariant("ru_RU")).toString();
        m_locale = QLocale(m_lang);
        QLocale::setDefault(m_locale);
    settings.endGroup();
    const QString baseName = "cw-vesupplier_";
    if (m_translator.load(":/i18n/" + baseName + m_lang, ":/translations"))
    {
        qApp->installTranslator(&m_translator);
        ui->retranslateUi(this);
    }

    ui->tabWidget->clear();
    connect(ui->about, &QMenu::aboutToShow, this, &MainWindow::about);
    connect(ui->open, &QAction::triggered, this, &MainWindow::open);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::close);
    connect(ui->close, &QAction::triggered, this, &MainWindow::close);
    connect(ui->save, &QAction::triggered, this, &MainWindow::save);
    connect(ui->russian, &QAction::triggered, this, &MainWindow::changeLangToRussian);
    connect(ui->english, &QAction::triggered, this, &MainWindow::changeLangToEnglish);
    setAcceptDrops(true);
}

void MainWindow::MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    QList<QString> file_names;
    foreach (const QUrl &url, e->mimeData()->urls())
    {
        file_names.append(url.toLocalFile());
    }
    foreach (QString file_name, file_names)
    {
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
                                , QString(tr("Wrong file format.")) + QString("\n") + file_name);
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
                                                 , splitted.at(5).toInt()
                                                 , splitted.at(6).toInt()
                                                 , splitted.at(7).toDouble());
             model->append(new_item);
        }
        new_table->setModel(sort_model);
        ui->tabWidget->addTab(new_table, file_name);
        file.close();
    }
}

MainWindow::~MainWindow()
{
    QSettings settings("MGSU", "Database");
    settings.beginGroup("MainWindowGeometry");
        settings.setValue("size", size());
        settings.setValue("position", pos());
    settings.endGroup();
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
    QString file_name = QFileDialog::getOpenFileName(this
                                                   , QString(tr("Open file"))
                                                   , QDir::homePath()
                                                   , QString(tr("Database (*.db)")));
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
                                             , splitted.at(5).toInt()
                                             , splitted.at(6).toInt()
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

void MainWindow::save()
{
    QString file_name;
    if (ui->tabWidget->currentIndex() != -1)
        file_name = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    else
    {
        QMessageBox::about(this
                         , QString(tr("Warning!"))
                         , QString(tr("You should open document before saving.")));
        return;
    }



     QTableView* tab = qobject_cast<QTableView*>(ui->tabWidget->currentWidget());
     if (tab == nullptr)
     {
         QMessageBox::about(this
                          , QString(tr("Warning!"))
                          , QString(tr("You should open document before saving.")));
         return;
     }
     QSortFilterProxyModel* sort_model = qobject_cast<QSortFilterProxyModel*>(tab->model());
     VesupplierTableModel* model = static_cast<VesupplierTableModel*>(sort_model->sourceModel());

     QFile file(file_name);
     if (!file.open(QFile::WriteOnly | QFile::Truncate))
         return;

     QTextStream out(&file);
     out << "<DB>" << Qt::endl;
     int rows = model->rowCount(QModelIndex());
     for (int i = 0; i < rows; ++i)
     {
         out << model->data(model->index(i, 0), Qt::DisplayRole).toString() << ";"
             << model->data(model->index(i, 1), Qt::DisplayRole).toString() << ";"
             << model->data(model->index(i, 2), Qt::DisplayRole).toString() << ";"
             << model->data(model->index(i, 3), Qt::DisplayRole).toString() << ";"
             << model->data(model->index(i, 4), Qt::DisplayRole).toString() << ";"
             << model->data(model->index(i, 5), Qt::DisplayRole).toString() << ";"
             << model->data(model->index(i, 6), Qt::DisplayRole).toString() << ";"
             << model->data(model->index(i, 7), Qt::DisplayRole).toString() << ";" << Qt::endl;
     }
     file.close();
     QMessageBox::about(this
                      , QString(tr("Success!"))
                      , QString(tr("File is successfuly saved.")));
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
    VesupplierTableModel* model = static_cast<VesupplierTableModel*>(sort_model->sourceModel());
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
     VesupplierTableModel* model = static_cast<VesupplierTableModel*>(sort_model->sourceModel());
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
                          , QString(tr("You should open document before editing.")));
         return;
     }
     QSortFilterProxyModel* sort_model = qobject_cast<QSortFilterProxyModel*>(tab->model());
     VesupplierTableModel* model = static_cast<VesupplierTableModel*>(sort_model->sourceModel());
     QModelIndex index = tab->currentIndex();
     QModelIndex source_index = sort_model->mapToSource(index);
     if (!source_index.isValid())
         return;
     int row = source_index.row();
     EditDialog editDialog(this, model, row);
     editDialog.exec();
}


void MainWindow::on_findButton_clicked()
{
     QTableView* tab = qobject_cast<QTableView*>(ui->tabWidget->currentWidget());
     if (tab == nullptr)
     {
         QMessageBox::about(this
                          , QString(tr("Warning!"))
                          , QString(tr("You should open document before finding.")));
         return;
     }
     QSortFilterProxyModel* sort_model = qobject_cast<QSortFilterProxyModel*>(tab->model());
     VesupplierTableModel* model = static_cast<VesupplierTableModel*>(sort_model->sourceModel());
     FindDialog findDialog(this, model);
     findDialog.exec();

}

void MainWindow::changeLangToRussian()
{
    QSettings settings("MGSU", "Database");
    settings.beginGroup("Language");
        settings.setValue("lang", QVariant("ru_RU"));
    settings.endGroup();
    QMessageBox::about(this
                     , QString(tr("Change language"))
                       , QString(tr("Changes will be applied after restart.")));
}

void MainWindow::changeLangToEnglish()
{
    QSettings settings("MGSU", "Database");
    settings.beginGroup("Language");
        settings.setValue("lang", QVariant("en_US"));
    settings.endGroup();
    QMessageBox::about(this
                     , QString(tr("Change language"))
                       , QString(tr("Changes will be applied after restart.")));
}


void MainWindow::on_graphicsButton_clicked()
{
    QTableView* tab = qobject_cast<QTableView*>(ui->tabWidget->currentWidget());
    if (tab == nullptr)
    {
        QMessageBox::about(this
                         , QString(tr("Warning!"))
                         , QString(tr("You should open document before making graphics.")));
        return;
    }
    QSortFilterProxyModel* sort_model = qobject_cast<QSortFilterProxyModel*>(tab->model());
    VesupplierTableModel* model = static_cast<VesupplierTableModel*>(sort_model->sourceModel());
    QModelIndex index = tab->currentIndex();
    QModelIndex source_index = sort_model->mapToSource(index);
    if (!source_index.isValid())
        return;
    int column = source_index.column();
    if (column < 5)
    {
        QMessageBox::about(this
                         , QString(tr("Warning!"))
                         , QString(tr("You can't make graphics from text values.")));
        return;
    }
    GraphicsDialog graphicsDialog(this, model, column);
    graphicsDialog.exec();
}

