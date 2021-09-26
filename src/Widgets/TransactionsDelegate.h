
#ifndef TRANSACTIONDELEGATE_H
#define TRANSACTIONDELEGATE_H

#include <QModelIndex>
#include <QSqlRelationalDelegate>

QT_FORWARD_DECLARE_CLASS(QPainter)

class TransactionDateDelegate : public QStyledItemDelegate {
public:
    explicit TransactionDateDelegate(QObject *parent);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    QString displayText(const QVariant &value, const QLocale &locale) const override;
};

class TransactionAmountDelegate : public QStyledItemDelegate {
public:
    explicit TransactionAmountDelegate(QObject *parent);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    QString displayText(const QVariant &value, const QLocale &locale) const override;
};

#endif
