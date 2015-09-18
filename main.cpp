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
        QMessageBox::critical(nullptr,QStringLiteral("TotallyUnbalanced Launcher"), QStringLiteral("Your screen configuration is invalid. Please double-check your settings."));
		return 1;
	}
	// Screen is tiny?
	if( (qApp->primaryScreen()->size().width() < 640) || (qApp->primaryScreen()->size().height() < 360) )
	{
        QMessageBox::critical(nullptr,QStringLiteral("TotallyUnbalanced Launcher"), QStringLiteral("Your primary screen is too tiny to run Totally Unbalanced."));
		return 1;
	}
    QDir EngineDirectory(QDir::currentPath()+"/Engine");
    QDir TotallyUnballoncedDirectory(QDir::currentPath()+"/TotallyUnballonced");
    QDir ContentDirectory(QDir::currentPath()+"/TotallyUnballonced/Content");
    if (EngineDirectory.exists() == false)
    {
       QMessageBox::critical(nullptr,QStringLiteral("TotallyUnbalanced Launcher"),QStringLiteral("The Engine directory is missing,please verify the cache through Steam."));
       return 1;
    }
    if (TotallyUnballoncedDirectory.exists() == false)
    {
       QMessageBox::critical(nullptr,QStringLiteral("TotallyUnbalanced Launcher"),QStringLiteral("The TotallyUnbalanced game folders are missing,please verify the cache through Steam."));
       return 1;
    }
    if (ContentDirectory.exists() == false)
    {
       QMessageBox::critical(nullptr,QStringLiteral("TotallyUnbalanced Launcher"),QStringLiteral("The Game Content folder is missing,please verify the cache through Steam."));
       return 1;
    }
	MainWindow w;
	w.show();
	return a.exec();
}
