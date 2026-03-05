#ifndef SOLVER_CONFIGURATION_H
#define SOLVER_CONFIGURATION_H

/**
 * @brief Configuration parameters for the CFD Solver
 * 
 * Contains all configurable parameters for solver execution and monitoring.
 * Provides validation and default configuration factory method.
 * 
 * Validates: Requirements 7.1, 7.2
 */
struct SolverConfiguration {
    int iterationCount;           ///< Number of iterations, default: 100
    int iterationDelayMs;         ///< Delay per iteration in milliseconds, default: 100
    int progressUpdateInterval;   ///< Progress update interval in iterations, default: 1
    int monitorRefreshIntervalMs; ///< Monitor refresh interval in milliseconds, default: 1000
    
    /**
     * @brief Validate configuration parameters
     * @return true if all parameters are valid
     */
    bool isValid() const {
        return iterationCount > 0 && 
               iterationDelayMs >= 0 && 
               progressUpdateInterval > 0 && 
               monitorRefreshIntervalMs > 0;
    }
    
    /**
     * @brief Create default configuration
     * @return SolverConfiguration with default values
     */
    static SolverConfiguration defaultConfig() {
        return SolverConfiguration{
            .iterationCount = 100,
            .iterationDelayMs = 100,
            .progressUpdateInterval = 1,
            .monitorRefreshIntervalMs = 1000
        };
    }
};

#endif // SOLVER_CONFIGURATION_H
