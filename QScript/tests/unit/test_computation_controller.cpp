#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QTest>
#include "computation_controller.h"
#include "cfd_solver_thread.h"

/**
 * @brief Test fixture for ComputationController
 */
class ComputationControllerTest : public ::testing::Test {
protected:
    void SetUp() override {
        controller = new ComputationController();
    }
    
    void TearDown() override {
        delete controller;
        controller = nullptr;
    }
    
    ComputationController* controller;
};

/**
 * @brief Test initial state is Not_Started
 * Validates: Requirements 7.1, 7.2
 */
TEST_F(ComputationControllerTest, InitialStateIsNotStarted) {
    EXPECT_EQ(controller->currentState(), ComputationState::Not_Started);
    EXPECT_EQ(controller->currentIteration(), 0);
    EXPECT_EQ(controller->totalIterations(), 0);
}

/**
 * @brief Test start() transitions from Not_Started to Running
 * Validates: Requirements 2.1, 2.2, 2.3
 */
TEST_F(ComputationControllerTest, StartTransitionsToRunning) {
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    QSignalSpy startSpy(controller, &ComputationController::startSolver);
    
    controller->start(100);
    
    // Wait for signal emission
    QTest::qWait(50);
    
    EXPECT_EQ(controller->currentState(), ComputationState::Running);
    EXPECT_EQ(controller->totalIterations(), 100);
    EXPECT_EQ(stateSpy.count(), 1);
    EXPECT_EQ(startSpy.count(), 1);
    
    // Verify startSolver signal parameters
    QList<QVariant> arguments = startSpy.takeFirst();
    EXPECT_EQ(arguments.at(0).toInt(), 100); // totalIterations
    EXPECT_EQ(arguments.at(1).toInt(), 0);   // startFrom
}

/**
 * @brief Test start() is ignored when not in Not_Started state
 * Validates: Requirements 2.5
 */
TEST_F(ComputationControllerTest, StartIgnoredWhenNotInNotStartedState) {
    // Manually transition to Running state
    controller->start(100);
    QTest::qWait(50);
    
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    QSignalSpy startSpy(controller, &ComputationController::startSolver);
    
    // Try to start again (should be ignored)
    controller->start(200);
    QTest::qWait(50);
    
    EXPECT_EQ(controller->currentState(), ComputationState::Running);
    EXPECT_EQ(controller->totalIterations(), 100); // Should remain 100
    EXPECT_EQ(stateSpy.count(), 0); // No state change
    EXPECT_EQ(startSpy.count(), 0); // No new start signal
}

/**
 * @brief Test stop() transitions from Running to Stopped
 * Validates: Requirements 3.1, 3.2
 */
TEST_F(ComputationControllerTest, StopTransitionsToStopped) {
    // Start computation first
    controller->start(100);
    QTest::qWait(50);
    
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    QSignalSpy stopSpy(controller, &ComputationController::stopSolver);
    
    controller->stop();
    QTest::qWait(50);
    
    EXPECT_EQ(controller->currentState(), ComputationState::Stopped);
    EXPECT_EQ(stateSpy.count(), 1);
    EXPECT_EQ(stopSpy.count(), 1);
}

/**
 * @brief Test stop() is ignored when not in Running state
 * Validates: Requirements 3.5
 */
TEST_F(ComputationControllerTest, StopIgnoredWhenNotRunning) {
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    QSignalSpy stopSpy(controller, &ComputationController::stopSolver);
    
    controller->stop();
    QTest::qWait(50);
    
    EXPECT_EQ(controller->currentState(), ComputationState::Not_Started);
    EXPECT_EQ(stateSpy.count(), 0);
    EXPECT_EQ(stopSpy.count(), 0);
}

/**
 * @brief Test continueComputation() transitions from Stopped to Running
 * Validates: Requirements 4.1, 4.2
 */
TEST_F(ComputationControllerTest, ContinueTransitionsToRunning) {
    // Start and stop computation
    controller->start(100);
    QTest::qWait(50);
    
    // Simulate progress update
    controller->onProgressUpdate(50, 100);
    
    controller->stop();
    QTest::qWait(50);
    
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    QSignalSpy startSpy(controller, &ComputationController::startSolver);
    
    controller->continueComputation();
    QTest::qWait(50);
    
    EXPECT_EQ(controller->currentState(), ComputationState::Running);
    EXPECT_EQ(stateSpy.count(), 1);
    EXPECT_EQ(startSpy.count(), 1);
    
    // Verify startSolver signal parameters
    QList<QVariant> arguments = startSpy.takeFirst();
    EXPECT_EQ(arguments.at(0).toInt(), 100); // totalIterations
    EXPECT_EQ(arguments.at(1).toInt(), 50);  // startFrom (current iteration)
}

/**
 * @brief Test continueComputation() is ignored when not in Stopped state
 * Validates: Requirements 4.5
 */
TEST_F(ComputationControllerTest, ContinueIgnoredWhenNotStopped) {
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    QSignalSpy startSpy(controller, &ComputationController::startSolver);
    
    controller->continueComputation();
    QTest::qWait(50);
    
    EXPECT_EQ(controller->currentState(), ComputationState::Not_Started);
    EXPECT_EQ(stateSpy.count(), 0);
    EXPECT_EQ(startSpy.count(), 0);
}

/**
 * @brief Test onProgressUpdate() updates iteration and forwards signal
 * Validates: Requirements 1.3, 6.2
 */
TEST_F(ComputationControllerTest, ProgressUpdateForwardsSignal) {
    QSignalSpy progressSpy(controller, &ComputationController::progressUpdated);
    
    controller->onProgressUpdate(42, 100);
    
    EXPECT_EQ(controller->currentIteration(), 42);
    EXPECT_EQ(controller->totalIterations(), 100);
    EXPECT_EQ(progressSpy.count(), 1);
    
    QList<QVariant> arguments = progressSpy.takeFirst();
    EXPECT_EQ(arguments.at(0).toInt(), 42);
    EXPECT_EQ(arguments.at(1).toInt(), 100);
}

/**
 * @brief Test onComputationCompleted() transitions to Completed
 * Validates: Requirements 1.5, 6.5
 */
TEST_F(ComputationControllerTest, CompletionTransitionsToCompleted) {
    // Start computation
    controller->start(100);
    QTest::qWait(50);
    
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    
    controller->onComputationCompleted();
    QTest::qWait(50);
    
    EXPECT_EQ(controller->currentState(), ComputationState::Completed);
    EXPECT_EQ(stateSpy.count(), 1);
}

/**
 * @brief Test onComputationError() transitions to Stopped and emits error
 * Validates: Requirements 8.1, 8.2
 */
TEST_F(ComputationControllerTest, ErrorTransitionsToStoppedAndEmitsError) {
    // Start computation
    controller->start(100);
    QTest::qWait(50);
    
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    QSignalSpy errorSpy(controller, &ComputationController::errorOccurred);
    
    QString errorMsg = "Test error message";
    controller->onComputationError(errorMsg);
    QTest::qWait(50);
    
    EXPECT_EQ(controller->currentState(), ComputationState::Stopped);
    EXPECT_EQ(stateSpy.count(), 1);
    EXPECT_EQ(errorSpy.count(), 1);
    
    QList<QVariant> arguments = errorSpy.takeFirst();
    EXPECT_EQ(arguments.at(0).toString(), errorMsg);
}

/**
 * @brief Test state machine transition validation
 * Validates: Requirements 7.4
 */
TEST_F(ComputationControllerTest, InvalidTransitionsAreRejected) {
    // Test: Cannot transition from Completed to Running directly
    controller->start(10);
    QTest::qWait(50);
    controller->onComputationCompleted();
    QTest::qWait(50);
    
    EXPECT_EQ(controller->currentState(), ComputationState::Completed);
    
    // Try to continue (should be ignored)
    controller->continueComputation();
    QTest::qWait(50);
    
    EXPECT_EQ(controller->currentState(), ComputationState::Completed);
}

/**
 * @brief Test progress preservation after stop
 * Validates: Requirements 3.3
 */
TEST_F(ComputationControllerTest, ProgressPreservedAfterStop) {
    controller->start(100);
    QTest::qWait(50);
    
    // Simulate progress
    controller->onProgressUpdate(75, 100);
    
    controller->stop();
    QTest::qWait(50);
    
    EXPECT_EQ(controller->currentIteration(), 75);
    EXPECT_EQ(controller->totalIterations(), 100);
    EXPECT_EQ(controller->currentState(), ComputationState::Stopped);
}

/**
 * @brief Test thread safety of getter methods
 * Validates: Requirements 7.5
 */
TEST_F(ComputationControllerTest, GettersAreThreadSafe) {
    controller->start(100);
    
    // Call getters from multiple threads (simulated by rapid calls)
    for (int i = 0; i < 100; ++i) {
        EXPECT_NO_THROW({
            controller->currentState();
            controller->currentIteration();
            controller->totalIterations();
        });
    }
}
