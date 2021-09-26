
#include <QString>
#include <QVariant>
#include <QSqlQuery>
#include "DbCategory.h"

QVariant DbCategory::addCategory(QSqlQuery &q, const QString &name) {
    q.addBindValue(name);
    q.exec();
    return q.lastInsertId();
}

QStringList DbCategory::getCategoryList() {
    QStringList lst;
    QSqlQuery q(CATEGORY_SQL);
    if (q.exec()) {
        while (q.next()) {
            lst.append(q.value(0).toString());
        }
        return lst;
    }
    return {};
}
