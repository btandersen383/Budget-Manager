
#include <iostream>
#include <src/Database/DbCategory.h>
#include <src/Database/DbLedger.h>
#include <QSqlTableModel>
#include "AnalysisWidget.h"

AnalysisWidget::AnalysisWidget(QWidget *parent): QWidget(parent), ui(new Ui::AnalysisView) {
    ui->setupUi(this);

    ledgersModel = new QSqlTableModel(this);
    ledgersModel->setTable("ledgers");
    if (!ledgersModel->select())
    {
//        showError(ledgersModel->lastError());
        return;
    }
    ledgersModel->sort(ledgersModel->fieldIndex("name"), Qt::AscendingOrder);
    ui->ledgerEdit->setModel(ledgersModel);
    ui->ledgerEdit->setModelColumn(ledgersModel->fieldIndex("name"));

    createExpenseChart(ui->chartView_1);
    createIncomeChart(ui->chartView_2);
    createBarChart(ui->chartView_3);
}

void AnalysisWidget::createExpenseChart(QChartView *view) {
    auto *series = new QtCharts::QPieSeries();
    QStringList catList = DbCategory::getCategoryList();
    QtCharts::QPieSlice *slice;
    for (auto & itr : catList) {
        if (!itr.contains(QRegExp("Income"))) {
            int amount = DbLedger::getTotalByCategory("Personal2020", itr);
            if (amount != 0) {
                series->append(itr, amount);
            }
        }
    }

    series->setLabelsVisible(true);

    expenseChart = new QtCharts::QChart();
    expenseChart->addSeries(series);
    expenseChart->setTitle("Expenses");
    expenseChart->setTheme(QtCharts::QChart::ChartThemeDark);

    expenseChart->legend()->setAlignment(Qt::AlignRight);

    view->setChart(expenseChart);
    view->setRenderHint(QPainter::Antialiasing);

    connect(series, &QPieSeries::hovered,
            this, &AnalysisWidget::onHover);
}

void AnalysisWidget::onHover(QPieSlice *slice, bool state) {
    slice->setExploded(state);
    if (state) {
        slice->setLabelArmLengthFactor(slice->labelArmLengthFactor()*1.5);
    }
    else {
        slice->setLabelArmLengthFactor(slice->labelArmLengthFactor()/1.5);
    }
}

void AnalysisWidget::createIncomeChart(QChartView *view) {
    auto *series = new QtCharts::QPieSeries();
    QStringList catList = DbCategory::getCategoryList();
    for (auto & itr : catList) {
        if (itr.contains(QRegExp("Income"))) {
            int amount = DbLedger::getTotalByCategory("Personal2020", itr);
            if (amount != 0) {
                series->append(itr, amount);
            }
        }
    }

    series->setLabelsVisible(true);

    incomeChart = new QtCharts::QChart();
    incomeChart->addSeries(series);
    incomeChart->setTitle("Expenses");
    incomeChart->setTheme(QtCharts::QChart::ChartThemeDark);

    view->setChart(incomeChart);
    view->setRenderHint(QPainter::Antialiasing);

    connect(series, &QPieSeries::hovered,
            this, &AnalysisWidget::onHover);
}

void AnalysisWidget::createBarChart(QChartView *view) {
    QMap<QString, int> totals = DbLedger::getTotals("Personal2020");
    auto *series = new QBarSeries();
    for (const auto &itr : totals.toStdMap()) {
        if (!itr.first.contains(QRegExp("Income"))) {
            auto *set = new QBarSet(itr.first);
            *set << itr.second;
            series->append(set);
        }
    }

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple barchart example");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);
//    chart.ax

    view->setChart(chart);
}