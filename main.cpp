#include "mainwindow.h"
#include "startupcheck.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	try {
		QString errorText;
		if(!CheckIntegrity(errorText))
		{
			QMessageBox::critical(nullptr,QStringLiteral("Totally Unbalanced Launcher"),errorText);
			return 1;
		}
	} catch(std::exception& e)
	{
		QMessageBox::critical(nullptr,QStringLiteral("Totally Unbalanced Launcher"),QLatin1String(e.what()));
		return 1;
	}
	
	MainWindow w;
	w.show();
	return a.exec();
	return 0;
}
