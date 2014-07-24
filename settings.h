#ifndef SETTINGS_H
#define SETTINGS_H

#include "qstring.h"

class Settings
{
public:
	Settings();

	static void setConstants();

	static QString tagKurzNazov;
	static QString tagKurzId;
	static QString tagDniTrvanie;
	static QString tagHodTrvanie;
	static QString tagTypSkolenia;

	static QString tagDatumZaciatok;
	static QString tagSkoliacaMiestnostNazov;
	static QString tagSkoliacaMiestnostId;
	static QString tagSkolitelNazov;
	static QString tagSkolitelId;
	static QString tagAktivDen;
	static QString tagFarbaSkolenia;
	static QString tagUtvarCislo;
	static QString tagZamokStav;
	static QString tagKurzVolitelnyText;
	static QString tagOptPridaneSkolenie;
	static QString tagOptZruseneSkolenie;

	static QString tagOptPracAktivita;
	static QString tagOptSoftskill;

	static QString calendarTitleLabel;
	static QString eventLabel;
	static QString roomLabel;
	static QString instructorLabel;
	static QString departmentLabel;
	static QString eventAfterDeadlineLabel;
	static QString softSkillLabel;
	static QString softSkillAfterDeadlineLabel;

	static bool generateEvent,generateSoftskill, generateRoom, generateInstructor;

	static float widthEventTitle;

	static QString calendarVersion;
};

#endif // SETTINGS_H
