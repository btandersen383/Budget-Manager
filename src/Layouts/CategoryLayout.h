
#ifndef TEST_CATEGORYLAYOUT_H
#define TEST_CATEGORYLAYOUT_H

#include <QtWidgets>
#include <QtSql>

#include "ui_categorylayout.h"

class CategoryLayout: public QWidget {
Q_OBJECT

public:
    explicit CategoryLayout(QWidget *parent = nullptr);

public slots:
    void refreshModels();

signals:
    void changedCategory();

private slots:
    void save();
    void deleteCat();
    void addCat();

private:
    Ui::CategoriesWidget ui{};
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;

    void showError(const QSqlError &err);
};


#endif //TEST_CATEGORYLAYOUT_H
