#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QScreen>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	// Screen is missing?
	if(!qApp->primaryScreen())
	{
		QMessageBox::critical(nullptr,QStringLiteral("Totally Unbalanced Launcher"), QStringLiteral("Your screen configuration is invalid. Please double-check your settings."));
		return 1;
	}
	// Screen is tiny?
	if( (qApp->primaryScreen()->size().width() < 640) || (qApp->primaryScreen()->size().height() < 360) )
	{
		QMessageBox::critical(nullptr,QStringLiteral("Totally Unbalanced Launcher"), QStringLiteral("Your primary screen is too tiny to run Totally Unbalanced."));
		return 1;
	}
	// Launcher in the wrong place?
	/*if(!QDir("./TotallyUnballonced").exists())
	{
		QMessageBox::critical(nullptr,QStringLiteral("Totally Unbalanced Launcher"), QStringLiteral("Game files are missing. Please verify your game installation."));
		return 1;
	}*/
	
	MainWindow w;
	w.show();
	return a.exec();
}
