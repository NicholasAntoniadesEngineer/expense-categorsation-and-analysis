/**
 * @file main_window.cpp
 * @brief Implementation of the Finance Manager main window
 * @author Nicholas Antoniades
 * @date 2024-01-24
 * 
 * This file implements the main GUI window functionality for the
 * Finance Manager application, handling user interactions and file processing.
 */

#include "main_window.hpp"
#include "finance_processor.hpp"
#include "chart_manager.hpp"
#include "window_manager.hpp"
#include "plot_manager.hpp"
#include "table_manager.hpp"
#include <QMessageBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <QIcon>
#include <QFont>
#include <QtCharts>
#include <QToolTip>
#include <QLabel>
#include <QTextEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <functional>

namespace FinanceManager {

// Constructor and Initialization
MainWindow::MainWindow(AppConfig& config, QWidget *parent)
    : QMainWindow(parent)
    , config(config)
{
    // Initialize window-specific strings
    config.strings = {
        .INPUT_DIR_TITLE = "Input Directory",
        .OUTPUT_DIR_TITLE = "Output Directory",
        .KEYWORD_FILE_TITLE = "Keyword File",
        .EXPORT_OPTIONS_TITLE = "Export Options",
        .BROWSE_BUTTON_TEXT = "Browse",
        .PROCESS_BUTTON_TEXT = "Process Files",
        .MONTHLY_SUMMARY_TEXT = "Export Monthly Summary",
        .WEEKLY_SUMMARY_TEXT = "Export Weekly Summary",
        .FULL_DATASET_TEXT = "Export Full Dataset",
        .SELECT_INPUT_DIR_TEXT = "Select Input Directory",
        .SELECT_OUTPUT_DIR_TEXT = "Select Output Directory",
        .SELECT_KEYWORD_FILE_TEXT = "Select Keyword File",
        .CSV_FILE_FILTER = "CSV Files (*.csv);;All Files (*)",
        .ERROR_TITLE = "Error",
        .SUCCESS_TITLE = "Success",
        .FIELDS_REQUIRED_ERROR = "All fields must be filled",
        .PROCESS_SUCCESS = "Files processed successfully!",
        .PLOT_WEEKLY_TEXT = "Plot Weekly Summary",
        .PLOT_MONTHLY_TEXT = "Plot Monthly Summary"
    };

    setupUi();
    createConnections();
}

// Window Setup and Configuration
bool MainWindow::initializeApplicationInfo() noexcept {
    QApplication::setApplicationName(config.app_name);
    QApplication::setOrganizationName(config.org_name);
    QApplication::setOrganizationDomain(config.org_domain);
    QApplication::setApplicationVersion(config.app_version);
    QApplication::setApplicationDisplayName(config.app_name);
    return true;
}

bool MainWindow::initializeAppearance() noexcept {
    QStyle* style = QStyleFactory::create(config.style_name);
    if (!style) {
        return false;
    }
    QApplication::setStyle(style);

    // Use ".AppleSystemUIFont" which is guaranteed to be available on macOS
    QFont appFont(".AppleSystemUIFont", config.default_font_size);
    QApplication::setFont(appFont);
    
    QIcon appIcon(":/icons/app_icon.png");
    if (!appIcon.isNull()) {
        QApplication::setWindowIcon(appIcon);
    }
    
    return true;
}

bool MainWindow::setupWindow() noexcept {
    setWindowTitle(config.app_name);
    resize(config.default_window_width, config.default_window_height);
    show();
    return true;
}

// UI Setup
void MainWindow::setupUi() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    // Create input directory group
    QGroupBox *inputGroup = new QGroupBox(config.strings.INPUT_DIR_TITLE, this);
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputDirEdit = new QLineEdit(this);
    inputBrowseButton = new QPushButton(config.strings.BROWSE_BUTTON_TEXT, this);
    inputLayout->addWidget(inputDirEdit);
    inputLayout->addWidget(inputBrowseButton);
    inputGroup->setLayout(inputLayout);
    
    // Create output directory group
    QGroupBox *outputGroup = new QGroupBox(config.strings.OUTPUT_DIR_TITLE, this);
    QHBoxLayout *outputLayout = new QHBoxLayout;
    outputDirEdit = new QLineEdit(this);
    outputBrowseButton = new QPushButton(config.strings.BROWSE_BUTTON_TEXT, this);
    outputLayout->addWidget(outputDirEdit);
    outputLayout->addWidget(outputBrowseButton);
    outputGroup->setLayout(outputLayout);
    
    // Create keyword file group
    QGroupBox *keywordGroup = new QGroupBox(config.strings.KEYWORD_FILE_TITLE, this);
    QHBoxLayout *keywordLayout = new QHBoxLayout;
    keywordFileEdit = new QLineEdit(this);
    keywordBrowseButton = new QPushButton(config.strings.BROWSE_BUTTON_TEXT, this);
    keywordLayout->addWidget(keywordFileEdit);
    keywordLayout->addWidget(keywordBrowseButton);
    keywordGroup->setLayout(keywordLayout);
    
    // Create export options group
    QGroupBox *exportGroup = new QGroupBox(config.strings.EXPORT_OPTIONS_TITLE, this);
    QVBoxLayout *exportLayout = new QVBoxLayout;
    exportMonthlySummaryCheck = new QCheckBox(config.strings.MONTHLY_SUMMARY_TEXT, this);
    exportWeeklySummaryCheck = new QCheckBox(config.strings.WEEKLY_SUMMARY_TEXT, this);
    exportFullDatasetCheck = new QCheckBox(config.strings.FULL_DATASET_TEXT, this);
    exportLayout->addWidget(exportMonthlySummaryCheck);
    exportLayout->addWidget(exportWeeklySummaryCheck);
    exportLayout->addWidget(exportFullDatasetCheck);
    exportGroup->setLayout(exportLayout);
    
    // Create process button
    processButton = new QPushButton(config.strings.PROCESS_BUTTON_TEXT, this);
    
    // Create plot buttons
    plotWeeklyButton = new QPushButton(config.strings.PLOT_WEEKLY_TEXT, this);
    plotMonthlyButton = new QPushButton(config.strings.PLOT_MONTHLY_TEXT, this);
    
    // Create visualization group
    QGroupBox *visualizationGroup = new QGroupBox("Visualization", this);
    QVBoxLayout *visualizationLayout = new QVBoxLayout;
    
    // Create subgroup for plots
    QGroupBox *plotGroup = new QGroupBox("Plots", this);
    QHBoxLayout *plotLayout = new QHBoxLayout;
    plotLayout->addWidget(plotWeeklyButton);
    plotLayout->addWidget(plotMonthlyButton);
    plotGroup->setLayout(plotLayout);
    
    // Create subgroup for summaries
    QGroupBox *summaryGroup = new QGroupBox("View Summaries", this);
    QHBoxLayout *summaryLayout = new QHBoxLayout;
    QPushButton *viewWeeklySummaryButton = new QPushButton("View Weekly Summary", this);
    QPushButton *viewMonthlySummaryButton = new QPushButton("View Monthly Summary", this);
    QPushButton *viewAllTransactionsButton = new QPushButton("View All Transactions", this);
    summaryLayout->addWidget(viewWeeklySummaryButton);
    summaryLayout->addWidget(viewMonthlySummaryButton);
    summaryLayout->addWidget(viewAllTransactionsButton);
    summaryGroup->setLayout(summaryLayout);
    
    visualizationLayout->addWidget(plotGroup);
    visualizationLayout->addWidget(summaryGroup);
    visualizationGroup->setLayout(visualizationLayout);
    
    // Add all widgets to main layout
    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(outputGroup);
    mainLayout->addWidget(keywordGroup);
    mainLayout->addWidget(exportGroup);
    mainLayout->addWidget(processButton);
    mainLayout->addWidget(visualizationGroup);
    mainLayout->addStretch();
    
    // Set default paths and states
    setupDefaultPaths();
    setupDefaultStates();

    // Connect summary buttons
    connect(viewWeeklySummaryButton, &QPushButton::clicked, this, &MainWindow::viewWeeklySummary);
    connect(viewMonthlySummaryButton, &QPushButton::clicked, this, &MainWindow::viewMonthlySummary);
    connect(viewAllTransactionsButton, &QPushButton::clicked, this, &MainWindow::viewAllTransactions);
}

void MainWindow::setupDefaultPaths() {
    // Navigate from source file location to repository root
    QDir appDir(QString::fromUtf8(__FILE__));
    for (int i = 0; i < 4; ++i) {
        appDir.cdUp();
    }
    
    // Set default values with paths relative to repository root
    inputDirEdit->setText(appDir.absoluteFilePath("input_files"));
    outputDirEdit->setText(appDir.absoluteFilePath("output_files"));
    keywordFileEdit->setText(appDir.absoluteFilePath("config/categorisation_keywords.csv"));
}

void MainWindow::setupDefaultStates() {
    exportMonthlySummaryCheck->setChecked(true);
    exportWeeklySummaryCheck->setChecked(false);
    exportFullDatasetCheck->setChecked(true);
}

void MainWindow::createConnections() {
    connect(inputBrowseButton, &QPushButton::clicked, this, &MainWindow::browseInputDirectory);
    connect(outputBrowseButton, &QPushButton::clicked, this, &MainWindow::browseOutputDirectory);
    connect(keywordBrowseButton, &QPushButton::clicked, this, &MainWindow::browseKeywordFile);
    connect(processButton, &QPushButton::clicked, this, &MainWindow::processFiles);
    connect(plotWeeklyButton, &QPushButton::clicked, this, &MainWindow::plotWeeklySummary);
    connect(plotMonthlyButton, &QPushButton::clicked, this, &MainWindow::plotMonthlySummary);
}

// Event Handlers
void MainWindow::browseInputDirectory() {
    QString dir = QFileDialog::getExistingDirectory(
        this, config.strings.SELECT_INPUT_DIR_TEXT,
        inputDirEdit->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (!dir.isEmpty()) {
        inputDirEdit->setText(dir);
        inputDirectory = dir;
    }
}

void MainWindow::browseOutputDirectory() {
    QString dir = QFileDialog::getExistingDirectory(
        this, config.strings.SELECT_OUTPUT_DIR_TEXT,
        outputDirEdit->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (!dir.isEmpty()) {
        outputDirEdit->setText(dir);
        outputDirectory = dir;
    }
}

void MainWindow::browseKeywordFile() {
    QString file = QFileDialog::getOpenFileName(
        this, config.strings.SELECT_KEYWORD_FILE_TEXT,
        keywordFileEdit->text(),
        config.strings.CSV_FILE_FILTER
    );
    
    if (!file.isEmpty()) {
        keywordFileEdit->setText(file);
        keywordFile = file;
    }
}

void MainWindow::processFiles() {
    try {
        QString inputDir = inputDirEdit->text();
        QString outputDir = outputDirEdit->text();
        QString keywordFile = keywordFileEdit->text();
        
        if (inputDir.isEmpty() || outputDir.isEmpty() || keywordFile.isEmpty()) {
            QMessageBox::warning(this, config.strings.ERROR_TITLE, config.strings.FIELDS_REQUIRED_ERROR);
            return;
        }
        
        FinanceProcessor processor(
            inputDir.toStdString(),
            outputDir.toStdString(),
            keywordFile.toStdString(),
            exportMonthlySummaryCheck->isChecked(),
            exportWeeklySummaryCheck->isChecked(),
            exportFullDatasetCheck->isChecked()
        );
        
        processor.run();
        QMessageBox::information(this, config.strings.SUCCESS_TITLE, config.strings.PROCESS_SUCCESS);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, config.strings.ERROR_TITLE, 
                            QString("Processing failed: %1").arg(e.what()));
    }
}

void MainWindow::plotData(const QString& filePattern, const QString& title, const QString& xAxisTitle) {
    QString outputDir = outputDirEdit->text();
    PlotWindow*& plotWindow = (title.contains("Weekly") ? weeklyPlotWindow : monthlyPlotWindow);
    
    PlotManager::createPlotFromFile(plotWindow, outputDir, filePattern, title, xAxisTitle, this,
        [this](const QString& category, bool visible) {
            updateSeriesVisibility(category, visible);
        });
}

void MainWindow::plotWeeklySummary() {
    if (weeklyPlotWindow) {
        weeklyPlotWindow->activateWindow();
        weeklyPlotWindow->raise();
        return;
    }
    plotData("weekly_summary.csv", "Weekly Expense Summary Over Time", "Week Number");
}

void MainWindow::plotMonthlySummary() {
    if (monthlyPlotWindow) {
        monthlyPlotWindow->activateWindow();
        monthlyPlotWindow->raise();
        return;
    }
    plotData("monthly_summary.csv", "Monthly Expense Summary Over Time", "Month Number");
}

void MainWindow::viewAllTransactions() {
    QString outputDir = outputDirEdit->text();
    TableManager::TableConfig config{
        .title = "All Categorised Transactions",
        .fileName = "categorised_transactions.csv",
        .width = 1200,
        .height = 800
    };
    
    allTransactionsWindow = TableManager::showTableFromFile(
        allTransactionsWindow, outputDir, config, this);

    if (allTransactionsWindow) {
        connect(allTransactionsWindow, &QObject::destroyed, this, [this]() {
            allTransactionsWindow = nullptr;
        });
    }
}

void MainWindow::viewWeeklySummary() {
    QString outputDir = outputDirEdit->text();
    TableManager::TableConfig config{
        .title = "Weekly Summary",
        .fileName = "weekly_summary.csv",
        .width = 1000,
        .height = 600
    };
    
    weeklySummaryWindow = TableManager::showTableFromFile(
        weeklySummaryWindow, outputDir, config, this);

    if (weeklySummaryWindow) {
        connect(weeklySummaryWindow, &QObject::destroyed, this, [this]() {
            weeklySummaryWindow = nullptr;
        });
    }
}

void MainWindow::viewMonthlySummary() {
    QString outputDir = outputDirEdit->text();
    TableManager::TableConfig config{
        .title = "Monthly Summary",
        .fileName = "monthly_summary.csv",
        .width = 1000,
        .height = 600
    };
    
    monthlySummaryWindow = TableManager::showTableFromFile(
        monthlySummaryWindow, outputDir, config, this);

    if (monthlySummaryWindow) {
        connect(monthlySummaryWindow, &QObject::destroyed, this, [this]() {
            monthlySummaryWindow = nullptr;
        });
    }
}

void MainWindow::updateSeriesVisibility(const QString& category, bool visible) {
    PlotManager::updateSeriesVisibility(weeklyPlotWindow, category, visible);
    PlotManager::updateSeriesVisibility(monthlyPlotWindow, category, visible);
}

// Utility Functions
void MainWindow::showErrorMessage(const QString& message, const char* title) noexcept {
    QMessageBox::critical(nullptr, title, message, QMessageBox::Ok);
}

}  // namespace FinanceManager 