#include <QApplication>
#include <QTextCodec>
#include "main_window.h"

int main(int argc, char *argv[]) {
    // 设置 UTF-8 编码支持中文
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    
    QApplication app(argc, argv);
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}
