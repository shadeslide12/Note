#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>
#include <QSignalSpy>
#include <QAtomicInt>
#include <QEventLoop>
#include <QTimer>
#include "cfd_solver.h"
#include "computation_controller.h"

/**
 * @brief Property 1: 迭代完整性
 * 
 * **Validates: Requirements 1.1**
 * 
 * 对于任意正整数迭代次数n，当求解器完成计算时，实际执行的迭代次数应该等于n。
 * 
 * Feature: cfd-solver-gui, Property 1: 迭代完整性
 */
RC_GTEST_PROP(SolverProperties, IterationCompleteness, ()) {
    // 生成随机正整数迭代次数（限制在1到10之间以提高测试性能）
    const int n = *rc::gen::inRange(1, 11);
    
    CFDSolver solver;
    QAtomicInt stopFlag(0);
    
    QSignalSpy progressSpy(&solver, &CFDSolver::progressUpdated);
    QSignalSpy completedSpy(&solver, &CFDSolver::computationCompleted);
    
    // 执行计算
    solver.compute(n, 0, stopFlag);
    
    // 验证完成信号被发射
    RC_ASSERT(completedSpy.count() == 1);
    
    // 验证进度信号被发射了n次
    RC_ASSERT(progressSpy.count() == n);
    
    // 验证最后一次进度更新显示完成了n次迭代
    if (progressSpy.count() > 0) {
        QList<QVariant> lastSignal = progressSpy.last();
        int finalIteration = lastSignal.at(0).toInt();
        int totalIterations = lastSignal.at(1).toInt();
        
        RC_ASSERT(finalIteration == n);
        RC_ASSERT(totalIterations == n);
    }
}

/**
 * @brief Property 2: 完成状态一致性
 * 
 * **Validates: Requirements 1.5**
 * 
 * 对于任意计算执行，当所有迭代完成时，系统状态应该转换为Completed。
 * 
 * Feature: cfd-solver-gui, Property 2: 完成状态一致性
 */
RC_GTEST_PROP(SolverProperties, CompletionStateConsistency, ()) {
    // 生成随机正整数迭代次数（限制在1到10之间以提高测试性能）
    const int n = *rc::gen::inRange(1, 11);
    
    ComputationController controller;
    
    QSignalSpy stateSpy(&controller, &ComputationController::stateChanged);
    QSignalSpy progressSpy(&controller, &ComputationController::progressUpdated);
    
    // 启动计算
    controller.start(n);
    
    // 等待计算完成（每次迭代100ms + 额外缓冲时间）
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    QObject::connect(&controller, &ComputationController::stateChanged, [&](ComputationState state) {
        if (state == ComputationState::Completed) {
            loop.quit();
        }
    });
    
    timer.start(n * 150 + 1000); // 超时保护
    loop.exec();
    
    // 验证状态转换为Completed
    RC_ASSERT(controller.currentState() == ComputationState::Completed);
    
    // 验证当前迭代次数等于总迭代次数
    RC_ASSERT(controller.currentIteration() == n);
    RC_ASSERT(controller.totalIterations() == n);
}

/**
 * @brief Property 12: 进度更新完整性
 * 
 * **Validates: Requirements 1.3, 6.2**
 * 
 * 对于任意计算执行，每完成一次迭代都应该触发至少一次进度更新信号。
 * 
 * Feature: cfd-solver-gui, Property 12: 进度更新完整性
 */
RC_GTEST_PROP(SolverProperties, ProgressUpdateCompleteness, ()) {
    // 生成随机正整数迭代次数（限制在1到10之间以提高测试性能）
    const int n = *rc::gen::inRange(1, 11);
    
    CFDSolver solver;
    QAtomicInt stopFlag(0);
    
    QSignalSpy progressSpy(&solver, &CFDSolver::progressUpdated);
    
    // 执行计算
    solver.compute(n, 0, stopFlag);
    
    // 验证进度信号被发射了n次（每次迭代一次）
    RC_ASSERT(progressSpy.count() == n);
    
    // 验证每次进度更新的迭代编号是连续的
    for (int i = 0; i < progressSpy.count(); ++i) {
        QList<QVariant> signal = progressSpy.at(i);
        int currentIteration = signal.at(0).toInt();
        int totalIterations = signal.at(1).toInt();
        
        // 验证当前迭代编号正确（从1开始）
        RC_ASSERT(currentIteration == i + 1);
        // 验证总迭代次数一致
        RC_ASSERT(totalIterations == n);
    }
}
