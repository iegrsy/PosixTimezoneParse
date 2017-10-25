#include <QCoreApplication>
#include <TimeZone/timezoneparse.h>
#include <timezoneparseposix.h>
#include <QDebug>
#include <QString>
#include <time.h>

static QString MYDEBUG = "Main ****: ";

static void testTimezoneParsePosix(QString tz) {
	try {
		TimeZoneParsePosix time = TimeZoneParsePosix(tz);
		if (!time.TZ.isEmpty())
			qDebug()<< MYDEBUG << tz << " ==> " << time.TZ;

	} catch (...) {
		qDebug()<< MYDEBUG << "Parse error failling!!!";
	}
}

static void testTimezoneParse(QString tz) {
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
#if 0
	testTimezoneParse("");
	testTimezoneParse("CO"); // must be 3
	testTimezoneParse("CODE"); // must be 3
	testTimezoneParse(":CO"); // no leading :
	testTimezoneParse("C0D"); // no numbers
	testTimezoneParse("CO+"); // no +
	testTimezoneParse("C-D"); // no -
	testTimezoneParse("CST!"); // offset must be number, optional +/-
	testTimezoneParse("CST+3A"); // DST must be 3 characters
	testTimezoneParse("CST+3AB"); // DST must be 3 characters
	// If you have DST, you must have the rest of the ",start[/time],end[/time]"
	testTimezoneParse("CST6CDT");
	testTimezoneParse("CST+3CDTZ");
	testTimezoneParse("CST+3CDT3");
	testTimezoneParse("CST+3CDT-3");
	testTimezoneParse("CST+3CDT+3");
	testTimezoneParse("CST+3CDT!3");
	testTimezoneParse("CST+3CDT+3:2");
	testTimezoneParse("CST+3CDT+3:2:1");
	testTimezoneParse("CST+3CDT+3:2:1,");
	testTimezoneParse("CST+3CDT+3:2:1,");
	testTimezoneParse("CST+3CDT+3:2:1!");

	// Mess with 3:2:1
	testTimezoneParse("CST+3CDT-x:2:1,M3.2.0/2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT-3:x:1,M3.2.0/2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT-3:2:x,M3.2.0/2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT-3:2:x!M3.2.0/2:00,M11.1.0/2:00:00");

	// Mess with Mm.n.d
	testTimezoneParse("CST+3CDT,M0.2.0/2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT,M13.2.0/2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT,Mx.2.0/2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT,M3.0.0/2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT,M3.6.0/2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT,M3.x.0/2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT,M3.2.7/2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT,M3.2.x/2:00,M11.1.0/2:00:00");

	// Mess with time of day change
	testTimezoneParse("CST+3CDT,M3.2.0/+2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT,M3.2.0/-2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT,M3.2.0/24:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT,M3.2.0/2:60,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT,M3.2.0/2:00:60,M11.1.0/2:00:00");

	testTimezoneParse("CST+3CDT,M3.2.0/2:00:60XM11.1.0/2:00:00");

	// We don't support Jn yet (Julian)
	testTimezoneParse("CST+3CDT,J1,M11.1.0/2:00:00");
	// We don't support 0-365 yet
	testTimezoneParse("CST+3CDT,1,M11.1.0/2:00:00");
	testTimezoneParse("CST3"); // DST is optional and stops parsing. Allows for EST+3
	testTimezoneParse("CST+3"); // DST is optional and stops parsing. Allows for EST+3
	testTimezoneParse("CST-3"); // DST is optional and stops parsing. Allows for EST+3
	testTimezoneParse("CST+3CDT,M3.2.0/2:00,M11.1.0/2:00");
	testTimezoneParse("CST+3CDT,M3.2.0/2:00,M11.1.0/2");
	testTimezoneParse("CVS+3CDT,M3.2.0/2:00,M11.1.0/2:0");

	testTimezoneParse("PST2PDT,M3.2.0/01:00,M11.1.0/02:00");
	testTimezoneParse("CST+3CDT-3:2:1,M3.2.0/2:00,M11.1.0/2:00");
	testTimezoneParse("CST+3CDT-3:2:1,M3.2.0/2:00,M11.1.0/2");
	testTimezoneParse("CST+3CDT-3:2:1,M3.2.0/2:00,M11.1.0/2:0");
	testTimezoneParse("CST+3CDT,M3.2.0/2:00,M11.1.0/2:00:00");
	testTimezoneParse("CST+3CDT-3:2:1,M3.2.0/2:00,M11.1.0/2:00:00");
#endif
	testTimezoneParsePosix("CST6CDT,M4.1.0/2,M10.5.0/2"); // America/Bahia_Banderas
	testTimezoneParsePosix("MST7MDT,M3.2.0/2,M11.1.0/2"); // America/Cambridge_Bay
	testTimezoneParsePosix("CST6CDT,M3.2.0/2,M11.1.0/2"); // America/Chicago
	testTimezoneParsePosix("EST5EDT,M3.2.0/2,M11.1.0/2"); // America/Fort_Wayne
	testTimezoneParsePosix("IST-2IDT,M3.4.4/2,M10.5.0/2"); // Asia/Jerusalem
	testTimezoneParsePosix("AMT4"); // Brazil/West
	testTimezoneParsePosix("CKT10"); // Pacific/Rarotonga
	testTimezoneParsePosix(""); // Empty

	testTimezoneParse("CST6CDT,M4.1.0/2,M10.5.0/2"); // America/Bahia_Banderas
	testTimezoneParse("MST7MDT,M3.2.0/2,M11.1.0/2"); // America/Cambridge_Bay
	testTimezoneParse("CST6CDT,M3.2.0/2,M11.1.0/2"); // America/Chicago
	testTimezoneParse("EST5EDT,M3.2.0/2,M11.1.0/2"); // America/Fort_Wayne
	testTimezoneParse("IST-2IDT,M3.4.4/2,M10.5.0/2"); // Asia/Jerusalem
	testTimezoneParse("AMT4"); // Brazil/West
	testTimezoneParse("CKT10"); // Pacific/Rarotonga

	return 0;
}
