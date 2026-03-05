#ifndef COMPUTATION_CONTROLLER_H
#define COMPUTATION_CONTROLLER_H

#include <QObject>
#include <QMutex>
#include "computation_state.h"

// Forward declaration
class CFDSolverThread;

/**
 * @brief ComputationController manages the computation state machine
 * 
 * This class is responsible for:
 * - Managing the computation state machine (Not_Started → Running → Stopped → Running → Completed)
 * - Coordinating communication between GUI, Solver, and Monitor
 * - Ensuring state transition consistency and atomicity
 * - Thread-safe state management using QMutex
 * 
 * Validates: Requirements 7.1, 7.2, 7.4, 7.5
 */
class ComputationController : public QObject {
    Q_OBJECT
    
public:
    /**
     * @brief Constructor
     * @param parent Parent QObject
     */
    explicit ComputationController(QObject* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~ComputationController();
    
    /**
     * @brief Get current computation state
     * @return Current state (thread-safe)
     */
    ComputationState currentState() const;
    
    /**
     * @brief Get current iteration number
     * @return Current iteration (thread-safe)
     */
    int currentIteration() const;
    
    /**
     * @brief Get total iterations
     * @return Total iterations (thread-safe)
     */
    int totalIterations() const;
    
public slots:
    /**
     * @brief Start computation from beginning
     * @param iterations Total number of iterations to execute
     * @param inletPressure Inlet pressure (Pa)
     * @param inletTemperature Inlet temperature (K)
     * @param ambientTemperature Ambient temperature (K)
     * 
     * Validates: Requirements 2.1, 2.2, 2.3, 2.4, 2.5
     */
    void start(int iterations, double inletPressure = 101325.0, 
               double inletTemperature = 300.0, double ambientTemperature = 293.0);
    
    /**
     * @brief Stop running computation
     * 
     * Validates: Requirements 3.1, 3.2, 3.3, 3.4, 3.5
     */
    void stop();
    
    /**
     * @brief Continue stopped computation
     * 
     * Validates: Requirements 4.1, 4.2, 4.3, 4.4, 4.5
     */
    void continueComputation();
    
    /**
     * @brief Handle progress update from solver
     * @param current Current iteration number
     * @param total Total iterations
     * 
     * Validates: Requirements 1.5, 6.2, 6.5
     */
    void onProgressUpdate(int current, int total);
    
    /**
     * @brief Handle computation completion from solver
     * 
     * Validates: Requirements 1.5, 6.2, 6.5
     */
    void onComputationCompleted();
    
    /**
     * @brief Handle computation error from solver
     * @param error Error message
     * 
     * Validates: Requirements 8.1, 8.2, 8.3
     */
    void onComputationError(const QString& error);
    
signals:
    /**
     * @brief Emitted when computation state changes
     * @param newState The new computation state
     */
    void stateChanged(ComputationState newState);
    
    /**
     * @brief Emitted when computation progress updates
     * @param current Current iteration number
     * @param total Total iterations
     */
    void progressUpdated(int current, int total);
    
    /**
     * @brief Emitted when temperature updates
     * @param iteration Current iteration number
     * @param temperature Current temperature (K)
     */
    void temperatureUpdated(int iteration, double temperature);
    
    /**
     * @brief Emitted when an error occurs
     * @param error Error message
     */
    void errorOccurred(const QString& error);
    
    /**
     * @brief Signal to start solver thread
     * @param iterations Total iterations
     * @param startFrom Starting iteration position
     * @param inletPressure Inlet pressure (Pa)
     * @param inletTemperature Inlet temperature (K)
     * @param ambientTemperature Ambient temperature (K)
     */
    void startSolver(int iterations, int startFrom, double inletPressure, 
                    double inletTemperature, double ambientTemperature);
    
    /**
     * @brief Signal to stop solver thread
     */
    void stopSolver();
    
private:
    /**
     * @brief Transition to a new state
     * @param newState Target state
     * 
     * Validates: Requirements 7.4, 7.5
     */
    void transitionToState(ComputationState newState);
    
    /**
     * @brief Check if state transition is valid
     * @param from Source state
     * @param to Target state
     * @return true if transition is valid
     * 
     * Validates: Requirements 7.4, 7.5
     */
    bool isValidTransition(ComputationState from, ComputationState to) const;
    
    ComputationState m_state;           ///< Current computation state
    int m_currentIteration;             ///< Current iteration number
    int m_totalIterations;              ///< Total iterations
    mutable QMutex m_stateMutex;        ///< Mutex for thread-safe state access
    
    CFDSolverThread* m_solverThread;    ///< Solver thread instance
    
    // Temperature parameters
    double m_inletPressure;             ///< Inlet pressure (Pa)
    double m_inletTemperature;          ///< Inlet temperature (K)
    double m_ambientTemperature;        ///< Ambient temperature (K)
};

#endif // COMPUTATION_CONTROLLER_H
