
#ifndef TEST_DELETELEDGERDIALOG_H
#define TEST_DELETELEDGERDIALOG_H


#include <QDialog>
#include "ui_deleteledger.h"

class DeleteLedgerDialog: public QDialog {
public:
    explicit DeleteLedgerDialog(QWidget *parent);
    void setToDelete(const QString &name);
    QString getLedgerName();

private:
    Ui::DeleteLedger *ui;
};


#endif //TEST_DELETELEDGERDIALOG_H
