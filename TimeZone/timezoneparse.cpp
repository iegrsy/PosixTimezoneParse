#include "timezoneparse.h"


static QString MYDEBUG = "Parse ***: ";

TimeZoneParse::TimeZoneParse(QString datastr)
{
	init(datastr);
}

void TimeZoneParse::init(QString data){
	DEFAULT_CHANGEOVER_TIME = 7200000; // Default: 02:00:00
	validParse = false;
	noDST = false;
	startDates << 1 << 1 << 0;
	startTime = DEFAULT_CHANGEOVER_TIME;
	endDates << 1 << 1 << 0;
	endTime = DEFAULT_CHANGEOVER_TIME;
	currentOffset = 0;

	this->TZ = "";
	this->data = data;
	this->maxOffset = data.length();
	this->parse();
	this->setTzName();
}

bool TimeZoneParse::parse() {
	stdName = parseTimezoneName();
	if (stdName == NULL)
		throwParseException("invalid standard timezone", currentOffset);

	stdOffsetMS = parseTimezoneOffset(false);

	dstName = parseTimezoneName();
	if (dstName == NULL) {
		if (currentOffset < maxOffset)
			throwParseException("invalid data encountered", currentOffset);
		noDST = true;
		validParse = true;
		return validParse;
	}

	int tmpOffset = currentOffset;
	dstOffsetMS = parseTimezoneOffset(true);
	if (tmpOffset == currentOffset) {
		dstOffsetMS = stdOffsetMS - 3600000; // default: 1 hour ahead
	}

	if (currentOffset >= maxOffset || data.at(currentOffset++) != ',') {
		throwParseException("not enough information after dst name", currentOffset);
	}

	parseDates(startDates);
	if (currentOffset < maxOffset && data.at(currentOffset++) == '/') {
		startTime = parseTime(false);
	}

	if (currentOffset >= maxOffset || data.at(currentOffset++) != ',')
		throwParseException("missing end dst boundary date", currentOffset);
	parseDates(endDates);
	if (currentOffset < maxOffset && data.at(currentOffset++) == '/') {
		endTime = parseTime(false);
	}
	validParse = true;

	return validParse;
}

void TimeZoneParse::setTzName()
{
	if (!validParse)
		throwParseException("Parse invalid false.", 28);

	QFile mFile(":/TimeZone/date_time_zonespec.csv");
	if( !mFile.open(QFile::ReadOnly | QFile::Text) ){
		qDebug() << "could not open file for read";
		return;
	}

	QTextStream in(&mFile);
	QStringList z = in.readAll().split("\n");
	mFile.close();

	QList<QStringList> zones;
	foreach (QString ms, z) {
		if( ms.split(",").count() > 3 )
			zones.append(ms.replace("\"","").split(","));
	}

	for(int i = 0; i < zones.count(); i++){
		if (noDST){
			QString az = zones.at(i).at(5);
			QString os = QString::number(stdOffsetMS / ( 60*60*1000));
			if( zones.at(i).at(1).compare(stdName) == 0 )
				if(az.contains(os)){ //FIXME: time zone
					TZ = zones.at(i).at(0);
					break;
				}
		}else {
			if( zones.at(i).at(1).compare(stdName) == 0 && zones.at(i).at(3).compare(dstName) == 0){
				QStringList st = zones.at(i).at(7).split(";");
				QStringList et = zones.at(i).at(9).split(";");
				QString az = zones.at(i).at(5);
				QString os = QString::number(stdOffsetMS / ( 60*60*1000));
				if ( !(st.count() == 3 && et.count() == 3) )
					return;

				if (startDates[0] == st.at(2).toInt() &&
						startDates[1] == st.at(0).toInt() &&
						startDates[2] == st.at(1).toInt())
					if(endDates[0] == et.at(2).toInt() &&
							endDates[1] == et.at(0).toInt() &&
							endDates[2] == et.at(1).toInt())
						if(az.contains(os)){ //FIXME: time zone
							TZ = zones.at(i).at(0);
							break;
						}
			}
		}
	}
}

/**
   * Parse "Mm.w.d", representing the [d]ay of week (0-6, Sunday-Saturday) on the [w]eek of the
   * month (0-5, 5="last [d]ay of month), for the [m]onth (1-12).
   *
   * @param date array storing [m,w,d]
   * @
   */
void TimeZoneParse::parseDates(QVector<int> &date) {
	if (currentOffset >= maxOffset || data.at(currentOffset++) != 'M')
		throwParseException("invalid dst boundary date", currentOffset);
	int tmpOffset = currentOffset;
	date[0] = parseNumber();
	if (tmpOffset == currentOffset || date[0] < 1 || date[0] > 12)
		throwParseException("invalid dst month", currentOffset);
	if (currentOffset >= maxOffset || data.at(currentOffset++) != '.')
		throwParseException("invalid dst boundary char", currentOffset);
	tmpOffset = currentOffset;
	date[1] = parseNumber();
	if (tmpOffset == currentOffset || date[1] < 1 || date[1] > 5)
		throwParseException("invalid dst boundary week", currentOffset);
	if (currentOffset >= maxOffset || data.at(currentOffset++) != '.')
		throwParseException("invalid dst boundary char", currentOffset);
	tmpOffset = currentOffset;
	date[2] = parseNumber();
	if (tmpOffset == currentOffset || date[2] < 0 || date[2] > 6)
		throwParseException("invalid dst boundary day", currentOffset);
}

/**
   * Parse the name of the timezone; it must be 3 characters long, not start with a :, have embedded
   * digits, commas, or a +/-. There is no space between the name and the offset.
   * @return timezone name
   */
QString TimeZoneParse::parseTimezoneName() {
	int follow = currentOffset;
	if (follow+3 > maxOffset)
		return NULL;
	if (data.at(currentOffset) == ':') {
		return NULL;
	}

	int nameLength = follow+3;
	while (follow < nameLength) {
		QChar look = data.at(follow);
		if (look.isLetter()) {
			if (look == '-' || look == '+' || look == ',') {
				break;
			}
		} else {
			break;
		}
		follow++;
	}
	if (follow != nameLength)
		return NULL;
	QString ret = data.mid(currentOffset, follow - currentOffset);
	currentOffset = follow;
	return ret;
}

/**
 * Simply parse some digits.
*/
int TimeZoneParse::parseNumber() {
	int follow = currentOffset;
	int value = 0;
	while (follow < maxOffset) {
		QChar look = data.at(follow);
		if(look >= '0' && look <= '9') {
			value = value * 10 + (look.toLatin1() - QChar('0').toLatin1());
		} else {
			break;
		}
		follow++;
	}
	currentOffset = follow;
	return value;
}

/**
  * Parse a time offset, which can be in the format of [+|-]hh[:mm[:ss]].  This is the time
  * added to local time to get UTC.
  * @return prased time
*/
int TimeZoneParse::parseTimezoneOffset(bool isOptional)  {
	if (currentOffset >= maxOffset) {
		if(!isOptional)
			throwParseException("Missing non-optional timezone offset", currentOffset);
		return 0;
	}
	QChar optionalSign = data.at(currentOffset);
	bool negate = false;
	if (optionalSign == '+' || optionalSign == '-') {
		if (optionalSign == '-') negate = true;
		isOptional = false; // If the + is here, we need a number.
		currentOffset++;
	}

	int ret = parseTime(isOptional);
	return (negate == true) ? -ret : ret;
}

/**
  * Parse the [hh[:mm[:ss]]]
  * @ if time is not optional and missing or if it doesn't match the pattern
**/
int TimeZoneParse::parseTime(bool isOptional)  {
	int sec = 0;
	int min = 0;
	int tmpOffset = currentOffset;
	int hour = parseNumber();
	if (tmpOffset == currentOffset) {
		if(!isOptional) {
			throwParseException("Missing non-optional hours", currentOffset);
		}
		return 0;
	}
	if (hour < 0 || hour > 23) throwParseException("Invalid hour", currentOffset);
	if (currentOffset < maxOffset && data.at(currentOffset) == ':') {
		tmpOffset = ++currentOffset;
		min = parseNumber();
		if (tmpOffset == currentOffset) {
			throwParseException("Missing non-optional minutes", currentOffset);
		}
		if (min < 0 || min > 59) throwParseException("Invalid minute", currentOffset);
		if (currentOffset < maxOffset && data.at(currentOffset) == ':') {
			tmpOffset = ++currentOffset;
			sec = parseNumber();
			if (tmpOffset == currentOffset) {
				throwParseException("Missing non-optional seconds", currentOffset);
			}
			if (sec < 0 || sec > 59)
				throwParseException("Invalid second", currentOffset);
		}
	}
	int ret = ((hour * 3600) + (min * 60) + sec) * 1000;
	return ret;
}

void TimeZoneParse::throwParseException(QString msg, int obj){
	qDebug()<< MYDEBUG << msg << " : " << obj;
	validParse = false;
	throw msg;
}
