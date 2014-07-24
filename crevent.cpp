#include "crevent.h"


CrEvent::CrEvent(QString courseTitle,
				 int courseId,
				 QDate begin,
				 int duration,
				 int hours,
				 QColor colorDay,
				 QString optionalText,
				 QString department,
				 int id) {

    _title = courseTitle;
    _titleId = courseId;
    _dateBegin = begin;
    _duration = duration;
    _hours = hours;
	_colorDay = colorDay;
	_optionalText = optionalText;
    _department = department;
    _id = id;

    _dateEnd = _dateBegin.addDays(-1); // treba nastavit po vytvoreni eventu zavolanim calculateEndDate()

    _eventType = CrEvent::Course;

}

QDate CrEvent::dateBegin() { return _dateBegin; }
QDate CrEvent::dateEnd() { return _dateEnd; }
int CrEvent::hours() { return _hours; }

int CrEvent::duration() { return _duration; }
int CrEvent::eventId() { return _id; }
int CrEvent::titleId() { return _titleId; }
QString CrEvent::title() { return _title; }
QColor CrEvent::colorDay() {return _colorDay;}
QString CrEvent::department() { return _department; }
QString CrEvent::optionalText() {return _optionalText; }

CrEvent::EventType CrEvent::eventType() { return _eventType; }
void CrEvent::setEventType(EventType type) {
    _eventType = type;
}

void CrEvent::setColor(QColor color) {
	_colorDay = color;
}

QString CrEvent::getClassNameOfId() {
    QString str;
    str = "class_id_"+QString::number(_id);
    return str;
}

/**
 * @brief zisti, ci sa dany event casovo kryje s &other
 * @param other
 * @return
 */
bool CrEvent::isOverlap(CrEvent &other) {

    return !( other.dateEnd() < this->dateBegin()
            || other.dateBegin() > this->dateEnd() );

}

/**
 * @brief calculate end date of event from begin date and duration
 */
void CrEvent::calculateDateEnd() {

    assert(!_activeDays.operator !());

    int restDuration = _duration;

    QDate daysIterator = _dateBegin;


    do {
        CrEvent::DayOfWeek day = convertFromQtDayOfWeek((Qt::DayOfWeek)daysIterator.dayOfWeek());
        if(testActiveDay(day)) {
            restDuration --;
            _dateEnd = daysIterator;
        }

        daysIterator = daysIterator.addDays(1);
    } while(restDuration > 0);


}

QList<QDate> CrEvent::getEventDays() {

    QList<QDate> returnList;

    assert(!_activeDays.operator !());


    for(QDate dayIterator = _dateBegin; dayIterator <= _dateEnd; dayIterator = dayIterator.addDays(1)) {
        //qDebug() << dayIterator.toString("dd.MMM.yyyy");

        if(testActiveDay(convertFromQtDayOfWeek((Qt::DayOfWeek)(dayIterator.dayOfWeek())))) {
            returnList.append(dayIterator);
           // qDebug() << "is active";
        }
    }

    return returnList;
}



/***********************************************
 * _activeDays stuff
 ***********************************************/

void CrEvent::addActiveDay(CrEvent::DayOfWeek day) {
    _activeDays.operator |=(day);
}

/**
 * @brief aktivne dni zadane vo formate: "1234567"
 * @param str
 */
void CrEvent::addActiveDay(QString str) {

    if(str.contains('1')) { this->addActiveDay(CrEvent::Monday); }
    if(str.contains('2')) { this->addActiveDay(CrEvent::Tuesday); }
    if(str.contains('3')) { this->addActiveDay(CrEvent::Wednesday); }
    if(str.contains('4')) { this->addActiveDay(CrEvent::Thursday); }
    if(str.contains('5')) { this->addActiveDay(CrEvent::Friday); }
    if(str.contains('6')) { this->addActiveDay(CrEvent::Saturday); }
    if(str.contains('7')) { this->addActiveDay(CrEvent::Sunday); }

}

void CrEvent::removeActiveDay(CrEvent::DayOfWeek day) {
    _activeDays.operator &=(~day);
}

bool CrEvent::testActiveDay(CrEvent::DayOfWeek day) {
    return _activeDays.testFlag(day);
}

CrEvent::DayOfWeeks CrEvent::activeDays() {
    return _activeDays;
}

CrEvent::DayOfWeek CrEvent::convertFromQtDayOfWeek(Qt::DayOfWeek day) {
    CrEvent::DayOfWeek returnDay;
    switch(day) {
        case Qt::Monday: returnDay = CrEvent::Monday; break;
        case Qt::Tuesday: returnDay = CrEvent::Tuesday; break;
        case Qt::Wednesday: returnDay = CrEvent::Wednesday; break;
        case Qt::Thursday: returnDay = CrEvent::Thursday; break;
        case Qt::Friday: returnDay = CrEvent::Friday; break;
        case Qt::Saturday: returnDay = CrEvent::Saturday; break;
        case Qt::Sunday: returnDay = CrEvent::Sunday; break;
    default: qDebug() << day ; assert(false);
    }
    return returnDay;
}

Qt::DayOfWeek CrEvent::convertToQtDayOfWeek(CrEvent::DayOfWeek day) {
    Qt::DayOfWeek returnDay;
    switch(day) {
        case CrEvent::Monday: returnDay = Qt::Monday; break;
        case CrEvent::Tuesday: returnDay = Qt::Tuesday; break;
        case CrEvent::Wednesday: returnDay = Qt::Wednesday; break;
        case CrEvent::Thursday: returnDay = Qt::Thursday; break;
        case CrEvent::Friday: returnDay = Qt::Friday; break;
        case CrEvent::Saturday: returnDay = Qt::Saturday; break;
        case CrEvent::Sunday: returnDay = Qt::Sunday; break;
        default: assert(false);
    }
    return returnDay;
}
/**
 * @brief CrEvent::lessThanCourseType - radenie podla eventType, titleId, optionalText, dateBegin
 * @param e1
 * @param e2
 * @return
 */
bool CrEvent::lessThanCourseType(CrEvent &e1, CrEvent &e2) {
	//compare by courseType
    if (e1.eventType() < e2.eventType()) {
        return true;
    }

    if(e1.eventType() == e2.eventType()) {
        //compate by courseId
        if(e1.titleId() < e2.titleId()) {
            return true;
        }

		if(e1.titleId() == e2.titleId()) {
			int cmpResult = e1.optionalText().compare(e2.optionalText());
			if(cmpResult < 0) {
				//compare by optional text
				return true;
			} else if(cmpResult == 0) {
				//compare by dateBegin
				return e1.dateBegin()  < e2.dateBegin();
			}
		}
    }

    return false;
}

bool CrEvent::lessThenDateBegin(CrEvent &e1, CrEvent &e2) {
	//compare by date begin
	if(e1.dateBegin() < e2.dateBegin()) {
		return true;
	} else {
		return false;
	}
}
