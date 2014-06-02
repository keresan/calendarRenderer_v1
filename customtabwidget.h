#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include "crtablewidget.h"
#include "crcalendar.h"


#include <QTabWidget>

class CustomTabWidget : public QTabWidget {
    Q_OBJECT
public:
    explicit CustomTabWidget(QWidget *parent = 0);

    void loadData();


    QColor getCurrentRowColor();
    void setCurrentRowColor(QColor color);

    void clearTables();

    bool isActiveRow();

public slots:
    void selectedEventHandler(QColor *color);
signals:
    //void currentEvent();

private:
	CrTableWidget *_tableEvent; // *_tableRoom, *_tableInstructor;

    //load data
    void initTableEvent();

    void loadEvents();
	void loadEventsFromList(listOfListOfEvents &list, int &counter);

    //change data
    void changeLineEventColor(listOfListOfEvents &list, int index, QColor color);
    void changeEventColor(QColor color);
	void changeRoomColor(int index, QColor color, CalendarData::RoomOrInstructor what);


    int mapTableRowToEventLine(int tableRow);

    //konstanta, udava od ktoreho riadku v _tableEvent zacina vypisovanie udajov
    // z _listOfEventsAfterDeadline
    int _startRowEventAfterDeadline;
	int _startRowSoftSkill;
	int _startRowSoftSkillAfterDeadline;





    CalendarData _calendarData;
};

#endif // CUSTOMTABWIDGET_H
