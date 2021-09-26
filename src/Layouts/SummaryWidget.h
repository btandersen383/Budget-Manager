
#ifndef TEST_SUMMARYWIDGET_H
#define TEST_SUMMARYWIDGET_H

#include <QWidget>

#include "ui_summarylayout.h"

class SummaryWidget: public QWidget {
public:
    explicit SummaryWidget(QWidget *parent = nullptr);

private:
    Ui::SummaryLayout *ui;
};


#endif //TEST_SUMMARYWIDGET_H
