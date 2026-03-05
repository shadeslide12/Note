#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QTabWidget>
#include "computation_state.h"

// Forward declarations
class MonitorWidget;
class ComputationController;
class ScriptEngine;
class ScriptEditorWidget;

/**
 * @brief MainWindow provides the main GUI interface
 * 
 * This class is responsible for:
 * - Providing user interface with Start/Stop/Continue buttons
 * - Managing iteration count input
 * - Handling user input and button clicks
 * - Managing UI control states based on computation state
 * - Displaying error messages
 * 
 * Validates: Requirements 2.1, 2.4, 2.5, 3.1, 3.4, 3.5, 4.1, 4.4, 4.5, 5.1-5.6, 8.1, 8.2, 8.4
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    /**
     * @brief Constructor
     * @param parent Parent widget
     */
    explicit MainWindow(QWidget* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~MainWindow();
    
private slots:
    /**
     * @brief Handle Start button click
     * 
     * Validates: Requirements 2.1
     */
    void onStartClicked();
    
    /**
     * @brief Handle Stop button click
     * 
     * Validates: Requirements 3.1
     */
    void onStopClicked();
    
    /**
     * @brief Handle Continue button click
     * 
     * Validates: Requirements 4.1
     */
    void onContinueClicked();
    
    /**
     * @brief Handle iteration count change
     * @param value New iteration count value
     * 
     * Validates: Requirements 5.1, 5.2, 5.3, 5.4
     */
    void onIterationChanged(int value);
    
    /**
     * @brief Handle computation state change
     * @param state New computation state
     * 
     * Validates: Requirements 2.4, 2.5, 3.4, 3.5, 4.4, 4.5, 5.5, 5.6
     */
    void onStateChanged(ComputationState state);
    
    /**
     * @brief Handle error occurrence
     * @param error Error message
     * 
     * Validates: Requirements 8.1, 8.2, 8.4
     */
    void onErrorOccurred(const QString& error);
    
    /**
     * @brief Handle Save button click
     * 
     * Exports computation results to CSV file
     */
    void onSaveClicked();
    
private:
    /**
     * @brief Setup UI components and layout
     */
    void setupUI();
    
    /**
     * @brief Connect all signals and slots
     */
    void connectSignals();
    
    /**
     * @brief Update button states based on computation state
     * @param state Current computation state
     * 
     * Validates: Requirements 2.4, 2.5, 3.4, 3.5, 4.4, 4.5, 5.5, 5.6
     */
    void updateButtonStates(ComputationState state);
    
    // UI Components
    QPushButton* m_startButton;         ///< Start computation button
    QPushButton* m_stopButton;          ///< Stop computation button
    QPushButton* m_continueButton;      ///< Continue computation button
    QPushButton* m_saveButton;          ///< Save results button
    QSpinBox* m_iterationSpinBox;       ///< Iteration count input
    QDoubleSpinBox* m_inletPressureSpinBox;    ///< Inlet pressure input
    QDoubleSpinBox* m_inletTempSpinBox;        ///< Inlet temperature input
    QDoubleSpinBox* m_ambientTempSpinBox;      ///< Ambient temperature input
    MonitorWidget* m_monitorWidget;     ///< Monitor display widget
    QLabel* m_statusLabel;              ///< Status message label
    QTabWidget* m_tabWidget;            ///< Tab widget for main content
    
    ComputationController* m_controller; ///< Computation controller instance
    ScriptEngine* m_scriptEngine;       ///< Script engine instance
    ScriptEditorWidget* m_scriptEditor; ///< Script editor widget
};

#endif // MAIN_WINDOW_H
