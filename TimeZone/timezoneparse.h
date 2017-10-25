#ifndef TIMEZONEPARSE_H
#define TIMEZONEPARSE_H

#include <QString>
#include <QObject>
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTimeZone>

class TimeZoneParse
{
	int DEFAULT_CHANGEOVER_TIME;
	QString data;
	int currentOffset;
	int maxOffset;

	int parseTimezoneOffset(bool isOptional);
	void parseDates(QVector<int> &date);
	int parseTime(bool isOptional);
	QString parseTimezoneName();
	int parseNumber();
	void setTzName();

	bool parse();
	void init(QString data);

	void throwParseException(QString msg, int obj);
public:
	bool validParse;
	bool noDST;

	QString stdName;
	int stdOffsetMS;

	QString dstName;
	int dstOffsetMS;

	QVector<int> startDates;
	int startTime;
	QVector<int> endDates;
	int endTime;

	QString TZ;

	TimeZoneParse(QString str);
};

#endif // TIMEZONEPARSE_H
