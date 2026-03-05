#ifndef ERROR_INFO_H
#define ERROR_INFO_H

#include <QString>
#include <QDateTime>

/**
 * @brief Structure containing error information
 * 
 * Captures error details including message, timestamp, component name,
 * and iteration number where the error occurred.
 * 
 * Validates: Requirements 7.1, 7.2
 */
struct ErrorInfo {
    QString message;           ///< Error message description
    QDateTime timestamp;       ///< Time when error occurred
    QString component;         ///< Name of component where error occurred
    int iterationNumber;       ///< Iteration number when error occurred (-1 if not applicable)
    
    /**
     * @brief Convert error info to formatted string
     * @return Formatted error string with timestamp, component, message, and iteration
     */
    QString toString() const {
        return QString("[%1] %2: %3 (Iteration: %4)")
            .arg(timestamp.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(component)
            .arg(message)
            .arg(iterationNumber);
    }
};

#endif // ERROR_INFO_H
