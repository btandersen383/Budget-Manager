
#ifndef TEST_DBLEDGER_H
#define TEST_DBLEDGER_H

#include <QString>
#include <QVariant>
#include <QDate>
#include <QSqlQuery>


const auto SQL_CREATE_LEDGER_TABLE = QString(R"(
CREATE TABLE IF NOT EXISTS %1(id integer primary key,
                              dateof date,
                              category integer,
                              description blob,
                              amount integer)
)");

const auto SQL_DROP_LEDGER_TABLE = QString(R"(
DROP TABLE %1
)");

const auto SQL_DROP_LEDGER_REFERENCE = QString(R"(
DELETE FROM ledgers WHERE name=?
)");

const auto SQL_CREATE_CONTENTS_TABLE = QString(R"(
CREATE TABLE ledgers(name varchar primary key,
                     description blob)
)");

/*** Insert data into tables ***/
const auto INSERT_TRANSACTION_SQL = QString(R"(
INSERT INTO %1(dateof, category, description, amount)
               VALUES(?, ?, ?, ?)
)");

const auto SQL_INSERT_CONTENTS = QString(R"(
INSERT INTO ledgers(name, description)
                    VALUES(?, ?)
)");

// This will sum a single category
const auto SQL_SUM_SINGLE_CATEGORY = QString(R"(
SELECT SUM(amount) FROM %1
WHERE category=(SELECT id FROM categories
                WHERE categories.category=?)
)");

// This will sum grouped by category only
const auto SQL_SUM_ALL_CATEGORY = QString(R"(
SELECT categories.category, SUM(%1.amount)
FROM Personal2020
INNER JOIN categories ON categories.id=Personal2020.category
GROUP BY Personal2020.category
)");

// This will sum a single month
const auto SQL_SUM_SINGLE_MONTH = QString(R"(
SELECT SUM(Personal2020.amount),
       case strftime('%m', Personal2020.dateof)
           when '01' then 'January'
           when '02' then 'Febuary'
           when '03' then 'March'
           when '04' then 'April'
           when '05' then 'May'
           when '06' then 'June'
           when '07' then 'July'
           when '08' then 'August'
           when '09' then 'September'
           when '10' then 'October'
           when '11' then 'November'
           when '12' then 'December'
           else '' end
FROM Personal2020
WHERE strftime('%m', dateof)='03'
)");

// This will sum grouped by month only
const auto SQL_SUM_ALL_MONTH = QString(R"(
SELECT SUM(Personal2020.amount),
       case strftime('%m', Personal2020.dateof)
           when '01' then 'January'
           when '02' then 'Febuary'
           when '03' then 'March'
           when '04' then 'April'
           when '05' then 'May'
           when '06' then 'June'
           when '07' then 'July'
           when '08' then 'August'
           when '09' then 'September'
           when '10' then 'October'
           when '11' then 'November'
           when '12' then 'December'
           else '' end
FROM Personal2020
GROUP BY strftime('%m', dateof)
)");

// This will sum grouped categories within a date range
const auto SQL_SUM_CATEGORY_DATE_RANGE = QString(R"(
SELECT categories.category, SUM(Personal2020.amount)
FROM Personal2020
         INNER JOIN categories ON categories.id=Personal2020.category
WHERE Personal2020.dateof BETWEEN date('2020-02-15') AND ('2020-04-12')
GROUP BY Personal2020.category
)");

// This will sum grouped by Category and Month
const auto SQL_SUM_CATEGORY_MONTH = QString(R"(
SELECT categories.category, SUM(Personal2020.amount),
       case strftime('%m', Personal2020.dateof)
           when '01' then 'January'
           when '02' then 'Febuary'
           when '03' then 'March'
           when '04' then 'April'
           when '05' then 'May'
           when '06' then 'June'
           when '07' then 'July'
           when '08' then 'August'
           when '09' then 'September'
           when '10' then 'October'
           when '11' then 'November'
           when '12' then 'December'
           else '' end
FROM Personal2020
INNER JOIN categories ON categories.id=Personal2020.category
GROUP BY Personal2020.category, strftime('%m', dateof)
)");


class DbLedger {
public:
    static QSqlError addLedger(const QString &name, const QString &description);
    static QSqlError dropLedger(const QString &name);
    static void addTransactions(QSqlQuery &q, const QDate &dateof, const QString &category,
                                const QByteArray &description, int amount);
    static int getTotalByCategory(const QString &tableName, const QString &cat);
    static QMap<QString, int> getTotals(const QString &tableName);
};


#endif //TEST_DBLEDGER_H
