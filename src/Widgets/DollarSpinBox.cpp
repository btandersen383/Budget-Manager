
#include <iostream>
#include "DollarSpinBox.h"

int DollarSpinBox::valueFromText(const QString &text) const {
    // This should be validated by validator below
    QStringList total = text.split(".");
    total[0].replace(QRegularExpression("[$\\s]"), "");
    bool negative = total[0].contains(QRegularExpression("-"));
    int dollar = total[0].toInt();
    int cents = 0;
    if (total.count() > 1) {
        cents = total[1].toInt();
    }
    if (negative) {
        return dollar*100 - cents;
    }
    else {
        return dollar*100 + cents;
    }
}

QString DollarSpinBox::textFromValue(int val) const {
    int dollar = 0;
    int cents = 0;
    QString sign = "";
    if (val < 0) {
        sign = "-";
    }
    dollar = abs(val) / 100;
    cents = abs(val) % 100;
    QString strCents;
    if (cents < 10) {
        strCents = "0" + QString::number(cents);
    }
    else {
        strCents = QString::number(cents);
    }
    QString display = sign + QString::number(dollar) + "." + strCents;
    return display;
}

QValidator::State DollarSpinBox::validate(QString &input, int &pos) const {
    // Regex should match "$ -123.45", w/ or w/o "-" "." or cents
    QRegExpValidator validator(QRegExp("[$\\s]+-?[0-9]+\\.?[0-9]?[0-9]?"));
    return validator.validate(input, pos);
}
