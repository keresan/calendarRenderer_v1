#include "trainingroom.h"

TrainingRoom::TrainingRoom() {}

TrainingRoom::TrainingRoom(QString title, int id, QColor color) {
    _title = title;
    _id = id;
    _color = color;
}

void TrainingRoom::addDay(QDate date) {
    _occupiedDays.append(date);
}

void TrainingRoom::addDay(QList<QDate> list) {
    _occupiedDays.append(list);
}

void TrainingRoom::setColor(QColor color) {
    _color = color;
}

QList<QDate> TrainingRoom::occupiedDays() { return _occupiedDays;}
int TrainingRoom::id() { return _id;}
QString TrainingRoom::title() {return _title;}
QColor TrainingRoom::color() {return _color;}

bool TrainingRoom::isOccupied(QDate date) {
    for(int i = 0; i < _occupiedDays.count(); i++) {
        if(_occupiedDays.at(i) == date ) {
            return true;
        }
    }
    return false;

}
