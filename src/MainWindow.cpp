
#include "MainWindow.h"
#include "src/Database/DbManagement.h"
#include "src/Layouts/AnalysisWidget.h"
#include "src/Database/DbCategory.h"

MainWindow::MainWindow() : ui(new Ui::MainWindow) {
    ui->setupUi(this);

    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        QMessageBox::critical(this, "Unable to load database",
                "This demo needs the SQLITE driver");
    }

    // Initialize the database
    QSqlError err = DbManagement::initdb("testing.db");
    if (err.type() != QSqlError::NoError) {
        showError(err);
        return;
    }

    tw = new LedgerLayout(this);
    setCentralWidget(tw);

    cw = new CategoryLayout(this);
    cw->hide();
    connectMenuBar();
}

void MainWindow::connectMenuBar() {
    // File
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    // View
    connect(ui->actionLedgers, &QAction::triggered, this, &MainWindow::ledgerView);
    connect(ui->actionCategories, &QAction::triggered, this, &MainWindow::categoryView);

    // Analyze
    connect(ui->actionCharts, &QAction::triggered, this, &MainWindow::showCharts);
    connect(ui->actionSummary, &QAction::triggered, this, &MainWindow::showSummary);

    // Help
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
}

#include <iostream>
#include <src/Layouts/SummaryWidget.h>

void MainWindow::showCharts() {
    auto *analysisWindow = new AnalysisWidget();
    analysisWindow->show();
}

void MainWindow::showSummary() {
    auto *summaryWindow = new SummaryWidget();
    summaryWindow->show();
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Budgeteer"),
                       tr("<p>The <b>Budgeteer</b> app is mostly an project to learn Qt5. "
                          "It is also useful to keep track of some basic finances.</p>"
                          "<p>Version: 0.0.1"));
}

void MainWindow::showError(const QSqlError &err) {
    QMessageBox::critical(this, "Unable to initialize Database",
                          "Error initializing database: " + err.text());
}

void MainWindow::ledgerView() {
    if (centralWidget() != tw) {
        centralWidget()->hide();
        centralWidget()->setParent(nullptr);
        setCentralWidget(tw);
        tw->show();
    }
}

void MainWindow::categoryView() {
    if (centralWidget() != cw) {
        centralWidget()->hide();
        centralWidget()->setParent(nullptr);
        setCentralWidget(cw);
        cw->show();
    }
}
