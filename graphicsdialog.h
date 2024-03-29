#ifndef GRAPHICSDIALOG_H
#define GRAPHICSDIALOG_H

#include <QDialog>
#include <QChartView>
#include <QChart>
#include <QBarSeries>
#include <QBarSet>

#include "vesuppliermodel.h"

namespace Ui {
class GraphicsDialog;
}

class GraphicsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphicsDialog(QWidget *parent = nullptr, VesupplierModel *model = nullptr, int column = 7);
    ~GraphicsDialog();

private:
    Ui::GraphicsDialog *ui;
    VesupplierModel *m_model;
    QChart m_chart;
};

#endif // GRAPHICSDIALOG_H
