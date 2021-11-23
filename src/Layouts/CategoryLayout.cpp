
#include "CategoryLayout.h"

CategoryLayout::CategoryLayout(QWidget *parent): QWidget(parent) {
    ui.setupUi(this);

    model = new QSqlTableModel(ui.tableView);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("categories");
    if (!model->select()) {
        showError(model->lastError());
        return;
    }

    model->setHeaderData(model->fieldIndex("id"),
                         Qt::Horizontal, tr("Id"));
    model->setHeaderData(model->fieldIndex("category"),
                         Qt::Horizontal, tr("Category"));

    // Set the m_ledgerModel and hide the ID column
    ui.tableView->setModel(model);
    ui.tableView->setColumnHidden(model->fieldIndex("id"), true);
    ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.tableView->setItemDelegate(new QSqlRelationalDelegate(ui.tableView));

    // Keep the Description column the largest
    ui.tableView->horizontalHeader()->setSectionResizeMode(
            model->fieldIndex("category"), QHeaderView::Stretch);

    // Connects Database value to set value of widget, allows change in widget to change Database value
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->addMapping(ui.categoryEdit, model->fieldIndex("category"));

    connect(ui.tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            mapper, &QDataWidgetMapper::setCurrentModelIndex);

    // Connect the Add Transaction button to insert a new row
    connect(ui.addButton, &QPushButton::clicked,
            this, &CategoryLayout::addCat);
    connect(ui.saveButton, &QPushButton::clicked,
            this, &CategoryLayout::save);
    connect(ui.deleteButton, &QPushButton::clicked,
            this, &CategoryLayout::deleteCat);

    ui.tableView->selectRow(0);
}

void CategoryLayout::showError(const QSqlError &err) {
    QMessageBox::critical(this, "Unable to initialize Database",
                          "Error initializing database: " + err.text());

}

void CategoryLayout::save() {
    if (!model->submitAll()) {
        showError(model->lastError());
    }
    emit changedCategory();
}

void CategoryLayout::deleteCat() {
}

void CategoryLayout::addCat() {
    qDebug() << "Adding category";
    // Get an empty record for this table
    QSqlRecord rec = model->record();
    model->insertRecord(0, rec);
    ui.tableView->selectRow(0);
    ui.categoryEdit->setFocus();
}

void CategoryLayout::refreshModels() {

}
