#ifndef SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <memory>

// Forward declarations
class ComputationController;
namespace sol {
    class state;
}

/**
 * @brief ScriptEngine manages Lua script execution
 * 
 * This class provides:
 * - Lua state management
 * - C++ API binding to Lua
 * - Script execution in separate thread
 * - Error handling and logging
 * - Script interruption support
 */
class ScriptEngine : public QObject {
    Q_OBJECT
    
public:
    /**
     * @brief Constructor
     * @param controller Computation controller for API binding
     * @param parent Parent QObject
     */
    explicit ScriptEngine(ComputationController* controller, QObject* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~ScriptEngine();
    
    /**
     * @brief Execute Lua script code
     * @param code Lua script code to execute
     */
    void executeScript(const QString& code);
    
    /**
     * @brief Stop currently running script
     */
    void stopScript();
    
    /**
     * @brief Check if script is currently running
     * @return true if script is running
     */
    bool isRunning() const;
    
signals:
    /**
     * @brief Emitted when script outputs text
     * @param text Output text
     */
    void scriptOutput(const QString& text);
    
    /**
     * @brief Emitted when script encounters an error
     * @param error Error message
     */
    void scriptError(const QString& error);
    
    /**
     * @brief Emitted when script execution starts
     */
    void scriptStarted();
    
    /**
     * @brief Emitted when script execution finishes
     */
    void scriptFinished();
    
private:
    /**
     * @brief Initialize Lua state and bind APIs
     */
    void initializeLua();
    
    /**
     * @brief Bind C++ APIs to Lua
     */
    void bindAPIs();
    
    /**
     * @brief Lua print function implementation
     * @param message Message to print
     */
    void luaPrint(const QString& message);
    
    std::unique_ptr<sol::state> m_lua;      ///< Lua state
    ComputationController* m_controller;     ///< Computation controller
    mutable QMutex m_mutex;                  ///< Mutex for thread safety
    bool m_running;                          ///< Script running flag
    bool m_stopRequested;                    ///< Stop request flag
};

#endif // SCRIPT_ENGINE_H
