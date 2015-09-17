/*	Copyright (c) 2015 SGH
**	
**	Permission is granted to use, modify and redistribute this software.
**	Modified versions of this software MUST be marked as such.
**	
**	This software is provided "AS IS". In no event shall
**	the authors or copyright holders be liable for any claim,
**	damages or other liability. The above copyright notice
**	and this permission notice shall be included in all copies
**	or substantial portions of the software.
**	
**	File created on: 17/09/2015
*/

#ifndef SSETTINGS_H
#define SSETTINGS_H
#include <QSettings>
#include <QVariant>
#include <map>

class SSettingsIni {
	QString m_filepath;
	
	QStringList m_groups;
	QString m_currentgroup;
	
	std::map<QString, std::map<QString, QVariant>> m_data;
	bool m_dirty;
	
	bool load();
	void save();
public:
	// Format can only be IniFormat. It is actually ignored.
	SSettingsIni(QString const& path, QSettings::Format = QSettings::IniFormat);
	~SSettingsIni();
	
	void beginGroup(QString const& group);
	void endGroup();
	
	QVariant value(QString const&, QVariant = QVariant()) const;
	void setValue(QString const&, QVariant const&);
	void remove(QString const&);
};

#endif // SSETTINGS_H
