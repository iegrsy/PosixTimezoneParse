#include "timezoneparseposix.h"

TimeZoneParsePosix::TimeZoneParsePosix(QString data)
{
	if (data.isEmpty() || data.isNull())
		throw "Empty data.";

	this->data = data;
	init();
}

void TimeZoneParsePosix::init()
{
	loadZones();
	setTZ();
}

void TimeZoneParsePosix::loadZones()
{
	QFile mFile(":/TimeZone/timezonesdb.csv");
	if( !mFile.open(QFile::ReadOnly | QFile::Text) ){
		qDebug() << "could not open file for read";
		return;
	}

	QTextStream in(&mFile);
	QStringList z = in.readAll().split("\n");
	mFile.close();

	QList<QStringList> mzones;
	foreach (QString ms, z) {
		int c = ms.split(",").count() ;
		if (c > 3) {
			mzones.append(ms.replace("\"","").split(","));
		} else if (c == 3){
			QStringList l;
			QStringList s = ms.replace("\"","").split(",");
			l << s.at(0) << s.at(1) << "" << "" << s.at(2);
			mzones.append(l);
		}
	}

	this->zones = mzones;
}

void TimeZoneParsePosix::setTZ()
{
	QStringList tl = data.split(",");
	int c = tl.count();

	if (c == 1){
		foreach (QStringList sl, zones) {
			if ( sl.at(1).compare( tl.at(0)) == 0) {
				TZ = sl.at(0);
				break;
			}
		}
	} else if ( c == 3 ){
		foreach (QStringList sl, zones) {
			if( sl.at(1).contains(tl.at(0)) && sl.at(2).contains(tl.at(1).split("/").first()) && sl.at(3).contains(tl.at(2).split("/").first()) ){
				TZ = sl.at(0);
				break;
			}
		}
	}
}

