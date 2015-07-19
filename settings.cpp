#include "settings.h"

QString Settings::tagKurzNazov;
QString Settings::tagKurzId;
QString Settings::tagDniTrvanie;
QString Settings::tagHodTrvanie;
QString Settings::tagTypSkolenia;
QString Settings::tagOptPracAktivita;
QString Settings::tagDatumZaciatok;
QString Settings::tagSkoliacaMiestnostNazov;
QString Settings::tagSkoliacaMiestnostId;
QString Settings::tagSkolitelNazov;
QString Settings::tagSkolitelId;
QString Settings::tagAktivDen;
QString Settings::tagFarbaSkolenia;
QString Settings::tagUtvarCislo;
QString Settings::tagZamokStav;
QString Settings::tagKurzVolitelnyText;

QString Settings::tagOptSoftskill;
QString Settings::tagOptPridaneSkolenie;
QString Settings::tagOptZruseneSkolenie;

QString Settings::eventLabel;
QString Settings::roomLabel;
QString Settings::instructorLabel;
QString Settings::departmentLabel;
QString Settings::eventAfterDeadlineLabel;
QString Settings::calendarTitleLabel;
QString Settings::softSkillLabel;
QString Settings::softSkillAfterDeadlineLabel;
QString Settings::calendarVersion;

bool Settings::generateEvent = true;
bool Settings::generateRoom = true;
bool Settings::generateInstructor = true;
bool Settings::generateSoftskill = true;

float Settings::widthEventTitle;


Settings::Settings() {}

void Settings::setConstants() {
    tagKurzNazov = "kurz_nazov";
    tagKurzId = "kurz_id";
    tagDniTrvanie = "dni_trvanie";
    tagHodTrvanie = "hod_trvanie";
    tagTypSkolenia = "typ_skolenia";
    tagDatumZaciatok = "datum_zaciatok";
    tagSkoliacaMiestnostNazov = "skoliaca_miestnost_nazov";
    tagSkoliacaMiestnostId = "skoliaca_miestnost_id";
    tagSkolitelNazov = "skolitel_nazov";
    tagSkolitelId = "skolitel_id";
    tagAktivDen = "aktiv_den";
    tagFarbaSkolenia = "farba_skolenia";
    tagUtvarCislo = "utvar_cislo";
    tagZamokStav = "zamok_stav";
    tagKurzVolitelnyText = "kurz_volitelny_text";

    tagOptSoftskill = "soft_skill";
    tagOptPracAktivita = "prac_aktivita";
    tagOptPridaneSkolenie = "pridane_po_zamknuti";
    tagOptZruseneSkolenie = "zrusene_po_zamknuti";

    eventLabel = "Odborné školenie";
    eventAfterDeadlineLabel = "Odborné školenie po uzamknutí";
    roomLabel = "Miestnosť";
    instructorLabel = "Inštruktor";
    departmentLabel = "Útvar";

    calendarTitleLabel = "PLÁN ŠKOLENÍ";
    softSkillLabel = "Mäkká zručnosť (soft-skill)";
    softSkillAfterDeadlineLabel = "Mäkká zručnosť po uzamknutí";

    widthEventTitle = 15.0;

    calendarVersion = "2015.07.18";
}
