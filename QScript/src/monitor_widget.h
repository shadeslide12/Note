#ifndef MONITOR_WIDGET_H
#define MONITOR_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include "computation_state.h"

QT_CHARTS_USE_NAMESPACE

/**
 * @brief MonitorWidget displays real-time computation state and progress
 * 
 * This widget is responsible for:
 * - Displaying current ComputationState (in Chinese)
 * - Showing current/total iterations
 * - Displaying progress percentage
 * - Updating via QTimer with 1 second interval
 * - Showing error messages in red text
 * 
 * Validates: Requirements 6.1, 6.2, 6.3, 6.4, 6.5, 6.6, 8.2
 */
class MonitorWidget : public QWidget {
    Q_OBJECT
    
public:
    /**
     * @brief Constructor
     * @param parent Parent widget
     */
    explicit MonitorWidget(QWidget* parent = nullptr);
    
public slots:
    /**
     * @brief Update computation state
     * @param state New computation state
     * 
     * Validates: Requirements 6.1, 6.6
     */
    void updateState(ComputationState state);
    
    /**
     * @brief Update computation progress
     * @param current Current iteration number
     * @param total Total iterations
     * 
     * Validates: Requirements 6.2, 6.3, 6.4
     */
    void updateProgress(int current, int total);
    
    /**
     * @brief Update temperature value
     * @param iteration Current iteration number
     * @param temperature Current temperature (K)
     */
    void updateTemperature(int iteration, double temperature);
    
    /**
     * @brief Display error message
     * @param error Error message to display
     * 
     * Validates: Requirements 8.2
     */
    void displayError(const QString& error);
    
    /**
     * @brief Save computation results to CSV file
     * @param filename Path to save the CSV file
     * @return true if save successful, false otherwise
     */
    bool saveResults(const QString& filename);
    
private slots:
    /**
     * @brief Refresh display with cached data
     * Called by timer every 1 second
     * 
     * Validates: Requirements 6.5
     */
    void refreshDisplay();
    
private:
    /**
     * @brief Setup UI components and layout
     */
    void setupUI();
    
    /**
     * @brief Convert state enum to Chinese string
     * @param state Computation state
     * @return Chinese string representation
     */
    QString stateToString(ComputationState state) const;
    
    /**
     * @brief Calculate completion percentage
     * @param current Current iteration
     * @param total Total iterations
     * @return Percentage (0.0 to 100.0)
     * 
     * Validates: Requirements 6.4
     */
    double calculatePercentage(int current, int total) const;
    
    /**
     * @brief Setup temperature chart
     */
    void setupChart();
    
    /**
     * @brief Clear temperature chart data
     */
    void clearChart();
    
    // UI Components
    QLabel* m_stateLabel;              ///< Displays current state
    QLabel* m_iterationLabel;          ///< Displays current/total iterations
    QProgressBar* m_progressBar;       ///< Visual progress bar
    QLabel* m_percentageLabel;         ///< Displays percentage text
    QLabel* m_errorLabel;              ///< Displays error messages in red
    
    // Chart components
    QChartView* m_chartView;           ///< Chart view widget
    QChart* m_chart;                   ///< Chart instance
    QLineSeries* m_temperatureSeries;  ///< Temperature data series
    QValueAxis* m_axisX;               ///< X axis (Iteration)
    QValueAxis* m_axisY;               ///< Y axis (Temperature)
    
    QTimer* m_refreshTimer;            ///< Timer for periodic refresh
    
    // Cached data
    ComputationState m_currentState;   ///< Cached current state
    int m_currentIteration;            ///< Cached current iteration
    int m_totalIterations;             ///< Cached total iterations
    QString m_lastError;               ///< Cached last error message
    
    static constexpr int REFRESH_INTERVAL_MS = 1000;  ///< 1 second refresh interval
};

#endif // MONITOR_WIDGET_H
