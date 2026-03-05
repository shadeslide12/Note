#include <gtest/gtest.h>
#include <QApplication>

int main(int argc, char** argv) {
    // 初始化 QApplication，这是 Qt Widget 测试所必需的
    QApplication app(argc, argv);
    
    // 初始化 Google Test
    ::testing::InitGoogleTest(&argc, argv);
    
    // 运行所有测试
    return RUN_ALL_TESTS();
}
