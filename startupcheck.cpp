#include "startupcheck.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QScreen>

StartupCheck::StartupCheck()
{
    // Core Dirs
    EngineDirectory = QDir::currentPath()+"/Engine";
    TUDirectory = QDir::currentPath()+"/TotallyUnballonced";
    ContentDirectory = QDir::currentPath()+"/TotallyUnballonced/Content";
    // Core Files
    ProjectFile = (QDir::currentPath()+"/TotallyUnballonced/TotallyUnballonced.uproject");
    Bridge = (QDir::currentPath()+"/Bridge.exe");
}

int StartupCheck::CheckIntegrity()
{
    // return 0 = everything good
    // return 1 = something's missing
    // return 2 = MD5 mismatch
    //
    //
    // Folders
    if (EngineDirectory.exists() == false)
    {
       QMessageBox::critical(nullptr,QStringLiteral("TotallyUnbalanced Launcher"),QStringLiteral("The Engine directory is missing,please verify the cache through Steam."));
       return 1;
    }
    if (TUDirectory.exists() == false)
    {
       QMessageBox::critical(nullptr,QStringLiteral("TotallyUnbalanced Launcher"),QStringLiteral("The TotallyUnbalanced game folders are missing,please verify the cache through Steam."));
       return 1;
    }
    if (ContentDirectory.exists() == false)
    {
       QMessageBox::critical(nullptr,QStringLiteral("TotallyUnbalanced Launcher"),QStringLiteral("The Game Content folder is missing,please verify the cache through Steam."));
       return 1;
    }
    // Files
    if (ProjectFile.exists() == false)
    {
        QMessageBox::critical(nullptr,QStringLiteral("TotallyUnbalanced Launcher"),QStringLiteral("The Game uproject file is missing,please verify the cache through Steam."));
        return 1;
    }
    if (Bridge.exists() == false)
    {
        QMessageBox::critical(nullptr,QStringLiteral("TotallyUnbalanced Launcher"),QStringLiteral("Core executable is missing,please verify the cache through Steam."));
        return 1;
    }
    return 0;
}

int StartupCheck::CheckRequirements()
{
    // return 0 = everything good
    // return 1 = requirements not met
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
    return 0;
}

