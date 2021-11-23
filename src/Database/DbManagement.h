
#ifndef TEST_DBMANAGEMENT_H
#define TEST_DBMANAGEMENT_H

#include <QtSql>
#include <QString>


class DbManagement {
public:
    static QSqlError initdb(const QString &name);
    static void closedb(const QString &name);
};


#endif //TEST_DBMANAGEMENT_H
