#include "script_engine.h"
#include "computation_controller.h"
#include "computation_state.h"

#define SOL_ALL_SAFETIES_ON 1
#include <sol.hpp>

#include <QThread>
#include <QMutexLocker>

ScriptEngine::ScriptEngine(ComputationController* controller, QObject* parent)
    : QObject(parent)
    , m_controller(controller)
    , m_running(false)
    , m_stopRequested(false)
{
    initializeLua();
}

ScriptEngine::~ScriptEngine()
{
    stopScript();
}

void ScriptEngine::initializeLua()
{
    m_lua = std::make_unique<sol::state>();
    m_lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
    
    bindAPIs();
}

void ScriptEngine::bindAPIs()
{
    // Create solver table
    auto solver = m_lua->create_table("solver");
    
    // Bind setIterations
    solver["setIterations"] = [this](int iterations) {
        if (m_controller) {
            // This will be used when starting computation
            luaPrint(QString("Set iterations to: %1").arg(iterations));
        }
    };
    
    // Bind setInletPressure
    solver["setInletPressure"] = [this](double pressure) {
        luaPrint(QString("Set inlet pressure to: %1 Pa").arg(pressure));
    };
    
    // Bind setInletTemperature
    solver["setInletTemperature"] = [this](double temperature) {
        luaPrint(QString("Set inlet temperature to: %1 K").arg(temperature));
    };
    
    // Bind setAmbientTemperature
    solver["setAmbientTemperature"] = [this](double temperature) {
        luaPrint(QString("Set ambient temperature to: %1 K").arg(temperature));
    };
    
    // Bind start
    solver["start"] = [this](int iterations, sol::optional<double> inletPressure, 
                             sol::optional<double> inletTemp, sol::optional<double> ambientTemp) {
        if (m_controller) {
            double pressure = inletPressure.value_or(101325.0);
            double inlet = inletTemp.value_or(300.0);
            double ambient = ambientTemp.value_or(293.0);
            
            luaPrint(QString("Starting computation: iterations=%1, pressure=%2, inlet_temp=%3, ambient_temp=%4")
                    .arg(iterations).arg(pressure).arg(inlet).arg(ambient));
            
            // Use QMetaObject::invokeMethod for thread-safe call
            QMetaObject::invokeMethod(m_controller, "start", Qt::QueuedConnection,
                                     Q_ARG(int, iterations),
                                     Q_ARG(double, pressure),
                                     Q_ARG(double, inlet),
                                     Q_ARG(double, ambient));
        }
    };
    
    // Bind stop
    solver["stop"] = [this]() {
        if (m_controller) {
            luaPrint("Stopping computation...");
            QMetaObject::invokeMethod(m_controller, "stop", Qt::QueuedConnection);
        }
    };
    
    // Bind continue
    solver["continue"] = [this]() {
        if (m_controller) {
            luaPrint("Continuing computation...");
            QMetaObject::invokeMethod(m_controller, "continueComputation", Qt::QueuedConnection);
        }
    };
    
    // Bind getState
    solver["getState"] = [this]() -> std::string {
        if (m_controller) {
            ComputationState state = m_controller->currentState();
            switch (state) {
                case ComputationState::Not_Started: return "Not_Started";
                case ComputationState::Running: return "Running";
                case ComputationState::Stopped: return "Stopped";
                case ComputationState::Completed: return "Completed";
                default: return "Unknown";
            }
        }
        return "Unknown";
    };
    
    // Bind getCurrentIteration
    solver["getCurrentIteration"] = [this]() -> int {
        if (m_controller) {
            return m_controller->currentIteration();
        }
        return 0;
    };
    
    // Bind saveResults
    solver["saveResults"] = [this](const std::string& filename) {
        luaPrint(QString("Save results to: %1").arg(QString::fromStdString(filename)));
        // Note: Actual save functionality would need to be exposed through controller
        // For now, just log the request
    };
    
    // Bind print function
    (*m_lua)["print"] = [this](sol::variadic_args va) {
        QString output;
        for (auto v : va) {
            if (!output.isEmpty()) {
                output += "\t";
            }
            output += QString::fromStdString(m_lua->do_string("return tostring(...)", v).get<std::string>());
        }
        luaPrint(output);
    };
    
    // Bind sleep function for scripts
    (*m_lua)["sleep"] = [](int milliseconds) {
        QThread::msleep(milliseconds);
    };
}

void ScriptEngine::executeScript(const QString& code)
{
    QMutexLocker locker(&m_mutex);
    
    if (m_running) {
        emit scriptError("Script is already running");
        return;
    }
    
    m_running = true;
    m_stopRequested = false;
    locker.unlock();
    
    emit scriptStarted();
    
    // Execute in a separate thread to avoid blocking GUI
    QThread* thread = QThread::create([this, code]() {
        try {
            // Execute the script
            m_lua->script(code.toStdString());
            
            if (!m_stopRequested) {
                emit scriptOutput("Script completed successfully");
            }
        } catch (const sol::error& e) {
            if (!m_stopRequested) {
                emit scriptError(QString("Lua error: %1").arg(e.what()));
            }
        } catch (const std::exception& e) {
            if (!m_stopRequested) {
                emit scriptError(QString("Error: %1").arg(e.what()));
            }
        }
        
        QMutexLocker locker(&m_mutex);
        m_running = false;
        emit scriptFinished();
    });
    
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

void ScriptEngine::stopScript()
{
    QMutexLocker locker(&m_mutex);
    
    if (m_running) {
        m_stopRequested = true;
        luaPrint("Script stop requested...");
        // Note: Lua doesn't have built-in interruption mechanism
        // The script will stop at the next safe point
    }
}

bool ScriptEngine::isRunning() const
{
    QMutexLocker locker(&m_mutex);
    return m_running;
}

void ScriptEngine::luaPrint(const QString& message)
{
    emit scriptOutput(message);
}
