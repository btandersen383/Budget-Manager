
#ifndef TEST_FILTERLEDGERDIALOG_H
#define TEST_FILTERLEDGERDIALOG_H


#include <QDialog>
#include <QSqlTableModel>
#include "ui_filterledgerdialog.h"

class FilterLedgerDialog: public QDialog {
Q_OBJECT
public:
    explicit FilterLedgerDialog(QWidget *parent = nullptr);
    ~FilterLedgerDialog() override;

    void setStartDate(const QDate &date);
    void setEndDate(const QDate &date);
    void setCategory(const QString &str);

    bool hasStartDate();
    bool hasEndDate();
    bool hasCategory();

    QDate getStartDate();
    QDate getEndDate();
    QString getCategory();

private:
    Ui::FilterLedger ui{};
    QSqlTableModel *model;
};


#endif //TEST_FILTERLEDGERDIALOG_H
