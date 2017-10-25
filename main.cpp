#include <QCoreApplication>
#include <TimeZone/timezoneparse.h>
#include <QDebug>
#include <QString>
#include <time.h>

static QString MYDEBUG = "Main ****: ";

static void testFailingString(QString tz) {
	try {
		TimeZoneParse time = TimeZoneParse(tz);
		if (!time.validParse)
			return;
		qDebug()<< MYDEBUG << tz << " ==> " << time.TZ;

	} catch (...) {
		qDebug()<< MYDEBUG << "Parse error failling!!!";
	}
}

static void testPassingSimpleZone(QString tz) {
	try {
		TimeZoneParse time = TimeZoneParse(tz);
		if (!time.validParse)
			return;
		qDebug()<< MYDEBUG << tz << " ==> " << time.TZ;
	} catch (...) {
		qDebug()<< MYDEBUG << "Parse error passing!!!";
	}
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	testFailingString("");
	testFailingString("CO"); // must be 3
	testFailingString("CODE"); // must be 3
	testFailingString(":CO"); // no leading :
	testFailingString("C0D"); // no numbers
	testFailingString("CO+"); // no +
	testFailingString("C-D"); // no -
	testFailingString("CST!"); // offset must be number, optional +/-
	testFailingString("CST+3A"); // DST must be 3 characters
	testFailingString("CST+3AB"); // DST must be 3 characters
	// If you have DST, you must have the rest of the ",start[/time],end[/time]"
	testFailingString("CST6CDT");
	testFailingString("CST+3CDTZ");
	testFailingString("CST+3CDT3");
	testFailingString("CST+3CDT-3");
	testFailingString("CST+3CDT+3");
	testFailingString("CST+3CDT!3");
	testFailingString("CST+3CDT+3:2");
	testFailingString("CST+3CDT+3:2:1");
	testFailingString("CST+3CDT+3:2:1,");
	testFailingString("CST+3CDT+3:2:1,");
	testFailingString("CST+3CDT+3:2:1!");

	// Mess with 3:2:1
	testFailingString("CST+3CDT-x:2:1,M3.2.0/2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT-3:x:1,M3.2.0/2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT-3:2:x,M3.2.0/2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT-3:2:x!M3.2.0/2:00,M11.1.0/2:00:00");

	// Mess with Mm.n.d
	testFailingString("CST+3CDT,M0.2.0/2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT,M13.2.0/2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT,Mx.2.0/2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT,M3.0.0/2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT,M3.6.0/2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT,M3.x.0/2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT,M3.2.7/2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT,M3.2.x/2:00,M11.1.0/2:00:00");

	// Mess with time of day change
	testFailingString("CST+3CDT,M3.2.0/+2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT,M3.2.0/-2:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT,M3.2.0/24:00,M11.1.0/2:00:00");
	testFailingString("CST+3CDT,M3.2.0/2:60,M11.1.0/2:00:00");
	testFailingString("CST+3CDT,M3.2.0/2:00:60,M11.1.0/2:00:00");

	testFailingString("CST+3CDT,M3.2.0/2:00:60XM11.1.0/2:00:00");

	// We don't support Jn yet (Julian)
	testFailingString("CST+3CDT,J1,M11.1.0/2:00:00");
	// We don't support 0-365 yet
	testFailingString("CST+3CDT,1,M11.1.0/2:00:00");
	testPassingSimpleZone("CST3"); // DST is optional and stops parsing. Allows for EST+3
	testPassingSimpleZone("CST+3"); // DST is optional and stops parsing. Allows for EST+3
	testPassingSimpleZone("CST-3"); // DST is optional and stops parsing. Allows for EST+3
	testPassingSimpleZone("CST+3CDT,M3.2.0/2:00,M11.1.0/2:00");
	testPassingSimpleZone("CST+3CDT,M3.2.0/2:00,M11.1.0/2");
	testPassingSimpleZone("CVS+3CDT,M3.2.0/2:00,M11.1.0/2:0");

	testPassingSimpleZone("PST2PDT,M3.2.0/01:00,M11.1.0/02:00");
	testPassingSimpleZone("CST+3CDT-3:2:1,M3.2.0/2:00,M11.1.0/2:00");
	testPassingSimpleZone("CST+3CDT-3:2:1,M3.2.0/2:00,M11.1.0/2");
	testPassingSimpleZone("CST+3CDT-3:2:1,M3.2.0/2:00,M11.1.0/2:0");
	testPassingSimpleZone("CST+3CDT,M3.2.0/2:00,M11.1.0/2:00:00");
	testPassingSimpleZone("CST+3CDT-3:2:1,M3.2.0/2:00,M11.1.0/2:00:00");
	return 0;
}

