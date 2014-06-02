#ifndef CREVENT_H
#define CREVENT_H

#include <QDate>
#include <QDebug>
#include <QFlags>
#include <assert.h>
#include <daysofweeks.h>
#include <QColor>

/**
 * udrziava informacie o konkretnom evente
 */
class CrEvent {
public:
	CrEvent(QString courseTitle,
			int titleId,
			QDate begin,
			int duration,
			int hours,
			QColor colorDay,
			QString optionalText,
			QString department,
			int eventId);

    /**
     * @brief vyuziva sa aj pri qSort
     */
    enum EventType {
        workAction = 0,
		softskill = 1,
		Course = 2
    };

    enum DayOfWeek {
        Monday = 1 << 0,
        Tuesday = 1 << 1,
        Wednesday = 1 << 2,
        Thursday = 1 << 3,
        Friday = 1 << 4,
        Saturday = 1 << 5,
        Sunday = 1 << 6
    };

    Q_DECLARE_FLAGS(DayOfWeeks, CrEvent::DayOfWeek);

    QDate dateBegin();
    QDate dateEnd();
    int duration();
    EventType eventType();
    QString title();
    int titleId();
	//int roomId();
    int hours();
	QColor colorDay();
	QString optionalText();
    QString department();

	void setColor(QColor colorDay);


    void setEventType(EventType type);

	int eventId();
    bool isOverlap(CrEvent &other);
    void calculateDateEnd();

    void addActiveDay(CrEvent::DayOfWeek day);
    void addActiveDay(QString str);
    void removeActiveDay(CrEvent::DayOfWeek day);
    bool testActiveDay(CrEvent::DayOfWeek day);

    CrEvent::DayOfWeeks activeDays();
    Qt::DayOfWeek convertToQtDayOfWeek(CrEvent::DayOfWeek day);
    CrEvent::DayOfWeek convertFromQtDayOfWeek(Qt::DayOfWeek day);

    static bool lessThanCourseType(CrEvent &e1, CrEvent &e2);
	static bool lessThenDateBegin(CrEvent &e1, CrEvent &e2);

    QList<QDate> getEventDays();
    QString getClassNameOfId();

private:
    QDate _dateBegin;
    QDate _dateEnd; //pre zistenie, ci sa 2 udalosti casovo prekryvaju
    int _duration;
	int _hours; //trvanie v hodinach, dolezite, ak event trva 1 den a menej ako 8h
	int _id;
	QString _title;
    int _titleId;
	QString _optionalText;

	QColor _colorDay; //farba pre vykreslenie udalosti v kalendary
	QString _department; //utvar


    EventType _eventType;
    CrEvent::DayOfWeeks _activeDays;

};

typedef QList<QList<CrEvent> > listOfListOfEvents;
typedef QList<CrEvent> listOfEvents;

Q_DECLARE_OPERATORS_FOR_FLAGS(CrEvent::DayOfWeeks);

#endif // CREVENT_H
