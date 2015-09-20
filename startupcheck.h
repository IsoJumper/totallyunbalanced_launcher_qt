#ifndef STARTUPCHECK_H
#define STARTUPCHECK_H
#include <QDir>
#include <QFile>



class StartupCheck
{
public:
    StartupCheck();
    int CheckIntegrity();
    int CheckRequirements();
private:
    // Directories
    QDir EngineDirectory;
    QDir TUDirectory;
    QDir ContentDirectory;
    // Files
    QFileInfo ProjectFile;
    QFileInfo Bridge;

};

#endif // STARTUPCHECK_H
