#include "trainingroom.h"

TrainingRoom::TrainingRoom() {}

TrainingRoom::TrainingRoom(QString title, int id, QColor color) {
    _title = title;
    _id = id;
    _color = color;
}

TrainingRoom::TrainingRoom(QString title, int id) {
	_title = title;
	_id = id;
}

/*
void TrainingRoom::addDay(QDate date) {
    _occupiedDays.append(date);
}

void TrainingRoom::addDay(QList<QDate> list) {
    _occupiedDays.append(list);
}
*/
void TrainingRoom::addDay(QList<QDate> list, int eventId) {
	for(int i = 0; i < list.size(); i++) {
		_occupiedDaysNew.append(qMakePair(list[i],eventId));
	}
}

/*
void TrainingRoom::setColor(QColor color) {
    _color = color;
}
*/

QList<QPair<QDate, int> > TrainingRoom::occupiedDays() { return _occupiedDaysNew;}

int TrainingRoom::id() { return _id;}
QString TrainingRoom::title() {return _title;}
//QColor TrainingRoom::color() {return _color;}

/**
 * @brief Vyhlada eventId pre dany den
 * @param date
 * @return eventId pre dany den; -1 ak den nenajde
 */
int TrainingRoom::eventId(QDate date) {
	for(int i = 0; i < _occupiedDaysNew.count(); i++) {
		if(_occupiedDaysNew.at(i).first == date ) {
			return _occupiedDaysNew.at(i).second;
		}
	}
	return -1;
}

bool TrainingRoom::isOccupied(QDate date) {
	for(int i = 0; i < _occupiedDaysNew.count(); i++) {
		if(_occupiedDaysNew.at(i).first == date ) {
            return true;
        }
    }
    return false;
}
