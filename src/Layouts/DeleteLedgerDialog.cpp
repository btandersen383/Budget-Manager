
#include "DeleteLedgerDialog.h"

DeleteLedgerDialog::DeleteLedgerDialog(QWidget *parent)
: QDialog(parent), ui(new Ui::DeleteLedger) {
    ui->setupUi(this);

    // TODO: set up combo box
}

QString DeleteLedgerDialog::getLedgerName() {
    if (ui->tablesCombo->currentText() == ui->confirmationLine->text()) {
        return ui->tablesCombo->currentText();
    }
    else
        return {};
}
