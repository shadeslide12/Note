#include "cfd_solver_thread.h"
#include "cfd_solver.h"

CFDSolverThread::CFDSolverThread(QObject* parent)
    : QThread(parent)
    , m_solver(nullptr)
    , m_totalIterations(0)
    , m_startFrom(0)
    , m_stopRequested(0)
    , m_inletPressure(101325.0)
    , m_inletTemperature(300.0)
    , m_ambientTemperature(293.0)
{
}

CFDSolverThread::~CFDSolverThread()
{
    // 确保线程停止
    if (isRunning()) {
        m_stopRequested.storeRelease(1);
        wait(); // 等待线程完成
    }
}

void CFDSolverThread::startComputation(int totalIterations, int startFrom,
                                       double inletPressure, double inletTemperature, double ambientTemperature)
{
    // 保存计算参数
    m_totalIterations = totalIterations;
    m_startFrom = startFrom;
    m_inletPressure = inletPressure;
    m_inletTemperature = inletTemperature;
    m_ambientTemperature = ambientTemperature;
    
    // 重置停止标志
    m_stopRequested.storeRelease(0);
    
    // 启动线程（会调用run()方法）
    start();
}

void CFDSolverThread::stopComputation()
{
    // 设置停止标志，请求中断计算
    m_stopRequested.storeRelease(1);
}

void CFDSolverThread::run()
{
    // 在工作线程中创建CFDSolver实例
    m_solver = new CFDSolver();
    
    // 连接求解器信号到线程信号，使用Qt::DirectConnection因为工作线程没有事件循环
    // DirectConnection确保信号立即传递，而QueuedConnection需要事件循环才能工作
    connect(m_solver, &CFDSolver::progressUpdated,
            this, &CFDSolverThread::progressUpdated, Qt::DirectConnection);
    connect(m_solver, &CFDSolver::temperatureUpdated,
            this, &CFDSolverThread::temperatureUpdated, Qt::DirectConnection);
    connect(m_solver, &CFDSolver::computationCompleted,
            this, &CFDSolverThread::computationCompleted, Qt::DirectConnection);
    connect(m_solver, &CFDSolver::computationError,
            this, &CFDSolverThread::computationError, Qt::DirectConnection);
    
    // 执行计算
    m_solver->compute(m_totalIterations, m_startFrom, m_stopRequested,
                     m_inletPressure, m_inletTemperature, m_ambientTemperature);
    
    // 清理求解器实例
    delete m_solver;
    m_solver = nullptr;
}
