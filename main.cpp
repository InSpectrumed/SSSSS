#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //PageHandler iconsPageHandler;

    MinengS _Pars;

    return a.exec();
}
