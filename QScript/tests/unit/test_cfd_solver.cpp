#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QAtomicInt>
#include <QCoreApplication>
#include "cfd_solver.h"

/**
 * @brief 测试CFDSolver完成指定次数迭代
 * 
 * 验证求解器能够完成指定次数的迭代计算，并发射正确的信号。
 * Requirements: 1.1, 1.2, 1.3
 */
TEST(CFDSolverTest, CompletesSpecifiedIterations) {
    CFDSolver solver;
    QAtomicInt stopFlag(0);
    
    // 设置信号监听
    QSignalSpy progressSpy(&solver, &CFDSolver::progressUpdated);
    QSignalSpy completedSpy(&solver, &CFDSolver::computationCompleted);
    
    // 执行10次迭代
    const int totalIterations = 10;
    solver.compute(totalIterations, 0, stopFlag);
    
    // 验证完成信号被发射
    EXPECT_EQ(completedSpy.count(), 1);
    
    // 验证进度信号被发射了10次
    EXPECT_EQ(progressSpy.count(), totalIterations);
    
    // 验证最后一次进度更新是(10, 10)
    if (progressSpy.count() > 0) {
        QList<QVariant> lastSignal = progressSpy.last();
        EXPECT_EQ(lastSignal.at(0).toInt(), totalIterations);
        EXPECT_EQ(lastSignal.at(1).toInt(), totalIterations);
    }
}

/**
 * @brief 测试进度信号正确发射
 * 
 * 验证每次迭代后都会发射progressUpdated信号，且参数正确。
 * Requirements: 1.2, 1.3
 */
TEST(CFDSolverTest, EmitsProgressSignalsCorrectly) {
    CFDSolver solver;
    QAtomicInt stopFlag(0);
    
    QSignalSpy progressSpy(&solver, &CFDSolver::progressUpdated);
    
    const int totalIterations = 5;
    solver.compute(totalIterations, 0, stopFlag);
    
    // 验证发射了5次进度信号
    EXPECT_EQ(progressSpy.count(), totalIterations);
    
    // 验证每次信号的参数正确
    for (int i = 0; i < progressSpy.count(); ++i) {
        QList<QVariant> signal = progressSpy.at(i);
        int current = signal.at(0).toInt();
        int total = signal.at(1).toInt();
        
        EXPECT_EQ(current, i + 1);
        EXPECT_EQ(total, totalIterations);
    }
}

/**
 * @brief 测试stopFlag中断机制
 * 
 * 验证设置stopFlag后，计算能够正确中断。
 * Requirements: 1.3, 1.4
 */
TEST(CFDSolverTest, StopFlagInterruptsComputation) {
    CFDSolver solver;
    QAtomicInt stopFlag(0);
    
    QSignalSpy progressSpy(&solver, &CFDSolver::progressUpdated);
    QSignalSpy completedSpy(&solver, &CFDSolver::computationCompleted);
    
    // 在另一个线程中，延迟后设置停止标志
    // 这里我们使用一个简单的方法：在计算前设置stopFlag
    // 实际测试中，应该在计算过程中设置
    
    const int totalIterations = 100;
    
    // 先让它运行几次迭代，然后停止
    // 由于我们在主线程中，我们需要一个不同的策略
    // 我们将在开始前就设置stopFlag来测试立即停止
    stopFlag.storeRelease(1);
    
    solver.compute(totalIterations, 0, stopFlag);
    
    // 验证没有发射完成信号（因为被中断了）
    EXPECT_EQ(completedSpy.count(), 0);
    
    // 验证进度信号发射次数少于总迭代次数
    EXPECT_LT(progressSpy.count(), totalIterations);
}

/**
 * @brief 测试边缘情况：1次迭代
 * 
 * 验证最小有效迭代次数（1次）能够正确执行。
 * Requirements: 1.1, 1.2
 */
TEST(CFDSolverTest, HandlesOneIteration) {
    CFDSolver solver;
    QAtomicInt stopFlag(0);
    
    QSignalSpy progressSpy(&solver, &CFDSolver::progressUpdated);
    QSignalSpy completedSpy(&solver, &CFDSolver::computationCompleted);
    
    solver.compute(1, 0, stopFlag);
    
    // 验证完成信号被发射
    EXPECT_EQ(completedSpy.count(), 1);
    
    // 验证进度信号被发射1次
    EXPECT_EQ(progressSpy.count(), 1);
    
    // 验证进度是(1, 1)
    if (progressSpy.count() > 0) {
        QList<QVariant> signal = progressSpy.at(0);
        EXPECT_EQ(signal.at(0).toInt(), 1);
        EXPECT_EQ(signal.at(1).toInt(), 1);
    }
}

/**
 * @brief 测试从中间位置继续计算
 * 
 * 验证从非零起始位置开始计算能够正确执行。
 * Requirements: 1.1, 1.2, 1.3
 */
TEST(CFDSolverTest, ContinuesFromMiddle) {
    CFDSolver solver;
    QAtomicInt stopFlag(0);
    
    QSignalSpy progressSpy(&solver, &CFDSolver::progressUpdated);
    QSignalSpy completedSpy(&solver, &CFDSolver::computationCompleted);
    
    const int totalIterations = 10;
    const int startFrom = 5;
    
    solver.compute(totalIterations, startFrom, stopFlag);
    
    // 验证完成信号被发射
    EXPECT_EQ(completedSpy.count(), 1);
    
    // 验证进度信号被发射了5次（从5到10）
    EXPECT_EQ(progressSpy.count(), totalIterations - startFrom);
    
    // 验证第一次进度更新是(6, 10)
    if (progressSpy.count() > 0) {
        QList<QVariant> firstSignal = progressSpy.first();
        EXPECT_EQ(firstSignal.at(0).toInt(), startFrom + 1);
        EXPECT_EQ(firstSignal.at(1).toInt(), totalIterations);
    }
    
    // 验证最后一次进度更新是(10, 10)
    if (progressSpy.count() > 0) {
        QList<QVariant> lastSignal = progressSpy.last();
        EXPECT_EQ(lastSignal.at(0).toInt(), totalIterations);
        EXPECT_EQ(lastSignal.at(1).toInt(), totalIterations);
    }
}

/**
 * @brief 测试边缘情况：较大迭代次数
 * 
 * 验证较大迭代次数能够正确处理（但使用较小的值以避免测试时间过长）。
 * Requirements: 1.1, 1.2
 */
TEST(CFDSolverTest, HandlesLargeIterationCount) {
    CFDSolver solver;
    QAtomicInt stopFlag(0);
    
    QSignalSpy progressSpy(&solver, &CFDSolver::progressUpdated);
    QSignalSpy completedSpy(&solver, &CFDSolver::computationCompleted);
    
    // 使用50次迭代作为"大量"迭代的测试（避免测试时间过长）
    const int totalIterations = 50;
    solver.compute(totalIterations, 0, stopFlag);
    
    // 验证完成信号被发射
    EXPECT_EQ(completedSpy.count(), 1);
    
    // 验证进度信号被发射了50次
    EXPECT_EQ(progressSpy.count(), totalIterations);
}
