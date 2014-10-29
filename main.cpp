#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Egor Churaev");
    QCoreApplication::setApplicationName("Draftsman");
    MainWindow w;
    w.show();

    return a.exec();
}
