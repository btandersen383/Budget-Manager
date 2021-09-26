
#include <QtSql>
#include <iostream>

#include "LedgerLayout.h"
#include "src/Widgets/TransactionsDelegate.h"
#include "src/Database/DbLedger.h"

LedgerLayout::LedgerLayout(QWidget *parent) : QWidget(parent) {
    ui.setupUi(this);

    tablesModel = new QSqlTableModel(this);
    tablesModel->setTable("ledgers");
    if (!tablesModel->select())
    {
        showError(tablesModel->lastError());
        return;
    }
    tablesModel->sort(tablesModel->fieldIndex("name"), Qt::AscendingOrder);
    ui.tablesCombo->setModel(tablesModel);
    ui.tablesCombo->setModelColumn(tablesModel->fieldIndex("name"));

    // Create the data ledgerModel
    // Uses default connection to Database, Database must be opened
    ledgerModel = new QSqlRelationalTableModel(ui.transactionsTable);
    ledgerModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    changeTable(ui.tablesCombo->currentText());

    connectAll();
}

LedgerLayout::~LedgerLayout() {
    delete ledgerModel;
    delete tablesModel;
}

void LedgerLayout::connectAll() {
    // Connect the Add Transaction button to insert a new row
    connect(ui.addLedgerButton, &QPushButton::clicked,
            this, &LedgerLayout::addLedger);
    connect(ui.deleteLedger, &QPushButton::clicked,
            this, &LedgerLayout::deleteLedger);
    connect(ui.addButton, &QPushButton::clicked,
            this, &LedgerLayout::addTransaction);
    connect(ui.importButton, &QPushButton::clicked,
            this, &LedgerLayout::importCSV);
    connect(ui.filterButton, &QPushButton::clicked,
            this, &LedgerLayout::showFilterDialog);

    // Connect Revert, Save, and Delete buttons to save the ledgerModel to the Database
    connect(ui.saveButton, &QPushButton::clicked,
            this, &LedgerLayout::save);
    connect(ui.revertButton, &QPushButton::clicked,
            this, &LedgerLayout::revert);
    connect(ui.deleteButton, &QPushButton::clicked,
            this, &LedgerLayout::deleteRow);

    // Connect the table combo to change the viewed table
    // QOverload is used when there are multiple functions with the same name but different params
    connect(ui.tablesCombo, &QComboBox::currentTextChanged,
            this, &LedgerLayout::changeTable);
}

void LedgerLayout::enableButtons(bool enable) {
    ui.revertButton->setEnabled(enable);
    ui.saveButton->setEnabled(enable);
    // Disable the table changer when changes are not saved
    ui.tablesCombo->setDisabled(enable);
}

void LedgerLayout::addTransaction() {
    qDebug() << "Adding new transaction row";
    // Get an empty record for this table
    QSqlRecord rec = ledgerModel->record();
    rec.setValue("dateof", QDate::currentDate());
    rec.setValue("category", 1);
    rec.setValue("description", "new record");
    rec.setValue("amount", 69);
    ledgerModel->insertRecord(0, rec);
    ui.transactionsTable->selectRow(0);
}

void LedgerLayout::revert() {
    qDebug() << "Reverting changes";
    ledgerModel->revertAll();
    for (int i = 0; i < ledgerModel->rowCount(); ++i) {
        // warning, this will take a while for long tables...
        ui.transactionsTable->setRowHidden(i, false);
    }
}

void LedgerLayout::save() {
    qDebug() << "Saving the model to Database";
    if (!ledgerModel->submitAll()) {
        qWarning() << "Failed to submit changes";
        showError(ledgerModel->lastError());
    }
    for (int i = 0; i < ledgerModel->rowCount(); ++i) {
        // There has got to be a better way than to itr all...
        ui.transactionsTable->setRowHidden(i, false);
    }
}

void LedgerLayout::deleteRow() {
    int row = ui.transactionsTable->currentIndex().row();
    qDebug() << "Delete row " << row << " from table";
    ledgerModel->removeRow(row);
    ui.transactionsTable->hideRow(row);
    ui.transactionsTable->selectRow(row + 1);
}

void LedgerLayout::showFilterDialog() {
    qDebug() << "Showing Filter Dialog";
    fld = new FilterLedgerDialog(this);
    fld->exec();

    if (fld->result() == QDialog::Accepted) {
        QString filter = "";
        if (fld->hasStartDate() && !fld->hasEndDate()) {
            filter += " dateof > '" + fld->getStartDate().toString("yyyy-MM-dd") + "'";
        }
        else if (fld->hasEndDate() && !fld->hasStartDate()) {
            filter += " dateof < '" + fld->getEndDate().toString("yyyy-MM-dd") + "'";
        }
        else if (fld->hasStartDate() && fld->hasEndDate()) {
            filter += " dateof BETWEEN '" + fld->getStartDate().toString("yyyy-MM-dd")
                    + "' AND '" + fld->getEndDate().toString("yyyy-MM-dd") + "'";
        }

        if (fld->hasCategory()) {
            if (filter.length())
                filter += " AND ";
            int catIdx = ledgerModel->fieldIndex("category");
            filter += " relTblAl_" + QString::number(catIdx) + ".category='" + fld->getCategory() + "'";
        }

        ledgerModel->setFilter(filter);
    }

    delete fld;
}

void LedgerLayout::changeTable(const QString &name) {
    if (name.length() == 0) {
        return;
    }

    ledgerModel->setTable(name);

    // Set the relations to the other database tables
    // Sets the "category" column to be related to the "categories" table
    ledgerModel->setRelation(ledgerModel->fieldIndex("category"),
                             QSqlRelation("categories", "id", "category"));

    // Set the localized header captions
    ledgerModel->setHeaderData(ledgerModel->fieldIndex("dateof"),
                               Qt::Horizontal, tr("Date"));
    ledgerModel->setHeaderData(ledgerModel->fieldIndex("category"),
                               Qt::Horizontal, tr("Category"));
    ledgerModel->setHeaderData(ledgerModel->fieldIndex("description"),
                               Qt::Horizontal, tr("Description"));
    ledgerModel->setHeaderData(ledgerModel->fieldIndex("amount"),
                               Qt::Horizontal, tr("Amount"));

    // Populate the ledgerModel
    if (!ledgerModel->select())
    {
        showError(ledgerModel->lastError());
        return;
    }
    ledgerModel->sort(ledgerModel->fieldIndex("dateof"), Qt::DescendingOrder);

    // Set the ledgerModel and hide the ID column
    ui.transactionsTable->setModel(ledgerModel);
    ui.transactionsTable->setColumnHidden(ledgerModel->fieldIndex("id"), true);
    // Default sort by Date
    //    ui.transactionsTable->sortByColumn(ledgerModel->fieldIndex("dateof"));
    ui.transactionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    // Default relational delegate for all columns
    auto deleg = new QSqlRelationalDelegate(ui.transactionsTable);
    ui.transactionsTable->setItemDelegate(new QSqlRelationalDelegate(ui.transactionsTable));
    // Special delegates for some columns
    ui.transactionsTable->setItemDelegateForColumn(
            ledgerModel->fieldIndex("dateof"),
            new TransactionDateDelegate(ui.transactionsTable));
    ui.transactionsTable->setItemDelegateForColumn(
            ledgerModel->fieldIndex("amount"),
            new TransactionAmountDelegate(ui.transactionsTable));

    // Keep the Description column the largest
    ui.transactionsTable->horizontalHeader()->setSectionResizeMode(
            ledgerModel->fieldIndex("description"), QHeaderView::Stretch);
}

int strToCents(QString &str) {
    // Split the dollar from the cents
    QStringList total = str.split(".");
    // Replace and white space and $ signs
    total[0].replace(QRegularExpression("[$\\s]"), "");
    // Check for negative value
    bool negative = total[0].contains(QRegularExpression("-"));
    // Get dollar/cents ints
    int dollar = total[0].toInt();
    int cents = 0;
    if (total.count() > 1)
        cents = total[1].toInt();
    if (negative)
        return dollar*100 - cents;
    else
        return dollar*100 + cents;
}

void LedgerLayout::importCSV() {
    QString csvFileName = QFileDialog::getOpenFileName(this, tr("Open CSV"),
                                 QDir::homePath(), tr("CSV File (*.csv)"));
    qDebug() << "Attempting to import " << csvFileName << " into table";

    QFile csvFile(csvFileName);
    csvFile.open(QIODevice::ReadOnly);
    QTextStream s1(&csvFile);

    while (!s1.atEnd()) {
        QString line = s1.readLine();
        QStringList splitLine = line.split(',');

        QString dateof = splitLine[LedgerLayout::dateof];
        QString description = splitLine[LedgerLayout::description];
        QString category = splitLine[LedgerLayout::category];
        int totalCentsDebit = strToCents(splitLine[LedgerLayout::debit]);
        int totalCentsCredit = strToCents(splitLine[LedgerLayout::credit]);

        QSqlRecord rec = ledgerModel->record();
        rec.setValue("dateof", QDate::fromString(dateof, "yyyy-MM-dd"));
        rec.setValue("category", 1);
        rec.setValue("description", description);
        if (totalCentsDebit != 0) {
            rec.setValue("amount", -1*totalCentsDebit);
        }
        else if (totalCentsCredit != 0) {
            rec.setValue("amount", totalCentsCredit);
        }
        else {
            rec.setValue("amount", 0);
        }

        std::cout << "Date: " << rec.value("dateof").toString().toStdString()
        << "  Category: " << rec.value("category").toString().toStdString()
        << "  Description: " << rec.value("description").toString().toStdString()
        << "  Amount: " << rec.value("amount").toString().toStdString() << std::endl;

        ledgerModel->insertRecord(0, rec);
        ui.transactionsTable->selectRow(0);

    }
    csvFile.close();
}

void LedgerLayout::showError(const QSqlError &err) {
    QMessageBox::critical(this, "Unable to initialize Database",
                          "Error initializing database: " + err.text());
}

void LedgerLayout::addLedger() {
    addLedgerDialog = new AddLedgerDialog(this);
    addLedgerDialog->exec();
    if (addLedgerDialog->result() == QDialog::Accepted) {
        DbLedger::addLedger(addLedgerDialog->getLedgerName(), addLedgerDialog->getLedgerDesc());
        tablesModel->select();
        // Do not remove, the combox box will be empty after select() for some reason
        ui.tablesCombo->setCurrentText(addLedgerDialog->getLedgerName());
    }
    delete addLedgerDialog;
}

void LedgerLayout::deleteLedger() {

}
