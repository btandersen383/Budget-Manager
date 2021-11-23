
#include "SummaryWidget.h"
#include "src/Database/SqlQueryModelFactory.h"

SummaryWidget::SummaryWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SummaryLayout) {
    ui->setupUi(this);

    QString table("Personal2020");

    ui->tableViewLabel_1->setText("Categories");
    ui->tableView_1->setModel(
            SqlQueryModelFactory::createModel(
                    SqlQueryModelFactory::SUM_ALL_CATEGORIES, table));
    ui->tableView_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableViewLabel_2->setText("Monthly");
    ui->tableView_2->setModel(
            SqlQueryModelFactory::createModel(
                    SqlQueryModelFactory::SUM_ALL_MONTH, table));

    ui->tableViewLabel_3->setText("Unused");
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
