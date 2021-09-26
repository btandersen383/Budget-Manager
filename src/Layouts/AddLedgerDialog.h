
#ifndef TEST_ADDLEDGERDIALOG_H
#define TEST_ADDLEDGERDIALOG_H

#include <QDialog>

#include "ui_addledger.h"

class AddLedgerDialog: public QDialog {
    Q_OBJECT

public:
    explicit AddLedgerDialog(QWidget *parent);
    QString getLedgerName();
    QString getLedgerDesc();

private:
    Ui::AddLedger *ui;
};

#endif //TEST_ADDLEDGERDIALOG_H
