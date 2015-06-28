#ifndef CALENDARDATA_H
#define CALENDARDATA_H

#include "stdexcept"

#include "crevent.h"
#include "trainingroom.h"


class CalendarData {
public:
    CalendarData();

    enum RoomOrInstructor {
        room,
        instructor
    };

    enum Deadline {
        before,
        added,
        cancelled
    };

    static listOfListOfEvents _listOfEvents; // skolenia
    static listOfListOfEvents _listOfEventsAdded; // skolenia pridane po deadline
    static listOfListOfEvents _listOfEventsCancelled; // skolenia zrusene po deadline
    static listOfListOfEvents _listOfSoftskill; // soft skills
    static listOfListOfEvents _listOfSoftskillAdded;
    static listOfListOfEvents _listOfSoftskillCancelled;
    static listOfListOfEvents _listOfWorkAction; // ine pracovne aktivity

    static trainingRoomMap _trainingInstructor; //skolitelia
    static trainingRoomMap _trainingRooms; //miestnosti

    static listOfListOfEvents createCompressedEventList(QList<CrEvent> &list);
    static void createCompressedWorkActivityList(QList<CrEvent> &list);

    static void sortCompressedList(listOfListOfEvents &list);

    static void clearData();
    static int getEventsCount(listOfListOfEvents &list);

    void getRowColor(int eventId, int &eventRow);
    void getColorFromList(int eventId,listOfListOfEvents &list, int &eventRow);

    //print
    void printCompressedList();
    void printCompressedList(listOfListOfEvents &list);
    void printRoom(int id);
    static void printList(QList<CrEvent> &list);

private:

    static bool canAdd(CrEvent &event, QList<CrEvent> &list);
    static void add(CrEvent &event, QList<QList<CrEvent> > &list);
    static void sortList(QList<CrEvent> &list, bool sortOnlyByDate = false);
    static bool LessThanDate(QList<CrEvent> &list1, QList<CrEvent> &list2);


};



#endif // CALENDARDATA_H
