
#ifndef TEST_DOLLARSPINBOX_H
#define TEST_DOLLARSPINBOX_H

#include <QSpinBox>

class DollarSpinBox: public QSpinBox {
    Q_OBJECT
public:
    explicit DollarSpinBox(QWidget *parent) : QSpinBox(parent) {
        setPrefix("$ ");
        setMinimum(-999999);
        setMaximum(999999);
        setSingleStep(100);
        setFrame(false);
    }

protected:
    int valueFromText(const QString &text) const override;
    QString textFromValue(int val) const override;
    QValidator::State validate(QString &input, int &pos) const override;
};


#endif //TEST_DOLLARSPINBOX_H
