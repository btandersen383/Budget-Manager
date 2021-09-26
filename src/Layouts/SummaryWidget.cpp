
#include "SummaryWidget.h"
#include "src/Database/SqlQueryModelFactory.h"

SummaryWidget::SummaryWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SummaryLayout) {
    ui->setupUi(this);

    QString table("Personal2020");
    ui->tableView_1->setModel(
            SqlQueryModelFactory::createModel(
                    SqlQueryModelFactory::SQL_SUM_ALL_CATEGORY, table));
    ui->tableView_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->setModel(
            SqlQueryModelFactory::createModel(
                    SqlQueryModelFactory::SQL_SUM_ALL_MONTH, table));
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}
