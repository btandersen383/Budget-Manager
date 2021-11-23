
#include <iostream>
#include "DbManagement.h"
#include "DbCategory.h"
#include "DbLedger.h"


QSqlError DbManagement::initdb(const QString &name) {
    // Connect to the database and open
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(name);

    if (!m_db.open()) {
        return m_db.lastError();
    }

    // Check to see if this database is already initialized
    QStringList tables = m_db.tables();
    if (tables.contains("ledgers", Qt::CaseInsensitive)
        && tables.contains("categories", Qt::CaseInsensitive))
        return QSqlError();

    // Every database must have a table of ledgers and categories
    QSqlQuery q;
    if (!q.exec(SQL_CREATE_CONTENTS_TABLE))
        return q.lastError();
    if (!q.exec(TABLE_CATEGORY_SQL))
        return q.lastError();

    // Initialize some general categories
    if (!q.prepare(INSERT_CATEGORY_SQL))
        return q.lastError();
    DbCategory::addCategory(q, QLatin1String("Home"));
    DbCategory::addCategory(q, QLatin1String("Auto"));
    DbCategory::addCategory(q, QLatin1String("Grocery"));
    DbCategory::addCategory(q, QLatin1String("Dining"));
    DbCategory::addCategory(q, QLatin1String("Entertainment"));
    DbCategory::addCategory(q, QLatin1String("Income"));

    // Set up a random empty ledger
    QSqlError result = DbLedger::addLedger("helloledger", "A basic starting ledger.");
    if (result.type() != QSqlError::NoError) {
        return result;
    }

    return {};
}

void DbManagement::closedb(const QString &name) {
    QSqlDatabase m_db = QSqlDatabase::database();
    if (m_db.isOpen()) {
        m_db.close();
    }
}
