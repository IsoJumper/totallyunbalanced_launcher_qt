#include "mainwindow.h"
#include "startupcheck.h"
#include <QApplication>
#include <QScreen>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    StartupCheck Checker;
    int IntegrityState = Checker.CheckIntegrity();
    int RequirementsState = Checker.CheckRequirements();
    if (IntegrityState == 1){return 1;}
    if (IntegrityState == 0)
    {
       if (RequirementsState == 1){return 1;}
       if (RequirementsState == 0)
       {
           MainWindow w;
           w.show();
           return a.exec();
       }
    }
    return 0;
}
