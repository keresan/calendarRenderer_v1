#ifndef TRAININGROOM_H
#define TRAININGROOM_H

#include <QString>
#include <QDate>
#include <QList>
#include <QColor>
#include <QMap>

/**
 * @brief Trieda reprezentujuca miestnost skolenia alebo instruktora.
 */
class TrainingRoom {
public:
    TrainingRoom();
    TrainingRoom(QString title, int id, QColor color);

    TrainingRoom(QString title, int id);

    //void addDay(QDate date);
    //void addDay(QList<QDate> list);

    void addDay(QList <QDate> list, int eventId);

    int id();
    int eventId(QDate date);
    QString title();
    //QColor color();
    //void setColor(QColor color);

    QList<QPair<QDate, int> > occupiedDays();


    bool isOccupied(QDate date);

private:
    int _id;
    QString _title;
    QColor _color;
    QList<QDate> _occupiedDays;

    // list dvojic < den, id-event>
    // podla id-event sa bude zistovat farba vypisu, ktora moze byt pre
    // jednotlive udalosti v tej istej miestnosti rozdielna
    QList<QPair<QDate, int> > _occupiedDaysNew;

};

typedef QVector<TrainingRoom> TrainingRoomList;

typedef QMap<int, TrainingRoom> trainingRoomMap;

#endif // TRAININGROOM_H
