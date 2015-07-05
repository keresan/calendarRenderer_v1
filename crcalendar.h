#ifndef CRCALENDAR_H
#define CRCALENDAR_H

#include "cell.h"
#include "crtablewidget.h"

#include "calendardata.h"
#include "settings.h"

#include <QObject>
#include <QList>
#include <QtAlgorithms>
#include <QDomNode>
#include <QFile>
#include <QHash>
#include <QHashIterator>
#include <QStringList>
#include <QColor>
#include <QMap>

class CrCalendar : public QObject {
   Q_OBJECT
public:
    explicit CrCalendar(QDate first, QDate last, QObject *parent = 0);

    //load
    void loadDataFromXml(QString path);
    bool readXmlFile(QString path);
QString loadTagValue(QString tagName, QDomNode *parent);

    //utility
    void setCss(QString css);
    void addCss(QString css);
    QString css();
    QString getMonthString(int monthNumber);
    void loadMonthNames();

    void setDates(QDate first, QDate last);

    //generate calendar
    void generateCalendar();
    QString generateMonthTitle();
    QString generateDayNumber();
    QString generateEvents(listOfListOfEvents &list, QString title, bool showHeadTitle);
    QString generateEventsAfterDeadline(listOfListOfEvents &added, listOfListOfEvents &cancelled, QString title, QString kpi);
    QString generateEventsFromList(listOfListOfEvents &list, bool highlightEndTr, bool StrikenTitle = false);
    QString generateEventLine(QList<CrEvent> &list, QString className);
    QString generateEmptyLine();
    QString generateRooms(CalendarData::RoomOrInstructor what);
    QString generateWorkActivity();

    QString getHtml();

    //lepsie bude vycelnit _compressedListOfEvents do samostatnej static triedy
    //friend class CrTableWidget;

    //static listOfListOfEvents _compressedListOfEvents;

    QString getListOfClassAndColorsFromList(listOfListOfEvents &list, int &counter);
    QString getListOfClassAndColors();
    QString getClassNameOfRow(int index);

    QString getListOfClassAndColorsOfWorkActivity();

    QString getKPI(listOfListOfEvents &beforeLock, listOfListOfEvents &afterLockAdded, listOfListOfEvents &afterLockCancelled);

signals:
    void signalErrorMsg(QString smg);
    void signalWarningMsg(QString smg);
    void signalInfoMsg(QString smg);


private:
    QDate _dateFirst;
    QDate _dateLast;

    QList<CrEvent> _listEvents;

    CalendarData _calendarData;
    /*
     * ked sa uz listOfList vytvori, poradove cislo listOfEvents sa berie ako nazov triedy, podla ktorej
     * sa urcuje farba pri vykreslovani
     */

    //listOfListOfEvents _compressedListOfWorkAction;

    QString _html;
    QString _css;
    QString _addedCss;

    //trainingRoomMap _trainingRooms; //miestnosti
    //trainingRoomMap _trainingCoach; //skolitelia
    QStringList _monthNames;

    int _eventCounter; //pocitadlo udalosti, musi byt aby sa pocitali skolenia pridane po uzavierke


};



#endif // CRCALENDAR_H
