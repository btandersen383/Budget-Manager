
#include <QPushButton>
#include <iostream>

#include "AddLedgerDialog.h"

AddLedgerDialog::AddLedgerDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddLedger) {
    ui->setupUi(this);
}

QString AddLedgerDialog::getLedgerName() {
    return ui->nameEdit->text();
}

QString AddLedgerDialog::getLedgerDesc() {
    return ui->descriptionEdit->toPlainText();
}
