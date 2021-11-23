
#include <QtSql>
#include <iostream>

#include "LedgerLayout.h"
#include "src/Widgets/TransactionsDelegate.h"
#include "src/Database/DbLedger.h"

LedgerLayout::LedgerLayout(QWidget *parent) : QWidget(parent), m_ui(new Ui::TransactionsWidget) {
    m_ui->setupUi(this);
    refreshModels();
    changeTable(m_ui->ledgerListWidget->currentIndex().data().toString());
    connectAll();
}

LedgerLayout::~LedgerLayout() {
    delete m_ledgerModel;
    delete m_tablesModel;
}

void LedgerLayout::connectAll() {
    // Connect the Add Transaction button to insert a new row
    connect(m_ui->addLedgerButton, &QPushButton::clicked,
            this, &LedgerLayout::addLedger);
    connect(m_ui->deleteLedger, &QPushButton::clicked,
            this, &LedgerLayout::deleteLedger);
    connect(m_ui->addButton, &QPushButton::clicked,
            this, &LedgerLayout::addTransaction);
    connect(m_ui->importButton, &QPushButton::clicked,
            this, &LedgerLayout::importCSV);
    connect(m_ui->filterButton, &QPushButton::clicked,
            this, &LedgerLayout::showFilterDialog);

    // Connect Revert, Save, and Delete buttons to save the m_ledgerModel to the Database
    connect(m_ui->saveButton, &QPushButton::clicked,
            this, &LedgerLayout::save);
    connect(m_ui->revertButton, &QPushButton::clicked,
            this, &LedgerLayout::revert);
    connect(m_ui->deleteButton, &QPushButton::clicked,
            this, &LedgerLayout::deleteRow);

    // Connect the table combo to change the viewed table
    // QOverload is used when there are multiple functions with the same name but different params
    connect(m_ui->ledgerListWidget, &QListView::clicked,
            this, &LedgerLayout::ledgerListSelection);
}

void LedgerLayout::enableButtons(bool enable) {
    m_ui->revertButton->setEnabled(enable);
    m_ui->saveButton->setEnabled(enable);
    // Disable the table changer when changes are not saved
    m_ui->ledgerListWidget->setDisabled(enable);
}

void LedgerLayout::addTransaction() {
    qDebug() << "Adding new transaction row";
    // Get an empty record for this table
    QSqlRecord rec = m_ledgerModel->record();
    rec.setValue("dateof", QDate::currentDate());
    rec.setValue("category", 1);
    rec.setValue("description", "new record");
    rec.setValue("amount", 69);
    m_ledgerModel->insertRecord(0, rec);
    m_ui->transactionsTable->selectRow(0);
}

void LedgerLayout::revert() {
    qDebug() << "Reverting changes";
    m_ledgerModel->revertAll();
    for (int i = 0; i < m_ledgerModel->rowCount(); ++i) {
        // warning, this will take a while for long tables...
        m_ui->transactionsTable->setRowHidden(i, false);
    }
}

void LedgerLayout::save() {
    qDebug() << "Saving the model to Database";
    if (!m_ledgerModel->submitAll()) {
        qWarning() << "Failed to submit changes";
        showError(m_ledgerModel->lastError());
    }
    for (int i = 0; i < m_ledgerModel->rowCount(); ++i) {
        // There has got to be a better way than to itr all...
        m_ui->transactionsTable->setRowHidden(i, false);
    }
}

void LedgerLayout::deleteRow() {
    int row = m_ui->transactionsTable->currentIndex().row();
    qDebug() << "Delete row " << row << " from table";
    m_ledgerModel->removeRow(row);
    m_ui->transactionsTable->hideRow(row);
    m_ui->transactionsTable->selectRow(row + 1);
}

void LedgerLayout::showFilterDialog() {
    qDebug() << "Showing Filter Dialog";
    m_filterDialog = new FilterLedgerDialog(this);
    m_filterDialog->exec();

    if (m_filterDialog->result() == QDialog::Accepted) {
        QString filter = "";
        if (m_filterDialog->hasStartDate() && !m_filterDialog->hasEndDate()) {
            filter += " dateof > '" + m_filterDialog->getStartDate().toString("yyyy-MM-dd") + "'";
        }
        else if (m_filterDialog->hasEndDate() && !m_filterDialog->hasStartDate()) {
            filter += " dateof < '" + m_filterDialog->getEndDate().toString("yyyy-MM-dd") + "'";
        }
        else if (m_filterDialog->hasStartDate() && m_filterDialog->hasEndDate()) {
            filter += " dateof BETWEEN '" + m_filterDialog->getStartDate().toString("yyyy-MM-dd")
                      + "' AND '" + m_filterDialog->getEndDate().toString("yyyy-MM-dd") + "'";
        }

        if (m_filterDialog->hasCategory()) {
            if (filter.length())
                filter += " AND ";
            int catIdx = m_ledgerModel->fieldIndex("category");
            filter += " relTblAl_" + QString::number(catIdx) + ".category='" + m_filterDialog->getCategory() + "'";
        }

        m_ledgerModel->setFilter(filter);
    }

    delete m_filterDialog;
}

void LedgerLayout::ledgerListSelection(const QModelIndex &index) {
    changeTable(index.data().toString());
}

void LedgerLayout::changeTable(const QString &name) {
    if (name.length() == 0) {
        return;
    }

    m_ledgerModel->setTable(name);

    // Set the relations to the other database tables
    // Sets the "category" column to be related to the "categories" table
    m_ledgerModel->setRelation(m_ledgerModel->fieldIndex("category"),
                               QSqlRelation("categories", "id", "category"));

    // Set the localized header captions
    m_ledgerModel->setHeaderData(m_ledgerModel->fieldIndex("dateof"),
                                 Qt::Horizontal, tr("Date"));
    m_ledgerModel->setHeaderData(m_ledgerModel->fieldIndex("category"),
                                 Qt::Horizontal, tr("Category"));
    m_ledgerModel->setHeaderData(m_ledgerModel->fieldIndex("description"),
                                 Qt::Horizontal, tr("Description"));
    m_ledgerModel->setHeaderData(m_ledgerModel->fieldIndex("amount"),
                                 Qt::Horizontal, tr("Amount"));

    // Populate the m_ledgerModel
    if (!m_ledgerModel->select())
    {
        showError(m_ledgerModel->lastError());
        return;
    }
    m_ledgerModel->sort(m_ledgerModel->fieldIndex("dateof"), Qt::DescendingOrder);

    // Set the m_ledgerModel and hide the ID column
    m_ui->transactionsTable->setModel(m_ledgerModel);
    m_ui->transactionsTable->setColumnHidden(m_ledgerModel->fieldIndex("id"), true);
    // Default sort by Date
    //    ui.transactionsTable->sortByColumn(m_ledgerModel->fieldIndex("dateof"));
    m_ui->transactionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    // Default relational delegate for all columns
    // auto deleg = new QSqlRelationalDelegate(m_ui->transactionsTable);
    m_ui->transactionsTable->setItemDelegate(new QSqlRelationalDelegate(m_ui->transactionsTable));
    // Special delegates for some columns
    m_ui->transactionsTable->setItemDelegateForColumn(
            m_ledgerModel->fieldIndex("dateof"),
            new TransactionDateDelegate(m_ui->transactionsTable));
    m_ui->transactionsTable->setItemDelegateForColumn(
            m_ledgerModel->fieldIndex("amount"),
            new TransactionAmountDelegate(m_ui->transactionsTable));

    // Keep the Description column the largest
    m_ui->transactionsTable->horizontalHeader()->setSectionResizeMode(
            m_ledgerModel->fieldIndex("description"), QHeaderView::Stretch);

    // Set the title label
    m_ui->activeLedgerLabel->setText(name);
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

        QSqlRecord rec = m_ledgerModel->record();
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

//        std::cout << "Date: " << rec.value("dateof").toString().toStdString()
//        << "  Category: " << rec.value("category").toString().toStdString()
//        << "  Description: " << rec.value("description").toString().toStdString()
//        << "  Amount: " << rec.value("amount").toString().toStdString() << std::endl;

        m_ledgerModel->insertRecord(0, rec);
        m_ui->transactionsTable->selectRow(0);

    }
    csvFile.close();
}

void LedgerLayout::showError(const QSqlError &err) {
    QMessageBox::critical(this, "Unable to initialize Database",
                          "Error initializing database: " + err.text());
}

void LedgerLayout::addLedger() {
    m_addLedgerDialog = new AddLedgerDialog(this);
    m_addLedgerDialog->exec();
    if (m_addLedgerDialog->result() == QDialog::Accepted) {
        DbLedger::addLedger(m_addLedgerDialog->getLedgerName(), m_addLedgerDialog->getLedgerDesc());
        m_tablesModel->select();
    }
    delete m_addLedgerDialog;
}

void LedgerLayout::deleteLedger() {
    m_deleteLedgerDialog = new DeleteLedgerDialog(this);
    m_deleteLedgerDialog->setToDelete(m_ui->ledgerListWidget->currentIndex().data().toString());
    m_deleteLedgerDialog->exec();
    if (m_deleteLedgerDialog->result() == QDialog::Accepted) {
        DbLedger::dropLedger(m_deleteLedgerDialog->getLedgerName());
        m_tablesModel->select();
    }
    delete m_deleteLedgerDialog;
}

void LedgerLayout::refreshModels() {
    // Clean up if already initialized
    if (m_tablesModel) {
        delete m_tablesModel;
    }
    if (m_ledgerModel) {
        delete m_ledgerModel;
    }

    // Initialize new models
    m_tablesModel = new QSqlTableModel(this);
    m_tablesModel->setTable("ledgers");
    if (!m_tablesModel->select())
    {
        showError(m_tablesModel->lastError());
        return;
    }
    m_tablesModel->sort(m_tablesModel->fieldIndex("name"), Qt::AscendingOrder);
    m_ui->ledgerListWidget->setModel(m_tablesModel);
    m_ui->ledgerListWidget->setModelColumn(m_tablesModel->fieldIndex("name"));

    // Create the data m_ledgerModel
    // Uses default connection to Database, Database must be opened
    m_ledgerModel = new QSqlRelationalTableModel(m_ui->transactionsTable);
    m_ledgerModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
}
