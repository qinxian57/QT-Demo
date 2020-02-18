#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    //QApplication::setStyle(QStyleFactory::create("Fusion"));//整个程序使用系统内置的windows或Fusion样式

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    a.setStyle(QStyleFactory::create("Fusion"));   	//整个程序使用系统内置的windows或Fusion样式

    return a.exec();
}
