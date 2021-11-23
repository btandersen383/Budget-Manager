
#include <QSqlQuery>
#include <utility>
#include "SqlQueryModelFactory.h"

namespace SqlQueryModelFactory {

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

    QVariant CustomSqlQueryModel::data(const QModelIndex &item) const {
        QVariant value = QSqlQueryModel::data(item);
        if (m_display) {
            // In display mode, make pretty
            if (item.column() == 1) {
                return centsToString(value.toInt());
            }
        }
        else {
            // Not display mode, raw value
            if (item.column() == 1) {
                // May need to keep numbers all positive
                return m_positive ? abs(value.toInt()) : value.toInt();
            }
        }
        return value;
    }

    constexpr static auto SQL_SUM_SINGLE_CATEGORY = R"(
        SELECT SUM(amount) FROM %1
        WHERE category=(SELECT id FROM categories
                        WHERE categories.category=?)
    )";

    constexpr static auto SQL_SUM_ALL_CATEGORY = R"(
        SELECT categories.category, SUM(%1.amount)
        FROM %1
        INNER JOIN categories ON categories.id=%1.category
        GROUP BY %1.category
        ORDER BY SUM(%1.amount)
    )";

    constexpr static auto SQL_SUM_ALL_CATEGORY_EXPENSE = R"(
        SELECT categories.category, SUM(%1.amount)
        FROM %1
        INNER JOIN categories ON categories.id=%1.category
        WHERE amount < 0
        GROUP BY %1.category
        ORDER BY SUM(%1.amount)
    )";

    constexpr static auto SQL_SUM_ALL_CATEGORY_INCOME = R"(
        SELECT categories.category, SUM(%1.amount)
        FROM %1
        INNER JOIN categories ON categories.id=%1.category
        WHERE amount > 0
        GROUP BY %1.category
        ORDER BY SUM(%1.amount)
    )";

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

    CustomSqlQueryModel* createModel(const SqlQueryModelFactory::QueryModel type,
                                const QString &table, const QVariant &extra) {
        auto* model = new CustomSqlQueryModel(table);

        switch (type) {
            case SqlQueryModelFactory::SUM_SINGLE_CATEGORY:
                if (!extra.isValid() || !extra.canConvert<QString>()) {
                    // Must have a string given as additional argument
                    model = nullptr;
                    break;
                }
                model->setQuery(QString(SQL_SUM_SINGLE_CATEGORY).arg(table));
                model->query().addBindValue(extra.toString());
                model->setHeaderData(1, Qt::Horizontal, "Sum");
                break;
            case SqlQueryModelFactory::SUM_ALL_CATEGORIES:
                model->setQuery(QString(SQL_SUM_ALL_CATEGORY).arg(table));
                model->setHeaderData(0, Qt::Horizontal, "Category");
                model->setHeaderData(1, Qt::Horizontal, "Sum");
                break;
            case SqlQueryModelFactory::SUM_ALL_CATEGORIES_EXPENSE:
                model->setQuery(QString(SQL_SUM_ALL_CATEGORY_EXPENSE).arg(table));
                model->setHeaderData(0, Qt::Horizontal, "Category");
                model->setHeaderData(1, Qt::Horizontal, "Sum");
                break;
            case SqlQueryModelFactory::SUM_ALL_CATEGORIES_INCOME:
                model->setQuery(QString(SQL_SUM_ALL_CATEGORY_INCOME).arg(table));
                model->setHeaderData(0, Qt::Horizontal, "Category");
                model->setHeaderData(1, Qt::Horizontal, "Sum");
                break;
            case SqlQueryModelFactory::SUM_SINGLE_MONTH:
                if (!extra.isValid() || !extra.canConvert<QString>()) {
                    // Must provide month as extra argument
                    model = nullptr;
                    break;
                }
                model->setQuery(QString(SQL_SUM_SINGLE_MONTH).arg(table));
                model->query().addBindValue(extra.toString());
                model->setHeaderData(1, Qt::Horizontal, "Sum");
                break;
            case SqlQueryModelFactory::SUM_ALL_MONTH:
                model->setQuery(QString(SQL_SUM_ALL_MONTHS).arg(table));
                model->setHeaderData(0, Qt::Horizontal, "Month");
                model->setHeaderData(1, Qt::Horizontal, "Sum");
                break;
            case SqlQueryModelFactory::SUM_CATEGORY_MONTH:
                model->setQuery(QString(SQL_SUM_ALL_CATEGORY_MONTH).arg(table));
                model->setHeaderData(0, Qt::Horizontal, "Month");
                model->setHeaderData(1, Qt::Horizontal, "Category");
                model->setHeaderData(2, Qt::Horizontal, "Sum");
                break;
            default:
                break;
        }
        return model;
    }
}