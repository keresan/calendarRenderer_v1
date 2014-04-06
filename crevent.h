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
 *
 */



class CrEvent {
public:
    CrEvent(QString courseTitle,
            int titleId,
            QDate begin,
            int duration,
            int roomId,
            int hours,
            QColor color,
            QString department,
            int id);

    /**
     * @brief vyuziva sa aj pri qSort
     */
    enum EventType {
        workAction = 0,
        Course = 1
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
    int roomId();
    int hours();
    QColor color();
    QString department();

    void setColor(QColor color);


    void setEventType(EventType type);

    int id();
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
    int _priority; // pre rozlisenie inych aktivit, napr. generalna oprava - najvyssia priorita
    QString _title;
    int _titleId;
    int _roomId;
    QColor _color;
    QString _department; //utvrat

    EventType _eventType;

    CrEvent::DayOfWeeks _activeDays;


};

typedef QList<QList<CrEvent> > listOfListOfEvents;
typedef QList<CrEvent> listOfEvents;

Q_DECLARE_OPERATORS_FOR_FLAGS(CrEvent::DayOfWeeks);

#endif // CREVENT_H
