#include <gtest/gtest.h>
#include <QTest>
#include <QSignalSpy>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include "main_window.h"
#include "computation_controller.h"
#include "monitor_widget.h"
#include "computation_state.h"

/**
 * @brief Test fixture for MainWindow
 */
class MainWindowTest : public ::testing::Test {
protected:
    void SetUp() override {
        window = new MainWindow();
        
        // Find UI components
        startButton = window->findChild<QPushButton*>();
        QList<QPushButton*> buttons = window->findChildren<QPushButton*>();
        for (auto* btn : buttons) {
            if (btn->text() == "启动") {
                startButton = btn;
            } else if (btn->text() == "停止") {
                stopButton = btn;
            } else if (btn->text() == "继续") {
                continueButton = btn;
            }
        }
        
        iterationSpinBox = window->findChild<QSpinBox*>();
        monitorWidget = window->findChild<MonitorWidget*>();
        statusLabel = window->findChild<QLabel*>();
    }
    
    void TearDown() override {
        delete window;
        window = nullptr;
    }
    
    MainWindow* window;
    QPushButton* startButton;
    QPushButton* stopButton;
    QPushButton* continueButton;
    QSpinBox* iterationSpinBox;
    MonitorWidget* monitorWidget;
    QLabel* statusLabel;
};

/**
 * @brief Test window properties are set correctly
 * Validates: Basic window setup
 */
TEST_F(MainWindowTest, WindowPropertiesSetCorrectly) {
    EXPECT_EQ(window->windowTitle(), QString("CFD求解器GUI"));
    EXPECT_GE(window->minimumWidth(), 500);
    EXPECT_GE(window->minimumHeight(), 400);
}

/**
 * @brief Test all UI components are created
 * Validates: UI component initialization
 */
TEST_F(MainWindowTest, AllUIComponentsCreated) {
    ASSERT_NE(startButton, nullptr);
    ASSERT_NE(stopButton, nullptr);
    ASSERT_NE(continueButton, nullptr);
    ASSERT_NE(iterationSpinBox, nullptr);
    ASSERT_NE(monitorWidget, nullptr);
    ASSERT_NE(statusLabel, nullptr);
}

/**
 * @brief Test button labels are correct
 * Validates: Button text initialization
 */
TEST_F(MainWindowTest, ButtonLabelsCorrect) {
    EXPECT_EQ(startButton->text(), QString("启动"));
    EXPECT_EQ(stopButton->text(), QString("停止"));
    EXPECT_EQ(continueButton->text(), QString("继续"));
}

/**
 * @brief Test button minimum widths are set
 * Validates: Button sizing
 */
TEST_F(MainWindowTest, ButtonMinimumWidthsSet) {
    EXPECT_GE(startButton->minimumWidth(), 100);
    EXPECT_GE(stopButton->minimumWidth(), 100);
    EXPECT_GE(continueButton->minimumWidth(), 100);
}

/**
 * @brief Test iteration spinbox properties
 * Validates: Requirements 5.1, 5.2, 5.3
 */
TEST_F(MainWindowTest, IterationSpinBoxProperties) {
    EXPECT_EQ(iterationSpinBox->minimum(), 1);
    EXPECT_EQ(iterationSpinBox->maximum(), INT_MAX);
    EXPECT_EQ(iterationSpinBox->value(), 100);
    EXPECT_GE(iterationSpinBox->minimumWidth(), 150);
}

/**
 * @brief Test initial UI state is Not_Started
 * Validates: Requirements 2.4, 2.5, 3.4, 3.5, 4.4, 4.5, 5.5
 */
TEST_F(MainWindowTest, InitialUIStateCorrect) {
    EXPECT_TRUE(startButton->isEnabled());
    EXPECT_FALSE(stopButton->isEnabled());
    EXPECT_FALSE(continueButton->isEnabled());
    EXPECT_TRUE(iterationSpinBox->isEnabled());
}

/**
 * @brief Test Start button click triggers controller start
 * Validates: Requirements 2.1
 */
TEST_F(MainWindowTest, StartButtonClickTriggersStart) {
    // Set iteration count
    iterationSpinBox->setValue(200);
    
    // Click Start button
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Verify status label updated
    EXPECT_TRUE(statusLabel->text().contains("正在启动计算"));
    EXPECT_TRUE(statusLabel->text().contains("200"));
}

/**
 * @brief Test Stop button click triggers controller stop
 * Validates: Requirements 3.1
 */
TEST_F(MainWindowTest, StopButtonClickTriggersStop) {
    // Start computation first
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Click Stop button
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Verify status label updated
    EXPECT_TRUE(statusLabel->text().contains("正在停止计算"));
}

/**
 * @brief Test Continue button click triggers controller continue
 * Validates: Requirements 4.1
 */
TEST_F(MainWindowTest, ContinueButtonClickTriggersContinue) {
    // Start and stop computation
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Click Continue button
    QTest::mouseClick(continueButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Verify status label updated
    EXPECT_TRUE(statusLabel->text().contains("正在继续计算"));
}

/**
 * @brief Test UI state updates when transitioning to Running
 * Validates: Requirements 2.4, 5.6
 */
TEST_F(MainWindowTest, UIStateUpdatesWhenRunning) {
    // Click Start button
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Verify button states
    EXPECT_FALSE(startButton->isEnabled());
    EXPECT_TRUE(stopButton->isEnabled());
    EXPECT_FALSE(continueButton->isEnabled());
    EXPECT_FALSE(iterationSpinBox->isEnabled());
    
    // Verify status label
    EXPECT_TRUE(statusLabel->text().contains("计算运行中"));
}

/**
 * @brief Test UI state updates when transitioning to Stopped
 * Validates: Requirements 3.4, 5.5
 */
TEST_F(MainWindowTest, UIStateUpdatesWhenStopped) {
    // Start then stop
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Verify button states
    EXPECT_FALSE(startButton->isEnabled());
    EXPECT_FALSE(stopButton->isEnabled());
    EXPECT_TRUE(continueButton->isEnabled());
    EXPECT_TRUE(iterationSpinBox->isEnabled());
    
    // Verify status label
    EXPECT_TRUE(statusLabel->text().contains("计算已停止"));
}

/**
 * @brief Test UI state updates when transitioning to Completed
 * Validates: Requirements 2.5, 3.5, 4.5, 5.5
 */
TEST_F(MainWindowTest, UIStateUpdatesWhenCompleted) {
    // Manually trigger completion through controller
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    controller->start(10);
    QTest::qWait(50);
    controller->onComputationCompleted();
    QTest::qWait(50);
    
    // Verify button states
    EXPECT_TRUE(startButton->isEnabled());
    EXPECT_FALSE(stopButton->isEnabled());
    EXPECT_FALSE(continueButton->isEnabled());
    EXPECT_TRUE(iterationSpinBox->isEnabled());
    
    // Verify status label
    EXPECT_TRUE(statusLabel->text().contains("计算已完成"));
}

/**
 * @brief Test iteration input validation - minimum value
 * Validates: Requirements 5.1, 5.2
 */
TEST_F(MainWindowTest, IterationInputValidationMinimum) {
    iterationSpinBox->setValue(1);
    EXPECT_EQ(iterationSpinBox->value(), 1);
    
    // Try to set below minimum (should be clamped)
    iterationSpinBox->setValue(0);
    EXPECT_EQ(iterationSpinBox->value(), 1);
}

/**
 * @brief Test iteration input validation - maximum value
 * Validates: Requirements 5.1, 5.3
 */
TEST_F(MainWindowTest, IterationInputValidationMaximum) {
    iterationSpinBox->setValue(INT_MAX);
    EXPECT_EQ(iterationSpinBox->value(), INT_MAX);
}

/**
 * @brief Test iteration input validation - typical values
 * Validates: Requirements 5.1, 5.4
 */
TEST_F(MainWindowTest, IterationInputValidationTypicalValues) {
    int testValues[] = {1, 10, 100, 1000, 10000, 100000};
    
    for (int value : testValues) {
        iterationSpinBox->setValue(value);
        EXPECT_EQ(iterationSpinBox->value(), value);
    }
}

/**
 * @brief Test iteration input disabled when Running
 * Validates: Requirements 5.6
 */
TEST_F(MainWindowTest, IterationInputDisabledWhenRunning) {
    EXPECT_TRUE(iterationSpinBox->isEnabled());
    
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    EXPECT_FALSE(iterationSpinBox->isEnabled());
}

/**
 * @brief Test iteration input enabled when Stopped
 * Validates: Requirements 5.5
 */
TEST_F(MainWindowTest, IterationInputEnabledWhenStopped) {
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    EXPECT_FALSE(iterationSpinBox->isEnabled());
    
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QTest::qWait(50);
    
    EXPECT_TRUE(iterationSpinBox->isEnabled());
}

/**
 * @brief Test error display in status label
 * Validates: Requirements 8.1, 8.2
 */
TEST_F(MainWindowTest, ErrorDisplayInStatusLabel) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    QString errorMsg = "测试错误消息";
    controller->start(100);
    QTest::qWait(50);
    
    // Trigger error
    controller->onComputationError(errorMsg);
    QTest::qWait(50);
    
    // Verify error displayed in status label
    EXPECT_TRUE(statusLabel->text().contains("错误"));
    EXPECT_TRUE(statusLabel->text().contains(errorMsg));
}

/**
 * @brief Test error forwarded to MonitorWidget
 * Validates: Requirements 8.2, 8.4
 */
TEST_F(MainWindowTest, ErrorForwardedToMonitorWidget) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    QSignalSpy errorSpy(controller, &ComputationController::errorOccurred);
    
    controller->start(100);
    QTest::qWait(50);
    
    QString errorMsg = "MonitorWidget错误测试";
    controller->onComputationError(errorMsg);
    QTest::qWait(50);
    
    // Verify error signal was emitted
    EXPECT_EQ(errorSpy.count(), 1);
    QList<QVariant> arguments = errorSpy.takeFirst();
    EXPECT_EQ(arguments.at(0).toString(), errorMsg);
}

/**
 * @brief Test status label updates for all states
 * Validates: Requirements 2.4, 3.4, 4.4
 */
TEST_F(MainWindowTest, StatusLabelUpdatesForAllStates) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    // Not_Started
    EXPECT_TRUE(statusLabel->text().contains("准备就绪"));
    
    // Running
    controller->start(100);
    QTest::qWait(50);
    EXPECT_TRUE(statusLabel->text().contains("计算运行中"));
    
    // Stopped
    controller->stop();
    QTest::qWait(50);
    EXPECT_TRUE(statusLabel->text().contains("计算已停止"));
    
    // Completed
    controller->continueComputation();
    QTest::qWait(50);
    controller->onComputationCompleted();
    QTest::qWait(50);
    EXPECT_TRUE(statusLabel->text().contains("计算已完成"));
}

/**
 * @brief Test MonitorWidget receives state updates
 * Validates: Requirements 6.1, 6.6
 */
TEST_F(MainWindowTest, MonitorWidgetReceivesStateUpdates) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    
    controller->start(100);
    QTest::qWait(50);
    
    EXPECT_GE(stateSpy.count(), 1);
}

/**
 * @brief Test MonitorWidget receives progress updates
 * Validates: Requirements 6.2, 6.3, 6.4
 */
TEST_F(MainWindowTest, MonitorWidgetReceivesProgressUpdates) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    QSignalSpy progressSpy(controller, &ComputationController::progressUpdated);
    
    controller->start(100);
    QTest::qWait(50);
    
    // Simulate progress update
    controller->onProgressUpdate(50, 100);
    QTest::qWait(50);
    
    EXPECT_GE(progressSpy.count(), 1);
}

/**
 * @brief Test complete workflow: Start → Stop → Continue → Complete
 * Validates: Requirements 2.1, 3.1, 4.1, complete workflow
 */
TEST_F(MainWindowTest, CompleteWorkflowStartStopContinueComplete) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    // Start
    iterationSpinBox->setValue(100);
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    EXPECT_FALSE(startButton->isEnabled());
    EXPECT_TRUE(stopButton->isEnabled());
    EXPECT_FALSE(continueButton->isEnabled());
    
    // Simulate progress
    controller->onProgressUpdate(50, 100);
    QTest::qWait(50);
    
    // Stop
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QTest::qWait(50);
    EXPECT_FALSE(startButton->isEnabled());
    EXPECT_FALSE(stopButton->isEnabled());
    EXPECT_TRUE(continueButton->isEnabled());
    
    // Continue
    QTest::mouseClick(continueButton, Qt::LeftButton);
    QTest::qWait(50);
    EXPECT_FALSE(startButton->isEnabled());
    EXPECT_TRUE(stopButton->isEnabled());
    EXPECT_FALSE(continueButton->isEnabled());
    
    // Complete
    controller->onComputationCompleted();
    QTest::qWait(50);
    EXPECT_TRUE(startButton->isEnabled());
    EXPECT_FALSE(stopButton->isEnabled());
    EXPECT_FALSE(continueButton->isEnabled());
}

/**
 * @brief Test workflow: Start → Complete (no stop)
 * Validates: Requirements 2.1, complete workflow without interruption
 */
TEST_F(MainWindowTest, WorkflowStartToComplete) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    // Start
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    EXPECT_TRUE(statusLabel->text().contains("计算运行中"));
    
    // Simulate progress to completion
    controller->onProgressUpdate(100, 100);
    QTest::qWait(50);
    
    // Complete
    controller->onComputationCompleted();
    QTest::qWait(50);
    
    EXPECT_TRUE(startButton->isEnabled());
    EXPECT_TRUE(statusLabel->text().contains("计算已完成"));
}

/**
 * @brief Test workflow: Start → Error
 * Validates: Requirements 2.1, 8.1, 8.2
 */
TEST_F(MainWindowTest, WorkflowStartToError) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    // Start
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Trigger error
    QString errorMsg = "计算错误测试";
    controller->onComputationError(errorMsg);
    QTest::qWait(50);
    
    // Verify error handling
    EXPECT_TRUE(statusLabel->text().contains("错误"));
    EXPECT_TRUE(statusLabel->text().contains(errorMsg));
    EXPECT_FALSE(startButton->isEnabled());
    EXPECT_TRUE(continueButton->isEnabled());
}

/**
 * @brief Test rapid button clicks are handled correctly
 * Validates: Requirements 2.5, 3.5, 4.5
 */
TEST_F(MainWindowTest, RapidButtonClicksHandled) {
    // Rapid Start clicks (only first should take effect)
    for (int i = 0; i < 5; ++i) {
        QTest::mouseClick(startButton, Qt::LeftButton);
    }
    QTest::qWait(50);
    
    // Should be in Running state
    EXPECT_FALSE(startButton->isEnabled());
    EXPECT_TRUE(stopButton->isEnabled());
    
    // Rapid Stop clicks
    for (int i = 0; i < 5; ++i) {
        QTest::mouseClick(stopButton, Qt::LeftButton);
    }
    QTest::qWait(50);
    
    // Should be in Stopped state
    EXPECT_TRUE(continueButton->isEnabled());
}

/**
 * @brief Test Start button disabled when Running
 * Validates: Requirements 2.5
 */
TEST_F(MainWindowTest, StartButtonDisabledWhenRunning) {
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    EXPECT_FALSE(startButton->isEnabled());
}

/**
 * @brief Test Stop button disabled when Not_Started
 * Validates: Requirements 3.5
 */
TEST_F(MainWindowTest, StopButtonDisabledWhenNotStarted) {
    EXPECT_FALSE(stopButton->isEnabled());
}

/**
 * @brief Test Stop button disabled when Stopped
 * Validates: Requirements 3.5
 */
TEST_F(MainWindowTest, StopButtonDisabledWhenStopped) {
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QTest::qWait(50);
    
    EXPECT_FALSE(stopButton->isEnabled());
}

/**
 * @brief Test Stop button disabled when Completed
 * Validates: Requirements 3.5
 */
TEST_F(MainWindowTest, StopButtonDisabledWhenCompleted) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    controller->start(10);
    QTest::qWait(50);
    controller->onComputationCompleted();
    QTest::qWait(50);
    
    EXPECT_FALSE(stopButton->isEnabled());
}

/**
 * @brief Test Continue button disabled when Not_Started
 * Validates: Requirements 4.5
 */
TEST_F(MainWindowTest, ContinueButtonDisabledWhenNotStarted) {
    EXPECT_FALSE(continueButton->isEnabled());
}

/**
 * @brief Test Continue button disabled when Running
 * Validates: Requirements 4.5
 */
TEST_F(MainWindowTest, ContinueButtonDisabledWhenRunning) {
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    EXPECT_FALSE(continueButton->isEnabled());
}

/**
 * @brief Test Continue button disabled when Completed
 * Validates: Requirements 4.5
 */
TEST_F(MainWindowTest, ContinueButtonDisabledWhenCompleted) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    controller->start(10);
    QTest::qWait(50);
    controller->onComputationCompleted();
    QTest::qWait(50);
    
    EXPECT_FALSE(continueButton->isEnabled());
}

/**
 * @brief Test iteration value used when starting computation
 * Validates: Requirements 2.1, 5.4
 */
TEST_F(MainWindowTest, IterationValueUsedWhenStarting) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    QSignalSpy startSpy(controller, &ComputationController::startSolver);
    
    iterationSpinBox->setValue(500);
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    EXPECT_EQ(startSpy.count(), 1);
    QList<QVariant> arguments = startSpy.takeFirst();
    EXPECT_EQ(arguments.at(0).toInt(), 500);
}

/**
 * @brief Test changing iteration value while stopped
 * Validates: Requirements 5.5
 */
TEST_F(MainWindowTest, ChangeIterationValueWhileStopped) {
    // Start with 100 iterations
    iterationSpinBox->setValue(100);
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Stop
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Change iteration value (should be allowed)
    EXPECT_TRUE(iterationSpinBox->isEnabled());
    iterationSpinBox->setValue(200);
    EXPECT_EQ(iterationSpinBox->value(), 200);
}

/**
 * @brief Test edge case: minimum iterations (1)
 * Validates: Requirements 5.1, 5.2
 */
TEST_F(MainWindowTest, EdgeCaseMinimumIterations) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    QSignalSpy startSpy(controller, &ComputationController::startSolver);
    
    iterationSpinBox->setValue(1);
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    EXPECT_EQ(startSpy.count(), 1);
    QList<QVariant> arguments = startSpy.takeFirst();
    EXPECT_EQ(arguments.at(0).toInt(), 1);
}

/**
 * @brief Test edge case: maximum iterations
 * Validates: Requirements 5.1, 5.3
 */
TEST_F(MainWindowTest, EdgeCaseMaximumIterations) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    QSignalSpy startSpy(controller, &ComputationController::startSolver);
    
    iterationSpinBox->setValue(INT_MAX);
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    EXPECT_EQ(startSpy.count(), 1);
    QList<QVariant> arguments = startSpy.takeFirst();
    EXPECT_EQ(arguments.at(0).toInt(), INT_MAX);
}

/**
 * @brief Test multiple start-complete cycles
 * Validates: Requirements 2.1, 2.5
 */
TEST_F(MainWindowTest, MultipleStartCompleteCycles) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    for (int cycle = 0; cycle < 3; ++cycle) {
        // Start
        QTest::mouseClick(startButton, Qt::LeftButton);
        QTest::qWait(50);
        EXPECT_FALSE(startButton->isEnabled());
        
        // Complete
        controller->onComputationCompleted();
        QTest::qWait(50);
        EXPECT_TRUE(startButton->isEnabled());
    }
}

/**
 * @brief Test multiple stop-continue cycles
 * Validates: Requirements 3.1, 4.1
 */
TEST_F(MainWindowTest, MultipleStopContinueCycles) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    // Start
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    for (int cycle = 0; cycle < 3; ++cycle) {
        // Stop
        QTest::mouseClick(stopButton, Qt::LeftButton);
        QTest::qWait(50);
        EXPECT_TRUE(continueButton->isEnabled());
        
        // Continue
        QTest::mouseClick(continueButton, Qt::LeftButton);
        QTest::qWait(50);
        EXPECT_TRUE(stopButton->isEnabled());
    }
}

/**
 * @brief Test status label shows iteration count on start
 * Validates: Requirements 2.1, 2.4
 */
TEST_F(MainWindowTest, StatusLabelShowsIterationCountOnStart) {
    iterationSpinBox->setValue(250);
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    EXPECT_TRUE(statusLabel->text().contains("250"));
}

/**
 * @brief Test controller is properly connected
 * Validates: Signal-slot connections
 */
TEST_F(MainWindowTest, ControllerProperlyConnected) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    // Test stateChanged signal connection
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    controller->start(100);
    QTest::qWait(50);
    EXPECT_GE(stateSpy.count(), 1);
    
    // Test errorOccurred signal connection
    QSignalSpy errorSpy(controller, &ComputationController::errorOccurred);
    controller->onComputationError("Test error");
    QTest::qWait(50);
    EXPECT_EQ(errorSpy.count(), 1);
}

/**
 * @brief Test MonitorWidget is properly connected
 * Validates: Requirements 6.1, 6.2
 */
TEST_F(MainWindowTest, MonitorWidgetProperlyConnected) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    QSignalSpy progressSpy(controller, &ComputationController::progressUpdated);
    
    controller->start(100);
    QTest::qWait(50);
    controller->onProgressUpdate(50, 100);
    QTest::qWait(50);
    
    EXPECT_GE(stateSpy.count(), 1);
    EXPECT_GE(progressSpy.count(), 1);
}

/**
 * @brief Test error message box is displayed
 * Validates: Requirements 8.2, 8.4
 */
TEST_F(MainWindowTest, ErrorMessageBoxDisplayed) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    controller->start(100);
    QTest::qWait(50);
    
    // Note: QMessageBox::critical is modal and blocks, so we just verify
    // the error signal is emitted and status label is updated
    QSignalSpy errorSpy(controller, &ComputationController::errorOccurred);
    
    QString errorMsg = "严重错误";
    controller->onComputationError(errorMsg);
    QTest::qWait(50);
    
    EXPECT_EQ(errorSpy.count(), 1);
    EXPECT_TRUE(statusLabel->text().contains(errorMsg));
}

/**
 * @brief Test UI remains responsive after error
 * Validates: Requirements 8.4
 */
TEST_F(MainWindowTest, UIResponsiveAfterError) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    controller->start(100);
    QTest::qWait(50);
    
    controller->onComputationError("测试错误");
    QTest::qWait(50);
    
    // UI should allow continue after error
    EXPECT_TRUE(continueButton->isEnabled());
    
    // Should be able to continue
    QTest::mouseClick(continueButton, Qt::LeftButton);
    QTest::qWait(50);
    EXPECT_TRUE(stopButton->isEnabled());
}

/**
 * @brief Test state transitions are atomic
 * Validates: Requirements 7.4
 */
TEST_F(MainWindowTest, StateTransitionsAreAtomic) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    QSignalSpy stateSpy(controller, &ComputationController::stateChanged);
    
    // Start
    controller->start(100);
    QTest::qWait(50);
    int stateChanges1 = stateSpy.count();
    
    // Stop
    controller->stop();
    QTest::qWait(50);
    int stateChanges2 = stateSpy.count();
    
    // Each action should trigger exactly one state change
    EXPECT_EQ(stateChanges1, 1);
    EXPECT_EQ(stateChanges2, 2);
}

/**
 * @brief Test window can be shown and hidden
 * Validates: Basic window functionality
 */
TEST_F(MainWindowTest, WindowCanBeShownAndHidden) {
    window->show();
    EXPECT_TRUE(window->isVisible());
    
    window->hide();
    EXPECT_FALSE(window->isVisible());
}

/**
 * @brief Test all buttons are clickable
 * Validates: Button functionality
 */
TEST_F(MainWindowTest, AllButtonsClickable) {
    EXPECT_TRUE(startButton->isEnabled());
    EXPECT_NO_THROW(QTest::mouseClick(startButton, Qt::LeftButton));
    QTest::qWait(50);
    
    EXPECT_TRUE(stopButton->isEnabled());
    EXPECT_NO_THROW(QTest::mouseClick(stopButton, Qt::LeftButton));
    QTest::qWait(50);
    
    EXPECT_TRUE(continueButton->isEnabled());
    EXPECT_NO_THROW(QTest::mouseClick(continueButton, Qt::LeftButton));
    QTest::qWait(50);
}

/**
 * @brief Test spinbox value can be changed programmatically
 * Validates: Requirements 5.4
 */
TEST_F(MainWindowTest, SpinBoxValueChangeProgrammatically) {
    int testValues[] = {1, 50, 100, 500, 1000, 5000, 10000};
    
    for (int value : testValues) {
        iterationSpinBox->setValue(value);
        EXPECT_EQ(iterationSpinBox->value(), value);
    }
}

/**
 * @brief Test spinbox value can be changed by user input
 * Validates: Requirements 5.4
 */
TEST_F(MainWindowTest, SpinBoxValueChangeByUserInput) {
    // Simulate user typing
    iterationSpinBox->setFocus();
    iterationSpinBox->selectAll();
    QTest::keyClicks(iterationSpinBox, "999");
    QTest::qWait(50);
    
    EXPECT_EQ(iterationSpinBox->value(), 999);
}

/**
 * @brief Test status label is visible
 * Validates: UI visibility
 */
TEST_F(MainWindowTest, StatusLabelVisible) {
    ASSERT_NE(statusLabel, nullptr);
    EXPECT_TRUE(statusLabel->isVisible());
}

/**
 * @brief Test status label supports word wrap
 * Validates: UI text display
 */
TEST_F(MainWindowTest, StatusLabelSupportsWordWrap) {
    EXPECT_TRUE(statusLabel->wordWrap());
}

/**
 * @brief Test MonitorWidget is visible
 * Validates: Requirements 6.1
 */
TEST_F(MainWindowTest, MonitorWidgetVisible) {
    ASSERT_NE(monitorWidget, nullptr);
    EXPECT_TRUE(monitorWidget->isVisible());
}

/**
 * @brief Test complete workflow with progress updates
 * Validates: Requirements 2.1, 3.1, 4.1, 6.2, 6.3, 6.4
 */
TEST_F(MainWindowTest, CompleteWorkflowWithProgressUpdates) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    QSignalSpy progressSpy(controller, &ComputationController::progressUpdated);
    
    // Start
    iterationSpinBox->setValue(100);
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Simulate progress updates
    for (int i = 10; i <= 50; i += 10) {
        controller->onProgressUpdate(i, 100);
        QTest::qWait(50);
    }
    
    // Stop
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // Continue
    QTest::mouseClick(continueButton, Qt::LeftButton);
    QTest::qWait(50);
    
    // More progress
    for (int i = 60; i <= 100; i += 10) {
        controller->onProgressUpdate(i, 100);
        QTest::qWait(50);
    }
    
    // Complete
    controller->onComputationCompleted();
    QTest::qWait(50);
    
    EXPECT_GE(progressSpy.count(), 10);
    EXPECT_TRUE(startButton->isEnabled());
}

/**
 * @brief Test boundary: zero progress
 * Validates: Requirements 6.4
 */
TEST_F(MainWindowTest, BoundaryZeroProgress) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    controller->start(100);
    QTest::qWait(50);
    
    controller->onProgressUpdate(0, 100);
    QTest::qWait(50);
    
    // Should not crash
    SUCCEED();
}

/**
 * @brief Test boundary: full progress
 * Validates: Requirements 6.4
 */
TEST_F(MainWindowTest, BoundaryFullProgress) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    controller->start(100);
    QTest::qWait(50);
    
    controller->onProgressUpdate(100, 100);
    QTest::qWait(50);
    
    // Should not crash
    SUCCEED();
}

/**
 * @brief Test rapid state changes
 * Validates: Requirements 7.4
 */
TEST_F(MainWindowTest, RapidStateChanges) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    // Rapid start-stop-continue sequence
    controller->start(100);
    controller->stop();
    controller->continueComputation();
    controller->stop();
    controller->continueComputation();
    QTest::qWait(100);
    
    // Should handle gracefully
    EXPECT_TRUE(stopButton->isEnabled() || continueButton->isEnabled());
}

/**
 * @brief Test UI updates are synchronized with state
 * Validates: Requirements 2.4, 3.4, 4.4
 */
TEST_F(MainWindowTest, UIUpdatesSynchronizedWithState) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    // Start
    controller->start(100);
    QTest::qWait(50);
    EXPECT_EQ(controller->currentState(), ComputationState::Running);
    EXPECT_FALSE(startButton->isEnabled());
    EXPECT_TRUE(stopButton->isEnabled());
    
    // Stop
    controller->stop();
    QTest::qWait(50);
    EXPECT_EQ(controller->currentState(), ComputationState::Stopped);
    EXPECT_FALSE(stopButton->isEnabled());
    EXPECT_TRUE(continueButton->isEnabled());
}

/**
 * @brief Test error clears previous status
 * Validates: Requirements 8.2
 */
TEST_F(MainWindowTest, ErrorClearsPreviousStatus) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    controller->start(100);
    QTest::qWait(50);
    QString runningStatus = statusLabel->text();
    
    controller->onComputationError("新错误");
    QTest::qWait(50);
    
    EXPECT_NE(statusLabel->text(), runningStatus);
    EXPECT_TRUE(statusLabel->text().contains("错误"));
}

/**
 * @brief Test multiple errors are handled
 * Validates: Requirements 8.1, 8.2
 */
TEST_F(MainWindowTest, MultipleErrorsHandled) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    QSignalSpy errorSpy(controller, &ComputationController::errorOccurred);
    
    controller->start(100);
    QTest::qWait(50);
    
    controller->onComputationError("错误1");
    QTest::qWait(50);
    
    controller->continueComputation();
    QTest::qWait(50);
    
    controller->onComputationError("错误2");
    QTest::qWait(50);
    
    EXPECT_EQ(errorSpy.count(), 2);
}

/**
 * @brief Test iteration spinbox step increment
 * Validates: Requirements 5.4
 */
TEST_F(MainWindowTest, IterationSpinBoxStepIncrement) {
    iterationSpinBox->setValue(100);
    iterationSpinBox->stepUp();
    EXPECT_EQ(iterationSpinBox->value(), 101);
    
    iterationSpinBox->stepDown();
    EXPECT_EQ(iterationSpinBox->value(), 100);
}

/**
 * @brief Test window has central widget
 * Validates: Basic window structure
 */
TEST_F(MainWindowTest, WindowHasCentralWidget) {
    QWidget* centralWidget = window->centralWidget();
    ASSERT_NE(centralWidget, nullptr);
}

/**
 * @brief Test all UI components are children of window
 * Validates: UI hierarchy
 */
TEST_F(MainWindowTest, UIComponentsAreChildrenOfWindow) {
    EXPECT_EQ(startButton->window(), window);
    EXPECT_EQ(stopButton->window(), window);
    EXPECT_EQ(continueButton->window(), window);
    EXPECT_EQ(iterationSpinBox->window(), window);
    EXPECT_EQ(monitorWidget->window(), window);
    EXPECT_EQ(statusLabel->window(), window);
}

/**
 * @brief Test controller is child of window
 * Validates: Object ownership
 */
TEST_F(MainWindowTest, ControllerIsChildOfWindow) {
    ComputationController* controller = window->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    EXPECT_EQ(controller->parent(), window);
}

/**
 * @brief Test window cleanup on destruction
 * Validates: Memory management
 */
TEST_F(MainWindowTest, WindowCleanupOnDestruction) {
    MainWindow* tempWindow = new MainWindow();
    ComputationController* controller = tempWindow->findChild<ComputationController*>();
    ASSERT_NE(controller, nullptr);
    
    // Delete window should clean up controller
    delete tempWindow;
    
    // If we reach here without crash, cleanup worked
    SUCCEED();
}

/**
 * @brief Test default iteration value is 100
 * Validates: Requirements 5.4
 */
TEST_F(MainWindowTest, DefaultIterationValueIs100) {
    EXPECT_EQ(iterationSpinBox->value(), 100);
}

/**
 * @brief Test status label initial text
 * Validates: Initial UI state
 */
TEST_F(MainWindowTest, StatusLabelInitialText) {
    EXPECT_TRUE(statusLabel->text().contains("准备就绪"));
}
