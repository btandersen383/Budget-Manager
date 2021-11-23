
#ifndef TEST_ANALYSISWIDGET_H
#define TEST_ANALYSISWIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include "ui_analysislayout.h"

class ChartsAnalysisLayout: public QWidget {
public:
    explicit ChartsAnalysisLayout(QWidget *parent = nullptr);

private:
    Ui::AnalysisView *ui;
    QtCharts::QChart *expenseChart;
    QtCharts::QChart *incomeChart;
    QSqlTableModel *ledgersModel;


    void createExpenseChart(QChartView *view);
    void createIncomeChart(QChartView *view);
    void onHover(QPieSlice *slice, bool state);
    void createBarChart(QChartView *view);
};

#endif //TEST_ANALYSISWIDGET_H
