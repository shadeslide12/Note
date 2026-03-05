#include "computation_controller.h"
#include "cfd_solver_thread.h"
#include <QMutexLocker>

ComputationController::ComputationController(QObject* parent)
    : QObject(parent)
    , m_state(ComputationState::Not_Started)
    , m_currentIteration(0)
    , m_totalIterations(0)
    , m_solverThread(nullptr)
    , m_inletPressure(101325.0)
    , m_inletTemperature(300.0)
    , m_ambientTemperature(293.0)
{
    // Create solver thread instance
    m_solverThread = new CFDSolverThread(this);
    
    // Connect solver thread signals to controller slots
    connect(m_solverThread, &CFDSolverThread::progressUpdated,
            this, &ComputationController::onProgressUpdate);
    connect(m_solverThread, &CFDSolverThread::temperatureUpdated,
            this, &ComputationController::temperatureUpdated);
    connect(m_solverThread, &CFDSolverThread::computationCompleted,
            this, &ComputationController::onComputationCompleted);
    connect(m_solverThread, &CFDSolverThread::computationError,
            this, &ComputationController::onComputationError);
    
    // Connect controller signals to solver thread slots
    connect(this, &ComputationController::startSolver,
            m_solverThread, &CFDSolverThread::startComputation);
    connect(this, &ComputationController::stopSolver,
            m_solverThread, &CFDSolverThread::stopComputation);
}

ComputationController::~ComputationController()
{
    // Stop the thread if it's running
    if (m_solverThread && m_solverThread->isRunning()) {
        m_solverThread->stopComputation();
        m_solverThread->wait(5000); // Wait up to 5 seconds
    }
}

ComputationState ComputationController::currentState() const
{
    QMutexLocker locker(&m_stateMutex);
    return m_state;
}

int ComputationController::currentIteration() const
{
    QMutexLocker locker(&m_stateMutex);
    return m_currentIteration;
}

int ComputationController::totalIterations() const
{
    QMutexLocker locker(&m_stateMutex);
    return m_totalIterations;
}

void ComputationController::start(int iterations, double inletPressure, 
                                  double inletTemperature, double ambientTemperature)
{
    QMutexLocker locker(&m_stateMutex);
    
    // Validate current state is Not_Started
    if (m_state != ComputationState::Not_Started) {
        return; // Ignore invalid start request
    }
    
    // Save total iterations and temperature parameters
    m_totalIterations = iterations;
    m_currentIteration = 0;
    m_inletPressure = inletPressure;
    m_inletTemperature = inletTemperature;
    m_ambientTemperature = ambientTemperature;
    
    // Transition to Running state
    transitionToState(ComputationState::Running);
    
    // Emit signal to start solver (from iteration 0)
    locker.unlock(); // Unlock before emitting signal
    emit startSolver(iterations, 0, inletPressure, inletTemperature, ambientTemperature);
}

void ComputationController::stop()
{
    QMutexLocker locker(&m_stateMutex);
    
    // Validate current state is Running
    if (m_state != ComputationState::Running) {
        return; // Ignore invalid stop request
    }
    
    // Transition to Stopped state
    transitionToState(ComputationState::Stopped);
    
    // Emit signal to stop solver
    locker.unlock(); // Unlock before emitting signal
    emit stopSolver();
}

void ComputationController::continueComputation()
{
    QMutexLocker locker(&m_stateMutex);
    
    // Validate current state is Stopped
    if (m_state != ComputationState::Stopped) {
        return; // Ignore invalid continue request
    }
    
    // Save current iteration position
    int startFrom = m_currentIteration;
    
    // Transition to Running state
    transitionToState(ComputationState::Running);
    
    // Emit signal to start solver from current position with saved parameters
    locker.unlock(); // Unlock before emitting signal
    emit startSolver(m_totalIterations, startFrom, m_inletPressure, 
                    m_inletTemperature, m_ambientTemperature);
}

void ComputationController::onProgressUpdate(int current, int total)
{
    {
        QMutexLocker locker(&m_stateMutex);
        m_currentIteration = current;
        m_totalIterations = total;
    }
    
    // Forward progress signal
    emit progressUpdated(current, total);
}

void ComputationController::onComputationCompleted()
{
    QMutexLocker locker(&m_stateMutex);
    
    // Transition to Completed state
    transitionToState(ComputationState::Completed);
}

void ComputationController::onComputationError(const QString& error)
{
    QMutexLocker locker(&m_stateMutex);
    
    // Transition to Stopped state on error
    transitionToState(ComputationState::Stopped);
    
    // Emit error signal
    locker.unlock(); // Unlock before emitting signal
    emit errorOccurred(error);
}

void ComputationController::transitionToState(ComputationState newState)
{
    // Note: Caller must hold m_stateMutex lock
    
    // Validate transition
    if (!isValidTransition(m_state, newState)) {
        return; // Invalid transition, ignore
    }
    
    // Update state
    ComputationState oldState = m_state;
    m_state = newState;
    
    // Emit state changed signal (must be done without lock to avoid deadlock)
    // We'll emit after returning from this function
    if (oldState != newState) {
        // Use QMetaObject::invokeMethod to emit signal in a thread-safe way
        QMetaObject::invokeMethod(this, [this, newState]() {
            emit stateChanged(newState);
        }, Qt::QueuedConnection);
    }
}

bool ComputationController::isValidTransition(ComputationState from, ComputationState to) const
{
    // Valid state transitions:
    // Not_Started → Running
    // Running → Stopped
    // Running → Completed
    // Stopped → Running
    
    if (from == to) {
        return false; // No self-transitions
    }
    
    switch (from) {
        case ComputationState::Not_Started:
            return to == ComputationState::Running;
            
        case ComputationState::Running:
            return to == ComputationState::Stopped || 
                   to == ComputationState::Completed;
            
        case ComputationState::Stopped:
            return to == ComputationState::Running;
            
        case ComputationState::Completed:
            return false; // No transitions from Completed
            
        default:
            return false;
    }
}
