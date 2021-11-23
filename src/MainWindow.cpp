#include <iostream>
#include <src/Layouts/SummaryWidget.h>
#include "MainWindow.h"
#include "src/Database/DbManagement.h"
#include "src/Layouts/ChartsAnalysisLayout.h"
#include "src/Database/DbCategory.h"

MainWindow::MainWindow() : m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);

    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        QMessageBox::critical(this, "Unable to load database",
                "This demo needs the SQLITE driver");
    }

    // Initialize the database
    QSqlError err = DbManagement::initdb(m_database);
    if (err.type() != QSqlError::NoError) {
        showError(err);
        return;
    }

    m_ledgerLayout = new LedgerLayout(this);
    connect(this, &MainWindow::databaseChanged, m_ledgerLayout, &LedgerLayout::refreshModels);
    setCentralWidget(m_ledgerLayout);

    m_categoryLayout = new CategoryLayout(this);
    m_categoryLayout->hide();
    connectMenuBar();
}

void MainWindow::connectMenuBar() {
    // File
    connect(m_ui->actionLoad, &QAction::triggered, this, &MainWindow::loadDB);
    connect(m_ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    // View
    connect(m_ui->actionLedgers, &QAction::triggered, this, &MainWindow::ledgerView);
    connect(m_ui->actionCategories, &QAction::triggered, this, &MainWindow::categoryView);

    // Analyze
    connect(m_ui->actionCharts, &QAction::triggered, this, &MainWindow::showCharts);
    connect(m_ui->actionSummary, &QAction::triggered, this, &MainWindow::showSummary);

    // Help
    connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::showCharts() {
    auto *analysisWindow = new ChartsAnalysisLayout();
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
    if (centralWidget() != m_ledgerLayout) {
        centralWidget()->hide();
        centralWidget()->setParent(nullptr);
        setCentralWidget(m_ledgerLayout);
        m_ledgerLayout->show();
    }
}

void MainWindow::categoryView() {
    if (centralWidget() != m_categoryLayout) {
        centralWidget()->hide();
        centralWidget()->setParent(nullptr);
        setCentralWidget(m_categoryLayout);
        m_categoryLayout->show();
    }
}

void MainWindow::loadDB() {
    QString databaseFileName = QFileDialog::getOpenFileName(
            this, tr("Open Budget Database"),
            QDir::homePath(), tr("Database File (*.db)"));
    qDebug() << "Attempting to open " << databaseFileName;

    // Make sure the current connection closes
    DbManagement::closedb(m_database);

    // Set the new database name
    m_database = databaseFileName;

    // Initialize the database
    QSqlError err = DbManagement::initdb(m_database);
    if (err.type() != QSqlError::NoError) {
        showError(err);
        return;
    }

    emit databaseChanged();
}
