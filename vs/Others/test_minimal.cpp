#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "Before QApplication" << std::endl;
    QApplication a(argc, argv);
    std::cout << "After QApplication" << std::endl;
    return 0;
}
