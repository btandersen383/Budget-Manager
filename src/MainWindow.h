
#ifndef TEST_MAINWINDOW_H
#define TEST_MAINWINDOW_H

#include <QtWidgets>
#include <QtSql>

#include "ui_mainwindow.h"
#include "src/Layouts/LedgerLayout.h"
#include "src/Layouts/CategoryLayout.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

private slots:
    void about();
    void ledgerView();
    void categoryView();

private:
    Ui::MainWindow *ui;
    LedgerLayout *tw;
    CategoryLayout *cw;

    void connectMenuBar();
    void showError(const QSqlError &err);
    void showCharts();
    void showSummary();
};

#endif //TEST_MAINWINDOW_H
