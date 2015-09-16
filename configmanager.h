#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "QString"

class ConfigManager
{
public:
    ConfigManager();
    QString CurrentDir;
    void CreateConfig();
    void LoadConfig();

private:

};

#endif // CONFIGMANAGER_H
