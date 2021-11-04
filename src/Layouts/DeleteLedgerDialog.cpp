
#include "DeleteLedgerDialog.h"

DeleteLedgerDialog::DeleteLedgerDialog(QWidget *parent): QDialog(parent), ui(new Ui::DeleteLedger) {
    ui->setupUi(this);
}

QString DeleteLedgerDialog::getLedgerName() {
    if (ui->ledgerToDelete->text() == ui->confirmationLine->text()) {
        return ui->ledgerToDelete->text();
    }
    else
    {
        return {};
    }
}

void DeleteLedgerDialog::setToDelete(const QString &name) {
    ui->ledgerToDelete->setText(name);
}
