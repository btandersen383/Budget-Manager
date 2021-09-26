
#ifndef TEST_SQLQUERYMODELFACTORY_H
#define TEST_SQLQUERYMODELFACTORY_H

#include <QSqlQueryModel>

namespace SqlQueryModelFactory {

    enum QueryModel {
        SQL_SUM_SINGLE_CATEGORY,
        SQL_SUM_ALL_CATEGORY,
        SQL_SUM_SINGLE_MONTH,
        SQL_SUM_ALL_MONTH,
        SQL_SUM_CATEGORY_DATE_RANGE,
        SQL_SUM_CATEGORY_MONTH
    };

    // Creates a read-only model to use in a QTableView
    QSqlQueryModel* createModel(QueryModel type, const QString &table, const QVariant &extra = QVariant());

};


#endif //TEST_SQLQUERYMODELFACTORY_H
