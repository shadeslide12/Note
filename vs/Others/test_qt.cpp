#include <QApplication>
#include <QWidget>
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "Before QApplication" << std::endl;
    QApplication a(argc, argv);
    std::cout << "After QApplication" << std::endl;
    
    QWidget w;
    w.show();
    std::cout << "After QWidget" << std::endl;
    
    return a.exec();
}
