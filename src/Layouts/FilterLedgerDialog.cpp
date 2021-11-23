
#include "FilterLedgerDialog.h"
#include <QPushButton>

FilterLedgerDialog::FilterLedgerDialog(QWidget *parent): QDialog(parent) {
    ui.setupUi(this);

    ui.buttonBox->addButton(ui.buttonBox->button(QDialogButtonBox::Apply),
                            QDialogButtonBox::AcceptRole);

    QDate now = QDate::currentDate();
    QDate start;
    if (now.dayOfYear() < 60) {
        // If we are less than 2 months into this year,
        // Start from last year
        start = QDate(now.year()-1, 1, 1);
    }
    else {
        // If we are past 2 months, default to YTD
        start = QDate(now.year(), 1, 1);
    }
    ui.startDate->setDate(start);
    ui.endDate->setDate(now);
    ui.startDate->setMaximumDate(now);
    ui.endDate->setMaximumDate(now);

    // Initialize the Category combo edit values
    model = new QSqlTableModel();
    model->setTable("categories");
    if (!model->select()) {
//        showError(model->lastError());
        return;
    }
    // Get the relation m_ledgerModel, ie the table setRelation connects a column to
    ui.categoryEdit->setModel(model);
    // Set the column in the m_ledgerModel (categories table) to use for values
    ui.categoryEdit->setModelColumn(model->fieldIndex("category"));
    ui.categoryEdit->setEditable(false);

    // Ensure start is always <= end
    connect(ui.startDate, &QDateEdit::userDateChanged,
            ui.endDate, &QDateEdit::setMinimumDate);
    connect(ui.endDate, &QDateEdit::userDateChanged,
            ui.startDate, &QDateEdit::setMaximumDate);
}

FilterLedgerDialog::~FilterLedgerDialog() = default;

void FilterLedgerDialog::setStartDate(const QDate &date) {
    ui.startDate->setDate(date);
}

void FilterLedgerDialog::setEndDate(const QDate &date) {
    ui.endDate->setDate(date);
}

void FilterLedgerDialog::setCategory(const QString &str) {
    ui.categoryEdit->setCurrentText(str);
}

QDate FilterLedgerDialog::getStartDate() {
    return ui.startDate->date();
}

QDate FilterLedgerDialog::getEndDate() {
    return ui.endDate->date();
}

QString FilterLedgerDialog::getCategory() {
    return ui.categoryEdit->currentText();
}

bool FilterLedgerDialog::hasStartDate() {
    return ui.startDateEnabled->isChecked();
}

bool FilterLedgerDialog::hasEndDate() {
    return ui.endDateEnabled->isChecked();
}

bool FilterLedgerDialog::hasCategory() {
    return !ui.anyCategory->isChecked();
}
