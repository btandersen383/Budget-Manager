
#ifndef TEST_DBCATEGORY_H
#define TEST_DBCATEGORY_H

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QSqlQuery>

const auto TABLE_CATEGORY_SQL = QString(R"(
    create table categories(id integer primary key, category varchar)
)");
const auto INSERT_CATEGORY_SQL = QString(R"(
    insert into categories(category) values(?)
)");
/*** Select data from tables ***/
const auto CATEGORY_SQL = QString(R"(
    SELECT category from categories
)");

class DbCategory {
public:
    static QVariant addCategory(QSqlQuery &q, const QString &name);
    static QStringList getCategoryList();
};


#endif //TEST_DBCATEGORY_H
