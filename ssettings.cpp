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
#include "ssettings.h"
#include <QFile>
#include <QDataStream>
#include <QRect>

SSettingsIni::SSettingsIni(const QString &path, QSettings::Format)
	: m_filepath(path), m_dirty(false) { if(!load()) m_data.clear(); }
SSettingsIni::~SSettingsIni() {
	if(m_dirty)
		save();
}

enum ReadState {
	RS_EXPECT_SECT = 0,
	RS_EXPECT_SECT_END,
	RS_EXPECT_KEY_OR_SECT,
	RS_EXPECT_KEY_END,
	RS_EXPECT_VAL,
	RS_EXPECT_VAL_END
};

// From Qt5.5-source (qtbase/src/corelib/io/qsettings.cpp)
QStringList splitArgs(QString const& s, int idx)
{
	int l = s.length();
	Q_ASSERT(l > 0);
	Q_ASSERT(s.at(idx) == QLatin1Char('('));
	Q_ASSERT(s.at(l - 1) == QLatin1Char(')'));

	QStringList result;
	QString item;

	for (++idx; idx < l; ++idx) {
		QChar c = s.at(idx);
		if (c == QLatin1Char(')')) {
			Q_ASSERT(idx == l - 1);
			result.append(item);
		} else if (c == QLatin1Char(' ')) {
			result.append(item);
			item.clear();
		} else {
			item.append(c);
		}
	}

	return result;
}

// From Qt5.5-source (qtbase/src/corelib/io/qsettings.cpp)
static inline QVariant stringToVariant(QString const& s)
{
	if (s.startsWith(QLatin1Char('@'))) {
		if (s.endsWith(QLatin1Char(')'))) {
			if (s.startsWith(QLatin1String("@ByteArray("))) {
				return QVariant(s.toLatin1().mid(11, s.size() - 12));
			} else if (s.startsWith(QLatin1String("@Variant("))) {
#ifndef QT_NO_DATASTREAM
				QByteArray a(s.toLatin1().mid(9));
				QDataStream stream(&a, QIODevice::ReadOnly);
				stream.setVersion(QDataStream::Qt_4_0);
				QVariant result;
				stream >> result;
				return result;
#else
				Q_ASSERT(!"SSettingsIni: Cannot load custom types without QDataStream support");
#endif
#ifndef QT_NO_GEOM_VARIANT
			} else if (s.startsWith(QLatin1String("@Rect("))) {
				QStringList args = splitArgs(s, 5);
				if (args.size() == 4)
					return QVariant(QRect(args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt()));
			} else if (s.startsWith(QLatin1String("@Size("))) {
				QStringList args = splitArgs(s, 5);
				if (args.size() == 2)
					return QVariant(QSize(args[0].toInt(), args[1].toInt()));
			} else if (s.startsWith(QLatin1String("@Point("))) {
				QStringList args = splitArgs(s, 6);
				if (args.size() == 2)
					return QVariant(QPoint(args[0].toInt(), args[1].toInt()));
#endif
			} else if (s == QLatin1String("@Invalid()")) {
				return QVariant();
			}

		}
		if (s.startsWith(QLatin1String("@@")))
			return QVariant(s.mid(1));
	}

	return QVariant(s);
}

// From Qt5.5-source (qtbase/src/corelib/io/qsettings.cpp)
static inline QString variantToString(QVariant const& v)
{
	QString result;
	
	switch (v.type()) {
		case QVariant::Invalid:
			result = QLatin1String("@Invalid()");
			break;

		case QVariant::ByteArray: {
			QByteArray a = v.toByteArray();
			result = QLatin1String("@ByteArray(");
			result += QString::fromLatin1(a.constData(), a.size());
			result += QLatin1Char(')');
			break;
		}

		case QVariant::String:
		case QVariant::LongLong:
		case QVariant::ULongLong:
		case QVariant::Int:
		case QVariant::UInt:
		case QVariant::Bool:
		case QVariant::Double:
		case QVariant::KeySequence: {
			result = v.toString();
			if (result.startsWith(QLatin1Char('@')))
				result.prepend(QLatin1Char('@'));
			break;
		}
#ifndef QT_NO_GEOM_VARIANT
		case QVariant::Rect: {
			QRect r = qvariant_cast<QRect>(v);
			result += QLatin1String("@Rect(");
			result += QString::number(r.x());
			result += QLatin1Char(' ');
			result += QString::number(r.y());
			result += QLatin1Char(' ');
			result += QString::number(r.width());
			result += QLatin1Char(' ');
			result += QString::number(r.height());
			result += QLatin1Char(')');
			break;
		}
		case QVariant::Size: {
			QSize s = qvariant_cast<QSize>(v);
			result += QLatin1String("@Size(");
			result += QString::number(s.width());
			result += QLatin1Char(' ');
			result += QString::number(s.height());
			result += QLatin1Char(')');
			break;
		}
		case QVariant::Point: {
			QPoint p = qvariant_cast<QPoint>(v);
			result += QLatin1String("@Point(");
			result += QString::number(p.x());
			result += QLatin1Char(' ');
			result += QString::number(p.y());
			result += QLatin1Char(')');
			break;
		}
#endif // !QT_NO_GEOM_VARIANT

		default: {
#ifndef QT_NO_DATASTREAM
			QByteArray a;
			{
				QDataStream s(&a, QIODevice::WriteOnly);
				s.setVersion(QDataStream::Qt_4_0);
				s << v;
			}

			result = QLatin1String("@Variant(");
			result += QString::fromLatin1(a.constData(), a.size());
			result += QLatin1Char(')');
#else
			Q_ASSERT(!"SSettingsIni: Cannot save custom types without QDataStream support");
#endif
			break;
		}
	}
	
	return result;
}

bool SSettingsIni::load()
{
	m_dirty = false;
	QFile input(m_filepath);
	input.open(QFile::ReadOnly | QFile::Text);
	m_data.clear();
	
	ReadState rs = RS_EXPECT_SECT;
	QByteArray f = input.readAll();
	f.append('\n');
	
	QString currentSection;
	QString currentKey;
	QString currentVal;
	for(auto it = f.begin(); it != f.end(); ++it)
	{
		QChar ch = QLatin1Char(*it);
		
		if(rs == RS_EXPECT_SECT)
		{
			currentSection.clear();
			if(ch.isSpace()) {}
			else if(ch == QLatin1Char('[')) {
				currentSection.clear();
				currentKey.clear();
				currentVal.clear();
				rs = RS_EXPECT_SECT_END;
			}
			else
				return false;
		}
		else if(rs == RS_EXPECT_SECT_END)
		{
			if(ch == QLatin1Char('\n'))
				return false;
			else if(ch == QLatin1Char(']')) {
				currentSection = currentSection.trimmed();
				currentKey.clear();
				currentVal.clear();
				rs = RS_EXPECT_KEY_OR_SECT;
			}
			else currentSection += ch;
		}
		else if(rs == RS_EXPECT_KEY_OR_SECT)
		{
			if(ch == QLatin1Char('[')) {
				currentSection.clear();
				currentKey.clear();
				currentVal.clear();
				rs = RS_EXPECT_SECT_END;
			}
			else if(ch.isSpace()) {}
			else { currentKey += ch; rs = RS_EXPECT_KEY_END; }
		}
		else if(rs == RS_EXPECT_KEY_END)
		{
			if(ch == QLatin1Char('\n'))
				return false;
			else if(ch == QLatin1Char('=')) {
				currentKey = currentKey.trimmed();
				rs = RS_EXPECT_VAL;
			}
			else currentKey += ch;
		}
		else if(rs == RS_EXPECT_VAL)
		{
			if(ch == QLatin1Char('\n'))
				return false;
			else if(ch.isSpace()) {}
			else { currentVal += ch; rs = RS_EXPECT_VAL_END; }
		}
		else if(rs == RS_EXPECT_VAL_END)
		{
			if(ch == QLatin1Char('\n'))
			{
				// We got the value fully
				m_data[currentSection][currentKey] = stringToVariant(currentVal.trimmed());
				
				currentKey.clear();
				currentVal.clear();
				rs = RS_EXPECT_KEY_OR_SECT;
			}
			else currentVal += ch;
		}
	}
	return (rs == RS_EXPECT_KEY_OR_SECT) || (rs == RS_EXPECT_SECT);
}
void SSettingsIni::save()
{
	m_dirty = false;
	QFile output(m_filepath);
	output.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
	for(auto it = m_data.begin(); it != m_data.end(); ++it)
	{
		QString const& key = it->first;
		std::map<QString, QVariant> const& value = it->second;
		
		output.write("[");
		output.write(key.toLocal8Bit());
		output.write("]\n");
		
		for(auto itb = value.begin(); itb != value.end(); ++itb)
		{
			QString const& key = itb->first;
			QString value = variantToString(itb->second);
			output.write(key.toLocal8Bit());
			output.write(" = ");
			output.write(value.toLocal8Bit());
			output.write("\n");
			/*QVariant value = itb->second;
			if(value.convert(QMetaType::QString))
			{
				output.write(key);
				output.write(" = ");
				output.write(value.toString());
				output.write("\n");
			}*/
		}
		output.write("\n");
	}
}
QVariant SSettingsIni::value(const QString& key, QVariant def) const
{
	auto it = m_data.find(m_currentgroup);
	if(it == m_data.end())
		return def;
	
	auto itb = it->second.find(key);
	if(itb == it->second.end())
		return def;
	
	return itb->second;
}
void SSettingsIni::setValue(const QString& key, const QVariant& val)
{
	if(!val.isValid())
	{
		remove(key);
		return;
	}
	
	m_data[m_currentgroup][key] = val;
	m_dirty = true;
}
void SSettingsIni::remove(const QString& key)
{
	auto it = m_data.find(m_currentgroup);
	if(it == m_data.end())
		return;
	auto itb = it->second.find(key);
	if(itb == it->second.end())
		return;
	it->second.erase(itb);
	m_dirty = true;
	
	if(it->second.empty())
		m_data.erase(it);
}
void SSettingsIni::beginGroup(const QString &group)
{
	m_currentgroup += group;
	m_groups.push_back(m_currentgroup);
}
void SSettingsIni::endGroup()
{
	m_currentgroup.clear();
	if(m_groups.empty()) return;
	m_groups.pop_back();
	if(!m_groups.empty()) m_currentgroup = m_groups.back();
}
