#ifndef TRAININGROOM_H
#define TRAININGROOM_H

#include <QString>
#include <QDate>
#include <QList>
#include <QColor>
#include <QMap>



class TrainingRoom {
public:
    TrainingRoom();
    TrainingRoom(QString title, int id, QColor color);


    void addDay(QDate date);
    void addDay(QList<QDate> list);

    int id();
    QString title();
    QColor color();
    QList<QDate> occupiedDays();
    void setColor(QColor color);

    bool isOccupied(QDate date);



private:
    int _id;
    QString _title;
    QColor _color;
    QList<QDate> _occupiedDays;


};

typedef QMap<int, TrainingRoom> trainingRoomMap;

#endif // TRAININGROOM_H
