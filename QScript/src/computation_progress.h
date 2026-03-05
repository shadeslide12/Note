#ifndef COMPUTATION_PROGRESS_H
#define COMPUTATION_PROGRESS_H

#include "computation_state.h"

/**
 * @brief Structure representing the progress of a computation
 * 
 * Contains current iteration, total iterations, and computation state.
 * Provides utility methods for calculating percentage and validating consistency.
 * 
 * Validates: Requirements 7.1, 7.2
 */
struct ComputationProgress {
    int currentIteration;    ///< Current completed iteration [0, totalIterations]
    int totalIterations;     ///< Total number of iterations (> 0)
    ComputationState state;  ///< Current computation state
    
    /**
     * @brief Calculate completion percentage
     * @return Percentage of completion (0.0 to 100.0)
     */
    double percentage() const {
        if (totalIterations == 0) return 0.0;
        return (static_cast<double>(currentIteration) / totalIterations) * 100.0;
    }
    
    /**
     * @brief Check if computation is completed
     * @return true if all iterations are done
     */
    bool isCompleted() const {
        return currentIteration >= totalIterations;
    }
    
    /**
     * @brief Validate data consistency
     * @return true if the progress data is valid
     */
    bool isValid() const {
        return totalIterations > 0 && 
               currentIteration >= 0 && 
               currentIteration <= totalIterations;
    }
};

#endif // COMPUTATION_PROGRESS_H
