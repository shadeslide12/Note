#include "main_window.h"
#include "monitor_widget.h"
#include "computation_controller.h"
#include "script_engine.h"
#include "script_editor_widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QDoubleSpinBox>
#include <climits>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_startButton(nullptr)
    , m_stopButton(nullptr)
    , m_continueButton(nullptr)
    , m_saveButton(nullptr)
    , m_iterationSpinBox(nullptr)
    , m_inletPressureSpinBox(nullptr)
    , m_inletTempSpinBox(nullptr)
    , m_ambientTempSpinBox(nullptr)
    , m_monitorWidget(nullptr)
    , m_statusLabel(nullptr)
    , m_tabWidget(nullptr)
    , m_controller(nullptr)
    , m_scriptEngine(nullptr)
    , m_scriptEditor(nullptr)
{
    setupUI();
    
    // Create computation controller
    m_controller = new ComputationController(this);
    
    // Create script engine
    m_scriptEngine = new ScriptEngine(m_controller, this);
    
    // Create script editor widget
    m_scriptEditor = new ScriptEditorWidget(m_scriptEngine, this);
    
    // Add script editor to tab widget
    m_tabWidget->addTab(m_scriptEditor, "Script");
    
    connectSignals();
    
    // Initialize UI state
    updateButtonStates(ComputationState::Not_Started);
}

MainWindow::~MainWindow()
{
    // Qt parent-child relationship handles cleanup
}

void MainWindow::setupUI()
{
    // Set window properties
    setWindowTitle("CFD Solver GUI");
    setMinimumSize(500, 400);
    
    // Create central widget and main layout
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Create control group box
    QGroupBox* controlGroup = new QGroupBox("Computation Control", this);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);
    
    // Iteration input row
    QHBoxLayout* iterationLayout = new QHBoxLayout();
    QLabel* iterationLabel = new QLabel("Iterations:", this);
    m_iterationSpinBox = new QSpinBox(this);
    m_iterationSpinBox->setRange(1, INT_MAX);
    m_iterationSpinBox->setValue(100);
    m_iterationSpinBox->setMinimumWidth(150);
    iterationLayout->addWidget(iterationLabel);
    iterationLayout->addWidget(m_iterationSpinBox);
    iterationLayout->addStretch();
    controlLayout->addLayout(iterationLayout);
    
    // Inlet pressure input row
    QHBoxLayout* pressureLayout = new QHBoxLayout();
    QLabel* pressureLabel = new QLabel("Inlet Pressure (Pa):", this);
    m_inletPressureSpinBox = new QDoubleSpinBox(this);
    m_inletPressureSpinBox->setRange(0.0, 1000000.0);
    m_inletPressureSpinBox->setValue(101325.0);
    m_inletPressureSpinBox->setDecimals(1);
    m_inletPressureSpinBox->setMinimumWidth(150);
    pressureLayout->addWidget(pressureLabel);
    pressureLayout->addWidget(m_inletPressureSpinBox);
    pressureLayout->addStretch();
    controlLayout->addLayout(pressureLayout);
    
    // Inlet temperature input row
    QHBoxLayout* inletTempLayout = new QHBoxLayout();
    QLabel* inletTempLabel = new QLabel("Inlet Temperature (K):", this);
    m_inletTempSpinBox = new QDoubleSpinBox(this);
    m_inletTempSpinBox->setRange(0.0, 1000.0);
    m_inletTempSpinBox->setValue(300.0);
    m_inletTempSpinBox->setDecimals(1);
    m_inletTempSpinBox->setMinimumWidth(150);
    inletTempLayout->addWidget(inletTempLabel);
    inletTempLayout->addWidget(m_inletTempSpinBox);
    inletTempLayout->addStretch();
    controlLayout->addLayout(inletTempLayout);
    
    // Ambient temperature input row
    QHBoxLayout* ambientTempLayout = new QHBoxLayout();
    QLabel* ambientTempLabel = new QLabel("Ambient Temperature (K):", this);
    m_ambientTempSpinBox = new QDoubleSpinBox(this);
    m_ambientTempSpinBox->setRange(0.0, 1000.0);
    m_ambientTempSpinBox->setValue(293.0);
    m_ambientTempSpinBox->setDecimals(1);
    m_ambientTempSpinBox->setMinimumWidth(150);
    ambientTempLayout->addWidget(ambientTempLabel);
    ambientTempLayout->addWidget(m_ambientTempSpinBox);
    ambientTempLayout->addStretch();
    controlLayout->addLayout(ambientTempLayout);
    
    // Button row
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_startButton = new QPushButton("Start", this);
    m_stopButton = new QPushButton("Stop", this);
    m_continueButton = new QPushButton("Continue", this);
    m_saveButton = new QPushButton("Save", this);
    
    m_startButton->setMinimumWidth(100);
    m_stopButton->setMinimumWidth(100);
    m_continueButton->setMinimumWidth(100);
    m_saveButton->setMinimumWidth(100);
    
    buttonLayout->addWidget(m_startButton);
    buttonLayout->addWidget(m_stopButton);
    buttonLayout->addWidget(m_continueButton);
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addStretch();
    controlLayout->addLayout(buttonLayout);
    
    mainLayout->addWidget(controlGroup);
    
    // Create tab widget for main content
    m_tabWidget = new QTabWidget(this);
    
    // Create monitor widget
    m_monitorWidget = new MonitorWidget(this);
    m_tabWidget->addTab(m_monitorWidget, "Monitor");
    
    mainLayout->addWidget(m_tabWidget);
    
    // Status label
    m_statusLabel = new QLabel("", this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet("color: blue; font-style: italic;");
    mainLayout->addWidget(m_statusLabel);
    
    mainLayout->addStretch();
    
    setCentralWidget(centralWidget);
}

void MainWindow::connectSignals()
{
    // Connect button clicks to slots
    connect(m_startButton, &QPushButton::clicked,
            this, &MainWindow::onStartClicked);
    connect(m_stopButton, &QPushButton::clicked,
            this, &MainWindow::onStopClicked);
    connect(m_continueButton, &QPushButton::clicked,
            this, &MainWindow::onContinueClicked);
    connect(m_saveButton, &QPushButton::clicked,
            this, &MainWindow::onSaveClicked);
    
    // Connect iteration spinbox value change
    connect(m_iterationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::onIterationChanged);
    
    // Connect controller signals to UI update slots
    connect(m_controller, &ComputationController::stateChanged,
            this, &MainWindow::onStateChanged);
    connect(m_controller, &ComputationController::errorOccurred,
            this, &MainWindow::onErrorOccurred);
    
    // Connect controller signals to MonitorWidget
    connect(m_controller, &ComputationController::stateChanged,
            m_monitorWidget, &MonitorWidget::updateState);
    connect(m_controller, &ComputationController::progressUpdated,
            m_monitorWidget, &MonitorWidget::updateProgress);
    connect(m_controller, &ComputationController::temperatureUpdated,
            m_monitorWidget, &MonitorWidget::updateTemperature);
    connect(m_controller, &ComputationController::errorOccurred,
            m_monitorWidget, &MonitorWidget::displayError);
}

void MainWindow::onStartClicked()
{
    // Read parameters from spinboxes
    int iterations = m_iterationSpinBox->value();
    double inletPressure = m_inletPressureSpinBox->value();
    double inletTemp = m_inletTempSpinBox->value();
    double ambientTemp = m_ambientTempSpinBox->value();
    
    // Call controller to start computation
    m_controller->start(iterations, inletPressure, inletTemp, ambientTemp);
    
    // Update status
    m_statusLabel->setText(QString("Starting computation with %1 iterations").arg(iterations));
}

void MainWindow::onStopClicked()
{
    // Call controller to stop computation
    m_controller->stop();
    
    // Update status
    m_statusLabel->setText("Stopping computation...");
}

void MainWindow::onContinueClicked()
{
    // Call controller to continue computation
    m_controller->continueComputation();
    
    // Update status
    m_statusLabel->setText("Continuing computation...");
}

void MainWindow::onIterationChanged(int value)
{
    // Validation is handled by QSpinBox range (1 to INT_MAX)
    // This slot is called when value changes
    // No additional action needed as value is already validated
    Q_UNUSED(value);
}

void MainWindow::onStateChanged(ComputationState state)
{
    // Update button states based on new computation state
    updateButtonStates(state);
    
    // Update status label based on state
    switch (state) {
        case ComputationState::Not_Started:
            m_statusLabel->setText("Ready");
            break;
        case ComputationState::Running:
            m_statusLabel->setText("Computing...");
            break;
        case ComputationState::Stopped:
            m_statusLabel->setText("Stopped");
            break;
        case ComputationState::Completed:
            m_statusLabel->setText("Completed!");
            break;
    }
}

void MainWindow::onErrorOccurred(const QString& error)
{
    // Display error in status label
    m_statusLabel->setText(QString("Error: %1").arg(error));
    m_statusLabel->setStyleSheet("color: red; font-weight: bold;");
    
    // Show error message box
    QMessageBox::critical(this, "Computation Error", 
                         QString("An error occurred during computation:\n\n%1\n\nYou can modify parameters and restart.").arg(error));
    
    // Reset status label style
    m_statusLabel->setStyleSheet("color: blue; font-style: italic;");
}

void MainWindow::onSaveClicked()
{
    // Use QFileDialog to let user choose save location
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Save Results",
        "results.csv",
        "CSV Files (*.csv);;All Files (*)"
    );
    
    // If user cancelled, filename will be empty
    if (filename.isEmpty()) {
        return;
    }
    
    // Call MonitorWidget to save results
    bool success = m_monitorWidget->saveResults(filename);
    
    if (success) {
        m_statusLabel->setText(QString("Results saved to: %1").arg(filename));
        QMessageBox::information(this, "Save Successful", 
                                QString("Results successfully saved to:\n%1").arg(filename));
    } else {
        m_statusLabel->setText("Failed to save results");
        m_statusLabel->setStyleSheet("color: red; font-weight: bold;");
        QMessageBox::critical(this, "Save Failed", 
                             "Failed to save results. Please check file permissions and try again.");
        m_statusLabel->setStyleSheet("color: blue; font-style: italic;");
    }
}

void MainWindow::updateButtonStates(ComputationState state)
{
    // Update button and spinbox enabled/disabled states based on computation state
    // Rules:
    // - Not_Started: Start enabled, Stop/Continue disabled, Save disabled, All inputs enabled
    // - Running: Stop enabled, Start/Continue/Save disabled, All inputs disabled
    // - Stopped: Continue enabled, Start/Stop/Save disabled, All inputs enabled
    // - Completed: Start/Save enabled, Stop/Continue disabled, All inputs enabled
    
    switch (state) {
        case ComputationState::Not_Started:
            m_startButton->setEnabled(true);
            m_stopButton->setEnabled(false);
            m_continueButton->setEnabled(false);
            m_saveButton->setEnabled(false);
            m_iterationSpinBox->setEnabled(true);
            m_inletPressureSpinBox->setEnabled(true);
            m_inletTempSpinBox->setEnabled(true);
            m_ambientTempSpinBox->setEnabled(true);
            break;
            
        case ComputationState::Running:
            m_startButton->setEnabled(false);
            m_stopButton->setEnabled(true);
            m_continueButton->setEnabled(false);
            m_saveButton->setEnabled(false);
            m_iterationSpinBox->setEnabled(false);
            m_inletPressureSpinBox->setEnabled(false);
            m_inletTempSpinBox->setEnabled(false);
            m_ambientTempSpinBox->setEnabled(false);
            break;
            
        case ComputationState::Stopped:
            m_startButton->setEnabled(false);
            m_stopButton->setEnabled(false);
            m_continueButton->setEnabled(true);
            m_saveButton->setEnabled(false);
            m_iterationSpinBox->setEnabled(true);
            m_inletPressureSpinBox->setEnabled(true);
            m_inletTempSpinBox->setEnabled(true);
            m_ambientTempSpinBox->setEnabled(true);
            break;
            
        case ComputationState::Completed:
            m_startButton->setEnabled(true);
            m_stopButton->setEnabled(false);
            m_continueButton->setEnabled(false);
            m_saveButton->setEnabled(true);
            m_iterationSpinBox->setEnabled(true);
            m_inletPressureSpinBox->setEnabled(true);
            m_inletTempSpinBox->setEnabled(true);
            m_ambientTempSpinBox->setEnabled(true);
            break;
    }
}
