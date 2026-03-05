#ifndef COMPUTATION_STATE_H
#define COMPUTATION_STATE_H

/**
 * @brief Enumeration of computation states in the CFD Solver system
 * 
 * State machine transitions:
 * Not_Started → Running → Stopped → Running → Completed
 * 
 * Validates: Requirements 7.1, 7.2
 */
enum class ComputationState {
    Not_Started,  ///< Initial state, computation has not been started
    Running,      ///< Computation is currently executing
    Stopped,      ///< Computation has been stopped by user, can be resumed
    Completed     ///< Computation has completed all iterations
};

#endif // COMPUTATION_STATE_H
