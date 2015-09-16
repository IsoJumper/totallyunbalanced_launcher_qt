#include "mainwindow.h"
#include <QApplication>
#include "QDir"
#include "QFile"
#include "QSettings"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QSettings QS;
    return a.exec();
}
