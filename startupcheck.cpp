#include "startupcheck.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QScreen>
#include <map>
#include "checksum.h"

static std::map<std::string,ChecksumStorageInfo> const g_requiredFiles = {
	// {"Name.jpg","checksum"}
    {"bridgeMP.exe",{"",FLAG_WINANY}},
	{"bridge",{"",FLAG_LINUX}}
};

static std::vector<std::string> const g_requiredFolders = {
    "/Engine",
    "/TotallyUnballoncedMP",
    "/TotallyUnballoncedMP/Content"
};

bool CheckIntegrity(QString& errorText)
{
	errorText.clear();
	
	auto generic_error = [&](QString const& e) -> bool {
		errorText = (!e.isEmpty()) ? e : QStringLiteral("Could not check game requirements. Please verify your system settings.");
		return false;
	};
	auto folder_missing_error = [&](QString const& foldername) -> bool {
		if(foldername.isEmpty())
		{
			errorText = QStringLiteral("A game folder is missing. Please verify your game contents.");
		}
		else
		{
			errorText = QStringLiteral("Game folder \"");
			errorText += foldername;
			errorText += QStringLiteral("\" is missing. Please verify your game contents.");
		}
		return false;
	};
	auto file_missing_error = [&](QString const& filename) -> bool {
		if(filename.isEmpty())
		{
			errorText = QStringLiteral("A game file is missing. Please verify your game contents.");
		}
		else
		{
			errorText = QStringLiteral("Game file \"");
			errorText += filename;
			errorText += QStringLiteral("\" is missing. Please verify your game contents.");
		}
		return false;
	};
	auto checksum_error = [&](QString const& filename) -> bool {
		if(filename.isEmpty())
		{
			errorText = QStringLiteral("A game file is invalid. Please verify your game contents.");
		}
		else
		{
			errorText = QStringLiteral("Game file \"");
			errorText += filename;
			errorText += QStringLiteral("\" is invalid. Please verify your game contents.");
		}
		return false;
	};
	
	if(!qApp->primaryScreen())
		return generic_error(QStringLiteral("Your screen configuration is invalid. Please verify your system settings."));
	if( (qApp->primaryScreen()->size().width() < 640) || (qApp->primaryScreen()->size().height() < 360))
		return generic_error(QStringLiteral("Your primary screen is too tiny to run Totally Unballonced."));
	
	QString baseDirectory = QDir::currentPath();
	for(auto it = g_requiredFolders.begin(); it != g_requiredFolders.end(); ++it)
	{
		if(!QDir(baseDirectory + QString::fromStdString(*it)).exists())
			return folder_missing_error(QString::fromStdString(*it));
	}
	
	std::string cs;
	for(auto it = g_requiredFiles.begin(); it != g_requiredFiles.end(); ++it)
	{
		if(it->second.Required())
		{
			if(!FileChecksum(it->first,cs))
				return file_missing_error(QString::fromStdString(it->first));
			if(!it->second.Compare(cs))
				return checksum_error(QString::fromStdString(it->first));
		}
	}
	return true;
}
