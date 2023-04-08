#include "graphicsdialog.h"
#include "ui_graphicsdialog.h"

GraphicsDialog::GraphicsDialog(QWidget *parent, VesupplierTableModel *model, int column) :
    QDialog(parent),
    ui(new Ui::GraphicsDialog),
    m_model(model),
    m_chart(QChart())
{
    ui->setupUi(this); 
    QBarSeries *series = new QBarSeries(this);
    int rows = m_model->rowCount(QModelIndex());
    for (int i = 0; i < rows; ++i)
    {
        QBarSet *set = new QBarSet(m_model->data(m_model->index(i, 0), Qt::DisplayRole).toString(), this);
        *set << m_model->data(m_model->index(i, column), Qt::DisplayRole).toInt();
        series->append(set);
    }
    m_chart.addSeries(series);
    QChartView *chart_view = new QChartView(&m_chart, this);
    QString title = m_model->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();
    m_chart.setTitle(title);
    m_chart.legend()->setAlignment(Qt::AlignLeft);
    ui->verticalLayout_2->addWidget(chart_view);
}

GraphicsDialog::~GraphicsDialog()
{
    delete ui;
}
