
#ifndef TRANSACTIONWIDGET_H
#define TRANSACTIONWIDGET_H

#include <QtWidgets>
#include <QtSql>

#include "ui_ledgerlayout.h"
#include "src/Layouts/FilterLedgerDialog.h"
#include "AddLedgerDialog.h"
#include "DeleteLedgerDialog.h"

class LedgerLayout: public QWidget {
Q_OBJECT

public:
    explicit LedgerLayout(QWidget *parent = nullptr);
    ~LedgerLayout() override;

public slots:
    void refreshModels();

private slots:
    void addTransaction();
    void addLedger();
    void deleteLedger();
    void revert();
    void save();
    void enableButtons(bool enable);
    void importCSV();
    void deleteRow();
    void changeTable(const QString &name);
    void ledgerListSelection(const QModelIndex &index);
    void showFilterDialog();

private:
    void connectAll();
    void showError(const QSqlError &err);

    enum CSVColumns {
        dateof = 0,
        description = 1,
        category = 2,
        debit = 3,
        credit = 4
        };

    Ui::TransactionsWidget *m_ui = nullptr;
    FilterLedgerDialog *m_filterDialog = nullptr;
    AddLedgerDialog *m_addLedgerDialog = nullptr;
    DeleteLedgerDialog *m_deleteLedgerDialog = nullptr;
    QSqlTableModel *m_tablesModel = nullptr;
    QSqlRelationalTableModel *m_ledgerModel = nullptr;
};

#endif