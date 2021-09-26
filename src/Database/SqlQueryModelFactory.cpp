
#include "SqlQueryModelFactory.h"

QString centsToString(int cents) {
    // Use abs since its easier to calculate comma
    QString dollars = QString::number(abs(cents));
    dollars = dollars.insert(dollars.size()-2, ".");
    // If I need more than one comma, I'm probably not doing my own budget keeping...
    if (dollars.size() > 6) {
        dollars = dollars.insert(dollars.size()-6, ",");
    }
    dollars.prepend("$");
    if (cents < 0) {
        dollars.prepend("-");
    }
    return dollars;
}

class SumCategory: public QSqlQueryModel {

    // This will sum grouped by category only
    // This will sum a single category
    constexpr static auto SQL_SUM_SINGLE_CATEGORY = R"(
        SELECT SUM(amount) FROM %1
        WHERE category=(SELECT id FROM categories
                        WHERE categories.category=?)
    )";

public:
    SumCategory(const QString &tableName, const QString &category, QObject *parent = nullptr) : QSqlQueryModel(parent) {
        setQuery(QString(SQL_SUM_SINGLE_CATEGORY).arg(tableName, category));
        QSqlQueryModel::setHeaderData(1, Qt::Horizontal, tr("Sum"));
    }

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override {
        QVariant value = QSqlQueryModel::data(item, role);
        if (value.isValid() && role == Qt::DisplayRole) {
            if (item.column() == 1)
                return centsToString(value.toInt());
        }
        return value;
    }
};

class SumAllCategory: public QSqlQueryModel {

    // This will sum grouped by category only
    constexpr static auto SQL_SUM_ALL_CATEGORY = R"(
        SELECT categories.category, SUM(%1.amount)
        FROM %1
        INNER JOIN categories ON categories.id=%1.category
        GROUP BY %1.category
    )";

public:
    explicit SumAllCategory(const QString &tableName, QObject *parent = nullptr) : QSqlQueryModel(parent) {
        setQuery(QString(SQL_SUM_ALL_CATEGORY).arg(tableName));
        QSqlQueryModel::setHeaderData(0, Qt::Horizontal, tr("Category"));
        QSqlQueryModel::setHeaderData(1, Qt::Horizontal, tr("Sum"));
    }

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override {
        QVariant value = QSqlQueryModel::data(item, role);
        if (value.isValid() && role == Qt::DisplayRole) {
            if (item.column() == 1)
                return centsToString(value.toInt());
        }
        return value;
    }
};

class SumMonth: public QSqlQueryModel {

    // This will sum a single month
    constexpr static auto SQL_SUM_SINGLE_MONTH = R"(
        SELECT SUM(%1.amount),
               case strftime('%m', %1.dateof)
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
        FROM %1
        WHERE strftime('%m', dateof)='03'
    )";

public:
    SumMonth(const QString &tableName, const QString &month, QObject *parent = nullptr) : QSqlQueryModel(parent) {
        setQuery(QString(SQL_SUM_SINGLE_MONTH).arg(tableName, month));
        QSqlQueryModel::setHeaderData(1, Qt::Horizontal, tr("Sum"));
    }
};

class SumAllMonths: public QSqlQueryModel {

    // This will sum all month
    constexpr static auto SQL_SUM_ALL_MONTHS = R"(
        SELECT case strftime('%m', %1.dateof)
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
               else '' end,
            SUM(%1.amount)
        FROM %1
        GROUP BY strftime('%m', dateof)
    )";

public:
    explicit SumAllMonths(const QString &tableName, QObject *parent = nullptr) : QSqlQueryModel(parent) {
        setQuery(QString(SQL_SUM_ALL_MONTHS).arg(tableName));
        QSqlQueryModel::setHeaderData(0, Qt::Horizontal, tr("Month"));
        QSqlQueryModel::setHeaderData(1, Qt::Horizontal, tr("Sum"));
    }

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override {
        QVariant value = QSqlQueryModel::data(item, role);
        if (value.isValid() && role == Qt::DisplayRole) {
            if (item.column() == 1)
                return centsToString(value.toInt());
        }
        return value;
    }
};

class SumAllCategoryMonth: public QSqlQueryModel {

    // This will sum all month
    constexpr static auto SQL_SUM_ALL_CATEGORY_MONTH = R"(
        SELECT case strftime('%m', %1.dateof)
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
                    else '' end,
                categories.category, SUM(%1.amount)
        FROM %1
                 INNER JOIN categories ON categories.id=%1.category
        GROUP BY %1.category, strftime('%m', dateof)
    )";

public:
    explicit SumAllCategoryMonth(const QString &tableName, QObject *parent = nullptr) : QSqlQueryModel(parent) {
        setQuery(QString(SQL_SUM_ALL_CATEGORY_MONTH).arg(tableName));
        QSqlQueryModel::setHeaderData(0, Qt::Horizontal, tr("Month"));
        QSqlQueryModel::setHeaderData(1, Qt::Horizontal, tr("Category"));
        QSqlQueryModel::setHeaderData(2, Qt::Horizontal, tr("Sum"));
    }
};


QSqlQueryModel* SqlQueryModelFactory::createModel(SqlQueryModelFactory::QueryModel type,
                            const QString &table, const QVariant &extra) {
    switch (type) {
        case SqlQueryModelFactory::SQL_SUM_SINGLE_CATEGORY:
            // Expects a valid category in the extra variant
            return new SumCategory(table, extra.toString());
        case SqlQueryModelFactory::SQL_SUM_ALL_CATEGORY:
            return new SumAllCategory(table);
        case SqlQueryModelFactory::SQL_SUM_SINGLE_MONTH:
            // Expects a valid month in the extra variant
            return new SumMonth(table, extra.toString());
        case SqlQueryModelFactory::SQL_SUM_ALL_MONTH:
            return new SumAllMonths(table);
        case SqlQueryModelFactory::SQL_SUM_CATEGORY_DATE_RANGE:
            // This will require two variants...
            break;
        case SqlQueryModelFactory::SQL_SUM_CATEGORY_MONTH:
            return new SumAllCategoryMonth(table);
        default:
            return new QSqlQueryModel;
    }
    return new QSqlQueryModel;
}
