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
    , m_menu(QMenu(this))
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
    setAcceptDrops(true);
    QAction* action = m_menu.addAction(QString(tr("About")));

    connect(ui->binaryOpen, &QAction::triggered, this, &MainWindow::binaryOpen);
    connect(ui->binarySave, &QAction::triggered, this, &MainWindow::binarySave);
    connect(action, &QAction::triggered, this, &MainWindow::about);
    connect(ui->about, &QMenu::aboutToShow, this, &MainWindow::about);
    connect(ui->open, &QAction::triggered, this, &MainWindow::open);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::close);
    connect(ui->close, &QAction::triggered, this, &MainWindow::close);
    connect(ui->save, &QAction::triggered, this, &MainWindow::save);
    connect(ui->russian, &QAction::triggered, this, &MainWindow::changeLangToRussian);
    connect(ui->english, &QAction::triggered, this, &MainWindow::changeLangToEnglish);
    connect(ui->print, &QAction::triggered, this, &MainWindow::print);

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

void MainWindow::contextMenuEvent(QContextMenuEvent *e)
{
    m_menu.exec(e->globalPos());
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

void MainWindow::print()
{
    QTableView* tab = qobject_cast<QTableView*>(ui->tabWidget->currentWidget());
    if (tab == nullptr)
    {
        QMessageBox::about(this
                         , QString(tr("Warning!"))
                         , QString(tr("You should open document before printing.")));
        return;
    }
    QPrinter printer(QPrinter::PrinterResolution);
    QPrintDialog *printDialog = new QPrintDialog(&printer);
    printer.setFullPage(true);

    if (printDialog->exec() == QDialog::Accepted)
    {
        QSortFilterProxyModel* sort_model = qobject_cast<QSortFilterProxyModel*>(tab->model());
        VesupplierTableModel* model = static_cast<VesupplierTableModel*>(sort_model->sourceModel());
        QString strStream;
        QTextStream out(&strStream);
        out << "<html>\n"
            << "<header>\n"
            << "</header>\n"
            << "<body>\n"
            << "<table border=1>\n"
            << "<thead>\n"
            << "<tr>\n";
        int columns = 8;
        for (int i = 0; i < columns; ++i)
        {
            out << "<td>"
                <<  model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString()
                << "</td>\n";
        }
        out << "</tr>\n"
            << "</thead>\n"
            << "<tbody>\n";
        int rows = model->rowCount(QModelIndex());
        for (int i = 0; i < rows; ++i)
        {
            out << "<tr>\n"
                << "<td>"
                << model->data(model->index(i, 0), Qt::DisplayRole).toString()
                << "</td>\n"
                << "</td>\n"
                << "<td>"
                << model->data(model->index(i, 1), Qt::DisplayRole).toString()
                << "</td>\n"
                << "</td>\n"
                << "<td>"
                << model->data(model->index(i, 2), Qt::DisplayRole).toString()
                << "</td>\n"
                << "</td>\n"
                << "<td>"
                << model->data(model->index(i, 3), Qt::DisplayRole).toString()
                << "</td>\n"
                << "</td>\n"
                << "<td>"
                << model->data(model->index(i, 4), Qt::DisplayRole).toString()
                << "</td>\n"
                << "</td>\n"
                << "<td>"
                << model->data(model->index(i, 5), Qt::DisplayRole).toString()
                << "</td>\n"
                << "</td>\n"
                << "<td>"
                << model->data(model->index(i, 6), Qt::DisplayRole).toString()
                << "</td>\n"
                << "</td>\n"
                << "<td>"
                << model->data(model->index(i, 7), Qt::DisplayRole).toString()
                << "</td>\n"
                << "</td>\n"
                << "</tr>\n";
        }
        out << "</tbody>\n"
            << "</table>\n"
            << "</html>";
        QTextDocument document;
        qDebug() << strStream;
        document.setHtml(strStream);
        document.print(&printer);
    }
    delete printDialog;
}

void MainWindow::binaryOpen()
{
    QString file_name = QFileDialog::getOpenFileName(this
                                                   , QString(tr("Open file"))
                                                   , QDir::homePath()
                                                   , QString(tr("Database (*.db)")));
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&file);
    QString header;
    in >> header;
    if (header != QString("<DB>"))
    {

        QMessageBox::critical(this
                            , QString(tr("Error!"))
                            , QString(tr("Wrong file format.")));
        file.close();
        return;
    }
    QTableView* new_table =  new QTableView(ui->tabWidget);
    new_table->setSortingEnabled(true);
    VesupplierTableModel* model = new VesupplierTableModel(new_table);
    QSortFilterProxyModel* sort_model = new QSortFilterProxyModel(new_table);
    sort_model->setSourceModel(model);

    int row_count;
    int column_count;
    in >> row_count >> column_count;

    for (int row = 0; row < row_count; ++row)
    {
        QVariant data[column_count];
        for (int column = 0; column < column_count; ++column)
        {
            in >> data[column];
        }
        Vesupplier* new_item = new Vesupplier(data[0].toString()
                                            , data[1].toString()
                                            , data[2].toString()
                                            , data[3].toString()
                                            , data[4].toString()
                                            , data[5].toInt()
                                            , data[6].toInt()
                                            , data[7].toDouble());
        model->append(new_item);
    }
    new_table->setModel(sort_model);
    ui->tabWidget->addTab(new_table, file_name);
    file.close();
}

void MainWindow::binarySave()
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
                         , QString(tr("You should open document before adding.")));
        return;
    }
    QSortFilterProxyModel* sort_model = qobject_cast<QSortFilterProxyModel*>(tab->model());
    VesupplierTableModel* model = static_cast<VesupplierTableModel*>(sort_model->sourceModel());

    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return;

    QDataStream out(&file);
    out << QString("<DB>");
    int row_count = model->rowCount(QModelIndex());
    int column_count = model->columnCount(QModelIndex());

    out << row_count << column_count;
    for (int row = 0; row < row_count; ++row)
    {
        for (int column = 0; column < column_count; ++column)
        {
            out << model->data(model->index(row, column), Qt::DisplayRole);
        }
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
    const QString baseName = "cw-vesupplier_";
    if (m_translator.load(":/i18n/" + baseName + "ru_RU", ":/translations"))
    {
        qApp->installTranslator(&m_translator);
        ui->retranslateUi(this);
    }
}

void MainWindow::changeLangToEnglish()
{
    QSettings settings("MGSU", "Database");
    settings.beginGroup("Language");
        settings.setValue("lang", QVariant("en_US"));
    settings.endGroup();
    const QString baseName = "cw-vesupplier_";
    if (m_translator.load(":/i18n/" + baseName + "ru_RU", ":/translations"))
    {
        qApp->removeTranslator(&m_translator);
        ui->retranslateUi(this);
    }
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

