
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

    signals:
    void databaseChanged();

private slots:
    void about();
    void ledgerView();
    void categoryView();
    void loadDB();

private:
    Ui::MainWindow *m_ui;
    LedgerLayout *m_ledgerLayout;
    CategoryLayout *m_categoryLayout;
    QString m_database = "testing.db";

    void connectMenuBar();
    void showError(const QSqlError &err);
    void showCharts();
    void showSummary();
};

#endif //TEST_MAINWINDOW_H
