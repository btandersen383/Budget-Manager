
#ifndef TEST_SQLQUERYMODELFACTORY_H
#define TEST_SQLQUERYMODELFACTORY_H

#include <QSqlQueryModel>

namespace SqlQueryModelFactory {

    enum QueryModel {
        SUM_SINGLE_CATEGORY,  // Sums all transactions in a single category
        SUM_ALL_CATEGORIES,  // Sums all transactions in all categories
        SUM_ALL_CATEGORIES_EXPENSE,  // Sums all expenses in all categories
        SUM_ALL_CATEGORIES_INCOME,  // Sums all incomes in all categories
        SUM_SINGLE_MONTH,
        SUM_SINGLE_MONTH_EXPENSE,
        SUM_SINGLE_MONTH_INCOME,
        SUM_ALL_MONTH,
        SUM_ALL_MONTH_EXPENSE,
        SUM_ALL_MONTH_INCOME,
        SUM_CATEGORY_DATE_RANGE,
        SUM_CATEGORY_MONTH,
        AVG_SINGLE_CATEGORY,
        AVG_ALL_CATEGORIES,
        AVG_ALL_MONTH_INCOME,
        AVG_ALL_MONTH_EXPENSE
    };

    class CustomSqlQueryModel: public QSqlQueryModel {

        QString m_tableName;
        bool m_positive = false;
        bool m_display = true;

    public:
        explicit CustomSqlQueryModel(QString tableName, QObject *parent = nullptr)
        : QSqlQueryModel(parent), m_tableName(std::move(tableName)) {}

        QVariant data(const QModelIndex &item) const;

        QString tableName() { return m_tableName; }

        void setPositive(bool positive) { m_positive = positive; }

        void setDisplayRole(bool display) { m_display = display; }
    };

    // Creates a read-only model to use in a QTableView
    CustomSqlQueryModel* createModel(QueryModel type, const QString &table, const QVariant &extra = QVariant());

};


#endif //TEST_SQLQUERYMODELFACTORY_H
