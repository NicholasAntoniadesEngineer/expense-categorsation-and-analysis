#pragma once

#include <QString>
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "plot_manager.hpp"
#include "table_window.hpp"

namespace FinanceManager {

class VisualizationManager {
public:
    struct Windows {
        PlotWindow* weeklyPlotWindow = nullptr;
        PlotWindow* monthlyPlotWindow = nullptr;
        TableWindow* allTransactionsWindow = nullptr;
        TableWindow* weeklySummaryWindow = nullptr;
        TableWindow* monthlySummaryWindow = nullptr;
    };

    struct VisualizationButtons {
        QPushButton* plotWeeklyButton;
        QPushButton* plotMonthlyButton;
        QPushButton* viewWeeklySummaryButton;
        QPushButton* viewMonthlySummaryButton;
        QPushButton* viewAllTransactionsButton;
    };

    // Window management methods
    static void plotData(Windows& windows, 
                        const QString& outputDir,
                        const QString& filePattern, 
                        const QString& title, 
                        const QString& xAxisTitle,
                        QWidget* parent);

    static void plotWeeklySummary(Windows& windows, 
                                 const QString& outputDir,
                                 QWidget* parent);

    static void plotMonthlySummary(Windows& windows, 
                                  const QString& outputDir,
                                  QWidget* parent);

    static void viewAllTransactions(Windows& windows,
                                  const QString& outputDir,
                                  QWidget* parent);

    static void viewWeeklySummary(Windows& windows,
                                 const QString& outputDir,
                                 QWidget* parent);

    static void viewMonthlySummary(Windows& windows,
                                 const QString& outputDir,
                                 QWidget* parent);

    static void updateSeriesVisibility(Windows& windows,
                                     const QString& category,
                                     bool visible);

    // UI group creation methods
    static QGroupBox* createVisualizationGroup(QWidget* parent, VisualizationButtons& buttons);

private:
    template<typename T>
    static void setupWindowDestroyHandler(Windows& windows, T*& window) {
        (void)windows;  // Silence unused parameter warning
        QObject::connect(window, &QObject::destroyed, [&window]() {
            window = nullptr;
        });
    }

    static QGroupBox* createPlotGroup(QWidget* parent, 
                                    QPushButton* plotWeeklyButton, 
                                    QPushButton* plotMonthlyButton);

    static QGroupBox* createSummaryGroup(QWidget* parent, 
                                       QPushButton* viewWeeklySummaryButton,
                                       QPushButton* viewMonthlySummaryButton,
                                       QPushButton* viewAllTransactionsButton);
};

} // namespace FinanceManager 