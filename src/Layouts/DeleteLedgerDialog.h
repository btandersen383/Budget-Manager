
#ifndef TEST_DELETELEDGERDIALOG_H
#define TEST_DELETELEDGERDIALOG_H


#include <QDialog>
#include "ui_deleteledger.h"

class DeleteLedgerDialog: public QDialog {
public:
    explicit DeleteLedgerDialog(QWidget *parent);

    QString getLedgerName();

private:
    Ui::DeleteLedger *ui;
};


#endif //TEST_DELETELEDGERDIALOG_H
