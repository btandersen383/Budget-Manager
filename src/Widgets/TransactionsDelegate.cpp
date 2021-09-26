
#include <QtWidgets>
#include <iostream>

#include "TransactionsDelegate.h"
#include "src/Widgets/DollarSpinBox.h"

/*****************************************************************************/
TransactionDateDelegate::TransactionDateDelegate(QObject *parent)
: QStyledItemDelegate(parent) {}

QWidget *TransactionDateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const {
    // For editing the year, return a spinbox with a range from -1000 to 2100.
    auto *dateEdit = new QDateEdit(parent);
    dateEdit->setFrame(false);
    dateEdit->setDisplayFormat("dd MMM yyyy");
    dateEdit->setCalendarPopup(true);
    return dateEdit;
}

QString TransactionDateDelegate::displayText(const QVariant &value, const QLocale &locale) const {
    auto date = QDate::fromString(value.toString(), "yyyy-MM-dd");
    return date.toString("dd MMM yyyy");
}

/*****************************************************************************/
TransactionAmountDelegate::TransactionAmountDelegate(QObject *parent)
: QStyledItemDelegate(parent) {}

QWidget *TransactionAmountDelegate::createEditor(QWidget *parent,
                                                 const QStyleOptionViewItem &option,
                                                 const QModelIndex &index) const {
    auto *spinBox = new DollarSpinBox(parent);
    return spinBox;
}

QString TransactionAmountDelegate::displayText(const QVariant &value, const QLocale &locale) const {
    int dollar = abs(value.toInt()) / 100;
    int cents = abs(value.toInt()) % 100;
    QString sign = "";
    if (value.toInt() < 0) {
        sign = "-";
    }
    QString strCents = QString::number(cents);
    if (cents < 10) {
        strCents = "0" + strCents;
    }
    QString display = "$ " + sign + QString::number(dollar) + "." + strCents;
    return display;
}
