#include "monitor_widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFile>
#include <QTextStream>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

MonitorWidget::MonitorWidget(QWidget* parent)
    : QWidget(parent)
    , m_stateLabel(nullptr)
    , m_iterationLabel(nullptr)
    , m_progressBar(nullptr)
    , m_percentageLabel(nullptr)
    , m_errorLabel(nullptr)
    , m_chartView(nullptr)
    , m_chart(nullptr)
    , m_temperatureSeries(nullptr)
    , m_axisX(nullptr)
    , m_axisY(nullptr)
    , m_refreshTimer(nullptr)
    , m_currentState(ComputationState::Not_Started)
    , m_currentIteration(0)
    , m_totalIterations(0)
    , m_lastError("")
{
    setupUI();
    setupChart();
    
    // Create and configure refresh timer
    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(REFRESH_INTERVAL_MS);
    connect(m_refreshTimer, &QTimer::timeout, this, &MonitorWidget::refreshDisplay);
    m_refreshTimer->start();
}

void MonitorWidget::setupUI() {
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Create group box for monitor display
    QGroupBox* monitorGroup = new QGroupBox("Computation Monitor", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(monitorGroup);
    
    // State label
    m_stateLabel = new QLabel("State: Not Started", this);
    m_stateLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    groupLayout->addWidget(m_stateLabel);
    
    // Iteration label
    m_iterationLabel = new QLabel("Iteration: 0 / 0", this);
    groupLayout->addWidget(m_iterationLabel);
    
    // Progress bar
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(false);
    groupLayout->addWidget(m_progressBar);
    
    // Percentage label
    m_percentageLabel = new QLabel("Progress: 0.00%", this);
    groupLayout->addWidget(m_percentageLabel);
    
    // Error label (initially hidden)
    m_errorLabel = new QLabel("", this);
    m_errorLabel->setStyleSheet("color: red; font-weight: bold;");
    m_errorLabel->setWordWrap(true);
    m_errorLabel->setVisible(false);
    groupLayout->addWidget(m_errorLabel);
    
    mainLayout->addWidget(monitorGroup);
    
    // Add chart view
    m_chartView = new QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumHeight(300);
    mainLayout->addWidget(m_chartView);
    
    mainLayout->addStretch();
    
    setLayout(mainLayout);
}

void MonitorWidget::updateState(ComputationState state) {
    m_currentState = state;
    
    // Clear chart when starting new computation
    if (state == ComputationState::Running && m_currentIteration == 0) {
        clearChart();
    }
    // Display will be refreshed by timer
}

void MonitorWidget::updateProgress(int current, int total) {
    m_currentIteration = current;
    m_totalIterations = total;
    // Display will be refreshed by timer
}

void MonitorWidget::displayError(const QString& error) {
    m_lastError = error;
    // Display will be refreshed by timer
}

void MonitorWidget::refreshDisplay() {
    // Update state label
    m_stateLabel->setText(QString("State: %1").arg(stateToString(m_currentState)));
    
    // Update iteration label
    m_iterationLabel->setText(QString("Iteration: %1 / %2")
        .arg(m_currentIteration)
        .arg(m_totalIterations));
    
    // Update progress bar
    double percentage = calculatePercentage(m_currentIteration, m_totalIterations);
    m_progressBar->setValue(static_cast<int>(percentage));
    
    // Update percentage label
    m_percentageLabel->setText(QString("Progress: %1%")
        .arg(percentage, 0, 'f', 2));
    
    // Update error label
    if (!m_lastError.isEmpty()) {
        m_errorLabel->setText(QString("Error: %1").arg(m_lastError));
        m_errorLabel->setVisible(true);
    } else {
        m_errorLabel->setVisible(false);
    }
}

QString MonitorWidget::stateToString(ComputationState state) const {
    switch (state) {
        case ComputationState::Not_Started:
            return "Not Started";
        case ComputationState::Running:
            return "Running";
        case ComputationState::Stopped:
            return "Stopped";
        case ComputationState::Completed:
            return "Completed";
        default:
            return "Unknown";
    }
}

double MonitorWidget::calculatePercentage(int current, int total) const {
    if (total == 0) {
        return 0.0;
    }
    return (static_cast<double>(current) / total) * 100.0;
}

void MonitorWidget::updateTemperature(int iteration, double temperature)
{
    // Add data point to temperature series
    if (m_temperatureSeries) {
        m_temperatureSeries->append(iteration, temperature);
        
        // Update axis ranges dynamically
        if (m_axisX && m_axisY) {
            // Update X axis to show all iterations
            if (iteration > m_axisX->max()) {
                m_axisX->setRange(0, iteration + 10);
            }
            
            // Update Y axis to fit temperature range with some padding
            QList<QPointF> points = m_temperatureSeries->points();
            if (!points.isEmpty()) {
                double minTemp = temperature;
                double maxTemp = temperature;
                for (const QPointF& point : points) {
                    minTemp = qMin(minTemp, point.y());
                    maxTemp = qMax(maxTemp, point.y());
                }
                double padding = (maxTemp - minTemp) * 0.1;
                if (padding < 1.0) padding = 10.0;
                m_axisY->setRange(minTemp - padding, maxTemp + padding);
            }
        }
    }
}

void MonitorWidget::setupChart()
{
    // Create chart
    m_chart = new QChart();
    m_chart->setTitle("Temperature Field Evolution");
    m_chart->setAnimationOptions(QChart::NoAnimation);
    
    // Create temperature series
    m_temperatureSeries = new QLineSeries();
    m_temperatureSeries->setName("Temperature (K)");
    m_chart->addSeries(m_temperatureSeries);
    
    // Create axes
    m_axisX = new QValueAxis();
    m_axisX->setTitleText("Iteration");
    m_axisX->setLabelFormat("%d");
    m_axisX->setRange(0, 100);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_temperatureSeries->attachAxis(m_axisX);
    
    m_axisY = new QValueAxis();
    m_axisY->setTitleText("Temperature (K)");
    m_axisY->setLabelFormat("%.1f");
    m_axisY->setRange(280, 310);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_temperatureSeries->attachAxis(m_axisY);
    
    // Set chart to view
    m_chartView->setChart(m_chart);
}

void MonitorWidget::clearChart()
{
    if (m_temperatureSeries) {
        m_temperatureSeries->clear();
    }
    if (m_axisX) {
        m_axisX->setRange(0, 100);
    }
    if (m_axisY) {
        m_axisY->setRange(280, 310);
    }
}

bool MonitorWidget::saveResults(const QString& filename)
{
    // Open file for writing
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    
    // Write CSV header
    out << "Iteration,Temperature(K)\n";
    
    // Write data points from temperature series
    if (m_temperatureSeries) {
        QList<QPointF> points = m_temperatureSeries->points();
        for (const QPointF& point : points) {
            out << static_cast<int>(point.x()) << "," 
                << point.y() << "\n";
        }
    }
    
    file.close();
    return true;
}
