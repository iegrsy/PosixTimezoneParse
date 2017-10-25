#ifndef TIMEZONEPARSEPOSIX_H
#define TIMEZONEPARSEPOSIX_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class TimeZoneParsePosix
{
	QString data;
	QList<QStringList> zones;
public:
	QString TZ;
	TimeZoneParsePosix(QString data);
private:
	void init();
	void loadZones();
	void setTZ();
};

#endif // TIMEZONEPARSEPOSIX_H
