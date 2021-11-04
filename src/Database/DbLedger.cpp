
#include "DbManagement.h"
#include <iostream>
#include "DbLedger.h"

QSqlError DbLedger::addLedger(const QString &name, const QString &description) {
    QSqlQuery q;

    // First create the actual table itself
    auto test = SQL_CREATE_LEDGER_TABLE.arg(name);
    if (!q.prepare(test)) {
        return q.lastError();
    }
    q.exec();

    // Then add it to the list of ledgers table
    if (!q.prepare(SQL_INSERT_CONTENTS)) {
        return q.lastError();
    }
    q.addBindValue(name);
    q.addBindValue(description);
    q.exec();

    return q.lastError();
}

void DbLedger::addTransactions(QSqlQuery &q, const QDate &dateof, const QString &category,
                                   const QByteArray &description, int amount) {
    q.addBindValue(dateof);
    q.addBindValue(category);
    q.addBindValue(description);
    q.addBindValue(amount);
    q.exec();
}

int DbLedger::getTotalByCategory(const QString &tableName, const QString &cat) {
    auto q = QSqlQuery();
    if (!q.prepare(SQL_SUM_SINGLE_CATEGORY.arg(tableName))) {
        return 0;
    }
    q.addBindValue(cat);
    if (q.exec()) {
        if (q.next()) {
            return q.value(0).toInt();
        }
        else {
            return 0;
        }
    }
    return 0;
}

QSqlError DbLedger::dropLedger(const QString &name) {
    QSqlQuery q;

    // Prep and drop
    auto sqlstr = SQL_DROP_LEDGER_TABLE.arg(name);
    if (!q.prepare(sqlstr)) {
        return q.lastError();
    }
    q.exec();

    if (!q.prepare(SQL_DROP_LEDGER_REFERENCE)) {
        return q.lastError();
    }
    q.addBindValue(name);
    q.exec();

    return q.lastError();
}

QMap<QString, int> DbLedger::getTotals(const QString &tableName) {
    QMap<QString, int> map;
    auto q = QSqlQuery();

    if (!q.prepare(SQL_SUM_ALL_CATEGORY.arg(tableName))) {
        return {};
    }

    if (q.exec()) {
        while (q.next()) {
            map.insert(q.value(0).toString(), q.value(1).toInt());
        }
    }
    else {
        return {};
    }
    return map;
}
