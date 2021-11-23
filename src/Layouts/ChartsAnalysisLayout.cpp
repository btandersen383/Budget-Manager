
#include <iostream>
#include <QSqlTableModel>
#include <QVPieModelMapper>
#include <src/Database/DbCategory.h>
#include <src/Database/DbLedger.h>
#include <src/Database/SqlQueryModelFactory.h>
#include "ChartsAnalysisLayout.h"

ChartsAnalysisLayout::ChartsAnalysisLayout(QWidget *parent): QWidget(parent), ui(new Ui::AnalysisView) {
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

void ChartsAnalysisLayout::createExpenseChart(QChartView *view) {
    auto *mapper = new QVPieModelMapper(this);
    auto *model = SqlQueryModelFactory::createModel(SqlQueryModelFactory::SUM_ALL_CATEGORIES_EXPENSE, "Personal2020");
    model->setDisplayRole(false);
    auto *series = new QtCharts::QPieSeries();
    series->setLabelsVisible(true);

    mapper->setSeries(series);
    mapper->setModel(model);
    mapper->setLabelsColumn(0);
    mapper->setValuesColumn(1);

    for (auto itr : series->slices()) {
        itr->setLabel(itr->label() + " %" + QString::number(itr->percentage()*100, 'f', 1));
    }

    expenseChart = new QtCharts::QChart();
    expenseChart->addSeries(series);
    expenseChart->setTitle("Expenses");
    expenseChart->setTheme(QtCharts::QChart::ChartThemeDark);
    expenseChart->legend()->setAlignment(Qt::AlignRight);

    view->setChart(expenseChart);
    view->setRenderHint(QPainter::Antialiasing);

    connect(series, &QPieSeries::hovered,
            this, &ChartsAnalysisLayout::onHover);
}

void ChartsAnalysisLayout::onHover(QPieSlice *slice, bool state) {
    slice->setExploded(state);
    if (state) {
        slice->setLabelVisible(true);

    }
    else {
        slice->setLabelVisible(false);
    }
}

void ChartsAnalysisLayout::createIncomeChart(QChartView *view) {
    auto *mapper = new QVPieModelMapper(this);
    auto *model = SqlQueryModelFactory::createModel(SqlQueryModelFactory::SUM_ALL_CATEGORIES_INCOME, "Personal2020");
    model->setDisplayRole(false);
    auto *series = new QtCharts::QPieSeries();
    series->setLabelsVisible(true);

    mapper->setSeries(series);
    mapper->setModel(model);
    mapper->setLabelsColumn(0);
    mapper->setValuesColumn(1);

    for (auto itr : series->slices()) {
        itr->setLabel(itr->label() + " %" + QString::number(itr->percentage()*100, 'f', 1));
    }

    expenseChart = new QtCharts::QChart();
    expenseChart->addSeries(series);
    expenseChart->setTitle("Income");
    expenseChart->setTheme(QtCharts::QChart::ChartThemeDark);
    expenseChart->legend()->setAlignment(Qt::AlignRight);

    view->setChart(expenseChart);
    view->setRenderHint(QPainter::Antialiasing);
    connect(series, &QPieSeries::hovered,
            this, &ChartsAnalysisLayout::onHover);
}

void ChartsAnalysisLayout::createBarChart(QChartView *view) {
    auto *mapper = new QVBarModelMapper(this);
    auto *model = SqlQueryModelFactory::createModel(SqlQueryModelFactory::SUM_ALL_MONTH, "Personal2020");
    model->setDisplayRole(false);
    auto *series = new QtCharts::QBarSeries();

    mapper->setFirstBarSetColumn(1);
    mapper->setLastBarSetColumn(1);
    mapper->setModel(model);
    mapper->setSeries(series);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple barchart example");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTheme(QtCharts::QChart::ChartThemeDark);
    chart->legend()->setAlignment(Qt::AlignRight);

    QStringList categories;
    for (int i=0; i<model->rowCount(); ++i) {
        categories << model->data(model->index(i,0)).toString();
    }
//    for (auto itr : series->barSets()) {
//        std::cout << itr->at(0) << std::endl;
//        if (itr->at(0) < 0) {
//            itr->setColor(QColor::fromRgb(255,0,0));
//        }
//        else {
//            itr->setColor(QColor::fromRgb(0,255,0));
//        }
//    }

    auto *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto *axisY = new QValueAxis();
    axisY->setTickAnchor(0);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    view->setChart(chart);
}