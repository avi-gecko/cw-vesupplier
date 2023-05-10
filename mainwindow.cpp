#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * \brief MainWindow::MainWindow
 * \param parent
 *
 * Конструктор главного окна, применяются настройки расположение и размера окна,
 * язык и локаль.
 * Ставится соответствующий язык.
 * Соединяются функиональные элементы.
 */
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
        qApp->installTranslator(&m_qtTranslator);
        m_qtTranslator.load("qt_" + m_lang + ".qm"
                          , QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        ui->retranslateUi(this);
    }


    ui->tabWidget->clear(); ///< Удаление стандартных вкладок
    setAcceptDrops(true); ///< Включение Drag and Drop
    QAction* action = m_menu.addAction(QString(tr("About"))); ///< Добавление контекстного меню по ПКМ

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

/*!
 * \brief MainWindow::MainWindow::dragEnterEvent
 * \param e
 *
 * Проверка типа при перетаскивании файла над главное окно
 */
void MainWindow::MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

/*!
 * \brief MainWindow::dropEvent
 * \param e
 *
 * Открытие файла при отпускании ЛКМ над главным окном
 */
void MainWindow::dropEvent(QDropEvent *e)
{
    QList<QString> file_names;
    foreach (const QUrl &url, e->mimeData()->urls()) ///< Получение адресов файлов
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

        QTableView* new_table =  new QTableView(ui->tabWidget);
        new_table->setSortingEnabled(true);
        VesupplierModel* model = new VesupplierModel(new_table);
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
        new_table->resizeColumnsToContents();
        ui->tabWidget->addTab(new_table, file_name);
        file.close();
    }
}

/*!
 * \brief MainWindow::contextMenuEvent
 * \param e
 *
 * Действие при нажатии ПКМ (открытие контекстного меню)
 */
void MainWindow::contextMenuEvent(QContextMenuEvent *e)
{
    m_menu.exec(e->globalPos());
}

/*!
 * \brief MainWindow::~MainWindow
 *
 * Запоминание настроек
 */
MainWindow::~MainWindow()
{
    QSettings settings("MGSU", "Database");
    settings.beginGroup("MainWindowGeometry");
        settings.setValue("size", size());
        settings.setValue("position", pos());
    settings.endGroup();
    delete ui;
}

/*!
 * \brief MainWindow::about
 *
 * Вкладка "О программе"
 */
void MainWindow::about()
{
    QMessageBox::about(this
                     , QString(tr("About"))
                     , QString(tr("Created by Alexey Ilin ICTMS 2-5")));
}

/*!
 * \brief MainWindow::open
 *
 * Открытие файла
 */
void MainWindow::open()
{

    QString file_name = QFileDialog::getOpenFileName(this
                                                   , QString(tr("Open file"))
                                                   , QDir::homePath()
                                                   , QString(tr("Database (*.db)"))); ///< Получение названия файла
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) ///< Открытие файла
        return;

    QTextStream in(&file);
    QString header;
    header = in.readLine();


    if (header != QString("<DB>")) ///< Проверка на соответствие формата
    {

        QMessageBox::critical(this
                            , QString(tr("Error!"))
                            , QString(tr("Wrong file format.")));
        file.close();
        return;
    }



    QTableView* new_table =  new QTableView(ui->tabWidget); ///< Создание таблицы
    new_table->setSortingEnabled(true); ///< Включение сортировки
    VesupplierModel* model = new VesupplierModel(new_table); ///< Создание модели
    QSortFilterProxyModel* sort_model = new QSortFilterProxyModel(new_table); ///< Создание модели сортировки
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
    new_table->resizeColumnsToContents();
    ui->tabWidget->addTab(new_table, file_name);
    file.close();
}

/*!
 * \brief MainWindow::close
 *
 * Закрытие вкладки. Очищение памяти
 */
void MainWindow::close()
{
    QTableView* tab = qobject_cast<QTableView*>(ui->tabWidget->currentWidget());
    if (tab != nullptr)
        delete tab;
}

/*!
 * \brief MainWindow::save
 *
 * Сохранение файла
 */
void MainWindow::save()
{
    QString file_name;
    if (ui->tabWidget->currentIndex() != -1) ///< Проверка на наличии вкладки(таблицы)
        file_name = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    else
    {
        QMessageBox::about(this
                         , QString(tr("Warning!"))
                         , QString(tr("You should open document before saving.")));
        return;
    }

    QTableView* tab = qobject_cast<QTableView*>(ui->tabWidget->currentWidget()); ///< Получение таблицы из вкладки
    QSortFilterProxyModel* sort_model = qobject_cast<QSortFilterProxyModel*>(tab->model()); ///< Получение модели сортировки из таблицы
    VesupplierModel* model = qobject_cast<VesupplierModel*>(sort_model->sourceModel()); ///< Получение модели из модели сортировки

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

/*!
 * \brief MainWindow::print
 *
 * Печать файла
 */
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
        VesupplierModel* model = qobject_cast<VesupplierModel*>(sort_model->sourceModel());
        QString strStream;
        QTextStream out(&strStream);
        out << "<html>\n"
            << "<header>\n"
            << "</header>\n"
            << "<body>\n"
            << "<table border=1>\n"
            << "<thead>\n"
            << "<tr>\n"; ///< Создание html кода для последущей печати таблицы
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
        document.setHtml(strStream);
        document.print(&printer);
    }
    delete printDialog;
}

/*!
 * \brief MainWindow::binaryOpen
 *
 * Бинарное открытие файла. Дополнительное задание
 */
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
    VesupplierModel* model = new VesupplierModel(new_table);
    QSortFilterProxyModel* sort_model = new QSortFilterProxyModel(new_table);
    sort_model->setSourceModel(model);

    int row_count;
    int column_count;
    in >> row_count >> column_count; ///< Считивание количества строк и столбцов

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
    new_table->resizeColumnsToContents();
    ui->tabWidget->addTab(new_table, file_name);
    file.close();
}

/*!
 * \brief MainWindow::binarySave
 *
 * Бинарное сохранение. Дополнительно задание
 */
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
    VesupplierModel* model = qobject_cast<VesupplierModel*>(sort_model->sourceModel());

    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return;

    QDataStream out(&file);
    out << QString("<DB>");
    int row_count = model->rowCount(QModelIndex());
    int column_count = model->columnCount(QModelIndex());

    out << row_count << column_count; ///< Сохранение количество строк и столбцов
    for (int row = 0; row < row_count; ++row)
    {
        for (int column = 0; column < column_count; ++column)
        {
            out << model->data(model->index(row, column), Qt::DisplayRole); ///< Запись объектов QVariant
        }
    }

    file.close();
    QMessageBox::about(this
                     , QString(tr("Success!"))
                       , QString(tr("File is successfuly saved.")));
}

/*!
 * \brief MainWindow::on_addButton_clicked
 *
 * Добавление записи
 */
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
    VesupplierModel* model = qobject_cast<VesupplierModel*>(sort_model->sourceModel());
    AddDialog addDialog(this, model);
    addDialog.exec();
}

/*!
 * \brief MainWindow::on_deleteButton_clicked
 *
 * Удаление записи
 */
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
     VesupplierModel* model = qobject_cast<VesupplierModel*>(sort_model->sourceModel());
     QModelIndex index = tab->currentIndex();
     QModelIndex source_index = sort_model->mapToSource(index);
     if (!source_index.isValid())
         return;
     int row = source_index.row();
     model->deleteRow(row);
}

/*!
 * \brief MainWindow::on_editButton_clicked
 *
 * Изменение записи
 */
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
     VesupplierModel* model = qobject_cast<VesupplierModel*>(sort_model->sourceModel());
     QModelIndex index = tab->currentIndex();
     QModelIndex source_index = sort_model->mapToSource(index);
     if (!source_index.isValid())
         return;
     int row = source_index.row();
     EditDialog editDialog(this, model, row);
     editDialog.exec();
}

/*!
 * \brief MainWindow::on_findButton_clicked
 *
 * Поиск записи
 */
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
     VesupplierModel* model = qobject_cast<VesupplierModel*>(sort_model->sourceModel());
     FindDialog findDialog(this, model);
     findDialog.exec();

}

/*!
 * \brief MainWindow::changeLangToRussian
 *
 * Смена языка на русский
 */
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
        m_qtTranslator.load("qt_ru_RU.qm"
                          , QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        ui->retranslateUi(this);
    }
}

/*!
 * \brief MainWindow::changeLangToEnglish
 *
 * Смена языка на английский
 */
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
        m_qtTranslator.load("qt_us_US.qm"
                          , QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        ui->retranslateUi(this);
    }
}

/*!
 * \brief MainWindow::on_graphicsButton_clicked
 *
 * Построение графиков
 */
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
    VesupplierModel* model = qobject_cast<VesupplierModel*>(sort_model->sourceModel());
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

