#include "cfd_solver.h"
#include <QThread>

CFDSolver::CFDSolver(QObject* parent)
    : QObject(parent)
    , m_inletPressure(101325.0)
    , m_inletTemperature(300.0)
    , m_ambientTemperature(293.0)
    , m_currentTemperature(293.0)
{
}

CFDSolver::~CFDSolver()
{
}

void CFDSolver::compute(int totalIterations, int startFrom, const QAtomicInt& stopFlag,
                        double inletPressure, double inletTemperature, double ambientTemperature)
{
    // Store initial conditions
    m_inletPressure = inletPressure;
    m_inletTemperature = inletTemperature;
    m_ambientTemperature = ambientTemperature;
    m_currentTemperature = ambientTemperature;
    
    try {
        // 主计算循环：从startFrom迭代到totalIterations
        for (int i = startFrom; i < totalIterations; ++i) {
            // 检查停止标志
            if (stopFlag.loadAcquire() != 0) {
                // 用户请求停止，退出循环
                return;
            }
            
            // 执行单次迭代
            performIteration(i);
            
            // 计算温度场
            double temperature = calculateTemperature(i);
            
            // 发射进度更新信号
            emit progressUpdated(i + 1, totalIterations);
            
            // 发射温度更新信号
            emit temperatureUpdated(i + 1, temperature);
        }
        
        // 所有迭代完成，发射完成信号
        emit computationCompleted();
        
    } catch (const std::exception& e) {
        // 捕获异常并发射错误信号
        emit computationError(QString("Computation error: %1").arg(e.what()));
    } catch (...) {
        // 捕获未知异常
        emit computationError("Unknown computation error occurred");
    }
}

void CFDSolver::performIteration(int iterationNumber)
{
    // 模拟CFD计算工作
    simulateComputationWork();
    
    // 这里可以添加实际的CFD计算逻辑
    // 目前只是模拟延迟
}

void CFDSolver::simulateComputationWork()
{
    // 使用QThread::msleep模拟计算耗时
    QThread::msleep(ITERATION_DELAY_MS);
}

double CFDSolver::calculateTemperature(int iteration)
{
    // 简单的温度场迭代计算公式
    // T_new = T_old * 0.95 + (inlet_temp * inlet_pressure / 101325.0) * 0.05
    double pressureRatio = m_inletPressure / 101325.0;
    double inletEffect = m_inletTemperature * pressureRatio * 0.05;
    m_currentTemperature = m_currentTemperature * 0.95 + inletEffect;
    
    return m_currentTemperature;
}
