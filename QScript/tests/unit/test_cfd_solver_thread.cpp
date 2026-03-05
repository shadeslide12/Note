#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QAtomicInt>
#include <QCoreApplication>
#include <QThread>
#include <QEventLoop>
#include <QTimer>
#include <QTest>
#include "cfd_solver_thread.h"

/**
 * @brief 测试线程正确启动
 * 
 * 验证CFDSolverThread能够正确启动并开始执行计算。
 * Requirements: 1.4, 3.1
 */
TEST(CFDSolverThreadTest, ThreadStartsCorrectly) {
    CFDSolverThread thread;
    
    // 设置信号监听
    QSignalSpy progressSpy(&thread, &CFDSolverThread::progressUpdated);
    
    // 启动计算
    const int totalIterations = 5;
    thread.startComputation(totalIterations, 0);
    
    // 等待进度信号，处理事件
    EXPECT_TRUE(progressSpy.wait(2000));
    // 给一点时间让所有信号到达
    QTest::qWait(100);
    
    // 验证线程已经执行（收到进度信号）
    EXPECT_GT(progressSpy.count(), 0);
}

/**
 * @brief 测试线程正确停止
 * 
 * 验证CFDSolverThread能够正确停止并清理资源。
 * Requirements: 1.4, 3.2
 */
TEST(CFDSolverThreadTest, ThreadStopsCorrectly) {
    CFDSolverThread thread;
    
    QSignalSpy completedSpy(&thread, &CFDSolverThread::computationCompleted);
    
    // 启动计算
    const int totalIterations = 5;
    thread.startComputation(totalIterations, 0);
    
    // 等待完成信号，处理事件
    EXPECT_TRUE(completedSpy.wait(2000));
    // 给一点时间让所有信号到达
    QTest::qWait(100);
    
    // 验证线程已经停止
    EXPECT_FALSE(thread.isRunning());
    
    // 验证完成信号被发射
    EXPECT_EQ(completedSpy.count(), 1);
}

/**
 * @brief 测试进度信号正确跨线程传递
 * 
 * 验证CFDSolver的progressUpdated信号能够正确传递到主线程。
 * Requirements: 1.3, 3.1
 */
TEST(CFDSolverThreadTest, ProgressSignalsCrossThread) {
    CFDSolverThread thread;
    
    QSignalSpy progressSpy(&thread, &CFDSolverThread::progressUpdated);
    QSignalSpy completedSpy(&thread, &CFDSolverThread::computationCompleted);
    
    const int totalIterations = 10;
    thread.startComputation(totalIterations, 0);
    
    // 等待完成信号，处理事件
    EXPECT_TRUE(completedSpy.wait(3000));
    // 给一点时间让所有信号到达
    QTest::qWait(100);
    
    // 验证收到了所有进度信号
    EXPECT_EQ(progressSpy.count(), totalIterations);
    
    // 验证信号参数正确
    for (int i = 0; i < progressSpy.count(); ++i) {
        QList<QVariant> signal = progressSpy.at(i);
        int current = signal.at(0).toInt();
        int total = signal.at(1).toInt();
        
        EXPECT_EQ(current, i + 1);
        EXPECT_EQ(total, totalIterations);
    }
}

/**
 * @brief 测试完成信号正确跨线程传递
 * 
 * 验证CFDSolver的computationCompleted信号能够正确传递到主线程。
 * Requirements: 1.5, 3.1
 */
TEST(CFDSolverThreadTest, CompletionSignalCrossesThread) {
    CFDSolverThread thread;
    
    QSignalSpy completedSpy(&thread, &CFDSolverThread::computationCompleted);
    
    const int totalIterations = 5;
    thread.startComputation(totalIterations, 0);
    
    // 等待完成信号，处理事件
    EXPECT_TRUE(completedSpy.wait(2000));
    // 给一点时间让所有信号到达
    QTest::qWait(100);
    
    // 验证完成信号被发射一次
    EXPECT_EQ(completedSpy.count(), 1);
}

/**
 * @brief 测试stopComputation()正确中断计算
 * 
 * 验证调用stopComputation()能够中断正在运行的计算。
 * Requirements: 3.2
 */
TEST(CFDSolverThreadTest, StopComputationInterruptsCalculation) {
    CFDSolverThread thread;
    
    QSignalSpy progressSpy(&thread, &CFDSolverThread::progressUpdated);
    QSignalSpy completedSpy(&thread, &CFDSolverThread::computationCompleted);
    
    // 启动一个较长的计算
    const int totalIterations = 100;
    thread.startComputation(totalIterations, 0);
    
    // 等待一小段时间让计算开始
    QTest::qWait(200);
    
    // 停止计算
    thread.stopComputation();
    
    // 等待线程结束，处理事件
    thread.wait(2000);
    QTest::qWait(100);
    
    // 验证没有发射完成信号（因为被中断了）
    EXPECT_EQ(completedSpy.count(), 0);
    
    // 验证进度信号发射次数少于总迭代次数
    EXPECT_LT(progressSpy.count(), totalIterations);
}

/**
 * @brief 测试stopComputation()在计算早期中断
 * 
 * 验证在计算刚开始时调用stopComputation()能够快速中断。
 * Requirements: 3.2
 */
TEST(CFDSolverThreadTest, StopComputationEarlyInterruption) {
    CFDSolverThread thread;
    
    QSignalSpy progressSpy(&thread, &CFDSolverThread::progressUpdated);
    QSignalSpy completedSpy(&thread, &CFDSolverThread::computationCompleted);
    
    const int totalIterations = 100;
    thread.startComputation(totalIterations, 0);
    
    // 立即停止（在第一次迭代之前或期间）
    thread.stopComputation();
    
    // 等待线程结束，处理事件
    thread.wait(2000);
    QTest::qWait(100);
    
    // 验证没有完成
    EXPECT_EQ(completedSpy.count(), 0);
    
    // 验证进度信号很少（可能是0或几个）
    EXPECT_LT(progressSpy.count(), 10);
}

/**
 * @brief 测试线程资源正确清理
 * 
 * 验证线程结束后资源被正确清理，线程不再运行。
 * Requirements: 3.2
 */
TEST(CFDSolverThreadTest, ThreadResourcesCleanedUp) {
    CFDSolverThread thread;
    
    QSignalSpy completedSpy(&thread, &CFDSolverThread::computationCompleted);
    
    const int totalIterations = 5;
    thread.startComputation(totalIterations, 0);
    
    // 等待完成信号，处理事件
    EXPECT_TRUE(completedSpy.wait(2000));
    // 给一点时间让所有信号到达
    QTest::qWait(100);
    
    // 验证线程已经停止
    EXPECT_FALSE(thread.isRunning());
    
    // 再次启动应该能够成功
    QSignalSpy progressSpy(&thread, &CFDSolverThread::progressUpdated);
    QSignalSpy completedSpy2(&thread, &CFDSolverThread::computationCompleted);
    thread.startComputation(totalIterations, 0);
    
    EXPECT_TRUE(completedSpy2.wait(2000));
    QTest::qWait(100);
    EXPECT_EQ(progressSpy.count(), totalIterations);
}

/**
 * @brief 测试析构函数正确清理运行中的线程
 * 
 * 验证当线程对象被销毁时，正在运行的线程能够正确停止。
 * Requirements: 3.2
 */
TEST(CFDSolverThreadTest, DestructorCleansUpRunningThread) {
    // 创建一个作用域，让线程对象在作用域结束时被销毁
    {
        CFDSolverThread thread;
        
        // 启动一个较长的计算
        const int totalIterations = 100;
        thread.startComputation(totalIterations, 0);
        
        // 等待一小段时间确保计算开始
        QThread::msleep(100);
        
        // 线程对象将在这里被销毁，析构函数应该正确停止线程
    }
    
    // 如果析构函数正确工作，我们应该能够到达这里而不会崩溃或挂起
    SUCCEED();
}

/**
 * @brief 测试从中间位置继续计算
 * 
 * 验证线程能够从指定的起始位置继续计算。
 * Requirements: 1.4, 3.1
 */
TEST(CFDSolverThreadTest, ContinuesFromMiddle) {
    CFDSolverThread thread;
    
    QSignalSpy progressSpy(&thread, &CFDSolverThread::progressUpdated);
    QSignalSpy completedSpy(&thread, &CFDSolverThread::computationCompleted);
    
    const int totalIterations = 10;
    const int startFrom = 5;
    
    thread.startComputation(totalIterations, startFrom);
    
    // 等待完成信号，处理事件
    EXPECT_TRUE(completedSpy.wait(2000));
    // 给一点时间让所有信号到达
    QTest::qWait(100);
    
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
}

/**
 * @brief 测试多次启动和停止
 * 
 * 验证线程能够多次启动和停止而不出现问题。
 * Requirements: 3.1, 3.2
 */
TEST(CFDSolverThreadTest, MultipleStartStopCycles) {
    CFDSolverThread thread;
    
    for (int cycle = 0; cycle < 3; ++cycle) {
        QSignalSpy progressSpy(&thread, &CFDSolverThread::progressUpdated);
        QSignalSpy completedSpy(&thread, &CFDSolverThread::computationCompleted);
        
        const int totalIterations = 5;
        thread.startComputation(totalIterations, 0);
        
        // 等待完成信号，处理事件
        EXPECT_TRUE(completedSpy.wait(2000));
        // 给一点时间让所有信号到达
        QTest::qWait(100);
        
        // 验证每次都正确完成
        EXPECT_EQ(completedSpy.count(), 1);
        EXPECT_EQ(progressSpy.count(), totalIterations);
    }
}

/**
 * @brief 测试边缘情况：1次迭代
 * 
 * 验证线程能够正确处理最小迭代次数。
 * Requirements: 1.4, 3.1
 */
TEST(CFDSolverThreadTest, HandlesOneIteration) {
    CFDSolverThread thread;
    
    QSignalSpy progressSpy(&thread, &CFDSolverThread::progressUpdated);
    QSignalSpy completedSpy(&thread, &CFDSolverThread::computationCompleted);
    
    thread.startComputation(1, 0);
    
    // 等待完成信号，处理事件
    EXPECT_TRUE(completedSpy.wait(2000));
    // 给一点时间让所有信号到达
    QTest::qWait(100);
    
    // 验证完成信号被发射
    EXPECT_EQ(completedSpy.count(), 1);
    
    // 验证进度信号被发射1次
    EXPECT_EQ(progressSpy.count(), 1);
}

/**
 * @brief 测试信号参数完整性
 * 
 * 验证所有信号的参数都正确传递，没有数据丢失。
 * Requirements: 1.3, 3.1
 */
TEST(CFDSolverThreadTest, SignalParametersIntegrity) {
    CFDSolverThread thread;
    
    QSignalSpy progressSpy(&thread, &CFDSolverThread::progressUpdated);
    QSignalSpy completedSpy(&thread, &CFDSolverThread::computationCompleted);
    
    const int totalIterations = 20;
    thread.startComputation(totalIterations, 0);
    
    // 等待完成信号，处理事件
    EXPECT_TRUE(completedSpy.wait(5000));
    // 给一点时间让所有信号到达
    QTest::qWait(100);
    
    // 验证每个信号的参数都正确
    EXPECT_EQ(progressSpy.count(), totalIterations);
    
    for (int i = 0; i < progressSpy.count(); ++i) {
        QList<QVariant> signal = progressSpy.at(i);
        
        // 验证信号有两个参数
        EXPECT_EQ(signal.size(), 2);
        
        // 验证参数类型和值
        EXPECT_TRUE(signal.at(0).canConvert<int>());
        EXPECT_TRUE(signal.at(1).canConvert<int>());
        
        int current = signal.at(0).toInt();
        int total = signal.at(1).toInt();
        
        EXPECT_EQ(current, i + 1);
        EXPECT_EQ(total, totalIterations);
    }
}

/**
 * @brief 测试线程安全的停止标志
 * 
 * 验证停止标志是线程安全的，可以从不同线程访问。
 * Requirements: 3.2
 */
TEST(CFDSolverThreadTest, StopFlagIsThreadSafe) {
    CFDSolverThread thread;
    
    QSignalSpy completedSpy(&thread, &CFDSolverThread::computationCompleted);
    
    const int totalIterations = 100;
    thread.startComputation(totalIterations, 0);
    
    // 从主线程多次调用stopComputation（测试线程安全）
    thread.stopComputation();
    thread.stopComputation();
    thread.stopComputation();
    
    // 等待线程结束，处理事件
    thread.wait(2000);
    QTest::qWait(100);
    
    // 验证没有崩溃，线程正确停止
    EXPECT_FALSE(thread.isRunning());
}
