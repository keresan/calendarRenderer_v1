#include "customtabwidget.h"

CustomTabWidget::CustomTabWidget(QWidget *parent): QTabWidget(parent) {

    initTableEvent();
    initTableRoom();
    initTableCoach();

   // qDebug() << "CustomTabWidget(): size" << _tableEvent->horizontalHeader()->size();
    qDebug() << "CustomTabWidget(): horizontalHeader" << _tableEvent->horizontalHeader()->width();
   // _tableEvent->resize(_tableEvent->horizontalHeader()->size());

   // _tableEvent->col
    //QTableWidget::horizontalHeader().setStretchLastSection(true);
    //_tableEvent->horizontalHeader()->.setStretchLastSection(true);


    qDebug() << "CustomTabWidget(): horizontalHeader" << _tableEvent->horizontalHeader()->width();



    //_tableEvent->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    qDebug() << "CustomTabWidget(): _tableEvent sizeHint" <<_tableEvent->sizeHint();
    qDebug() << "CustomTabWidget(): _tableEvent size" <<_tableEvent->size();
    qDebug() << "CustomTabWidget(): _tableRoom size" <<_tableRoom->size();
    qDebug() << "CustomTabWidget(): _tableCoach size" <<_tableCoach->size();




}


void CustomTabWidget::initTableEvent() {

    QStringList rowCaptions;

    rowCaptions.append("farba");
    rowCaptions.append("názov školenia");
    //rowCaptions.append("miestnost");
    //rowCaptions.append("skolitel");

    _tableEvent = new CrTableWidget(rowCaptions);
    _tableEvent->setColumnWidth(0,150);
    _tableEvent->setColumnWidth(1,100);



    this->addTab(_tableEvent, "školenia");
}

void CustomTabWidget::initTableRoom() {

    QStringList rowCaptions;
    rowCaptions.append("farba");
    rowCaptions.append("školiaca miestnosť");

    _tableRoom = new CrTableWidget(rowCaptions);
    _tableRoom->setColumnWidth(0,150);
    _tableRoom->setColumnWidth(1,100);

    this->addTab(_tableRoom, "miestnosti");
}

void CustomTabWidget::initTableCoach() {

    QStringList rowCaptions;

    rowCaptions.append("farba");
    rowCaptions.append("meno inštruktora");

    _tableCoach = new CrTableWidget(rowCaptions);
    _tableCoach->setColumnWidth(0,150);
    _tableCoach->setColumnWidth(1,100);
    this->addTab(_tableCoach, "inštruktori");
}

void CustomTabWidget::loadEvents() {
    _eventCounter = 0;
    _tableEvent->setRowCount(0);

    loadEventsFromList(_calendarData._listOfEvents);

    _startRowEventAfterDeadline = _eventCounter;

    loadEventsFromList(_calendarData._listOfEventsAfterDeadline);

}

void CustomTabWidget::loadEventsFromList(listOfListOfEvents &list) {
    listOfListOfEvents::iterator listOfListIterator;

    //int row = 0;

     //rows count
    _tableEvent->setRowCount(_tableEvent->rowCount() + list.count());

    //listOfEvents
    for(listOfListIterator = list.begin(); listOfListIterator != list.end(); ++listOfListIterator) {

        //row number
        _tableEvent->setVerticalHeaderItem(_eventCounter, new QTableWidgetItem(QString::number(_eventCounter+1)));

        //color
        QTableWidgetItem *newItem = new QTableWidgetItem(listOfListIterator->first().color().name());
        newItem->setBackgroundColor(listOfListIterator->first().color());
        newItem->setTextAlignment(Qt::AlignCenter);
        newItem->setFlags( Qt::ItemIsEnabled);
        _tableEvent->setItem(_eventCounter, 0, newItem);

        //title
        newItem = new QTableWidgetItem(listOfListIterator->first().title());
        newItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        _tableEvent->setItem(_eventCounter, 1, newItem);
        _eventCounter++;
    }


}

void CustomTabWidget::loadRooms(CalendarData::RoomOrCoach what) {
    trainingRoomMap roomsMap;
    CrTableWidget *table;

    if(what == CalendarData::room) {
        roomsMap = _calendarData._trainingRooms;
        table = _tableRoom;
    } else if(what == CalendarData::coach) {
        roomsMap = _calendarData._trainingCoach;
        table = _tableCoach;
    } else {
        assert(false);
    }

    trainingRoomMap::iterator roomIterator;
    int row = 0;

    //rows count
    table->setRowCount(roomsMap.count());

    for(roomIterator = roomsMap.begin(); roomIterator != roomsMap.end(); ++roomIterator) {
        //row number
        table->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(row+1)));

        //color
        QTableWidgetItem *newItem = new QTableWidgetItem(roomIterator->color().name());
        newItem->setBackgroundColor(roomIterator->color());
        newItem->setTextAlignment(Qt::AlignCenter);
        newItem->setFlags( Qt::ItemIsEnabled);
        table->setItem(row, 0, newItem);

        //title
        newItem = new QTableWidgetItem(roomIterator->title());
        newItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        table->setItem(row, 1, newItem);

        row++;
    }

}

void CustomTabWidget::selectedEventHandler(QColor *color) {

    qDebug() << "hey hou, lets go";

    CrTableWidget *table = (CrTableWidget*)this->currentWidget();

    *color = table->currentItem()->backgroundColor();

   // qDebug() << "selectedEventHandler() current row: " << row;
}

int CustomTabWidget::mapTableRowToEventLine(int tableRow) {
   //zatial len tak, bude treba prerobit ked sa prida vypis pracovnych aktivit

    if(tableRow < _startRowEventAfterDeadline) {
        return tableRow;
    } else {
        return tableRow - _startRowEventAfterDeadline;
    }
}

QColor CustomTabWidget::getCurrentRowColor() {

    CrTableWidget *table = (CrTableWidget*)this->currentWidget();

    int row = table->currentRow();
    if(row < 0) {
        qDebug() << "blle";
        qDebug() << "return color: " << QColor().name();
        return QColor();
    }

    return table->item(row,0)->backgroundColor();
}

void CustomTabWidget::setCurrentRowColor(QColor color) {

    CrTableWidget *table =  (CrTableWidget*)this->currentWidget();

    //change color in table
    int row = table->currentRow();

    table->item(row,0)->setBackgroundColor(color);
    table->item(row,0)->setText(color.name());

    //change color in calendar data
    if(table == _tableEvent) {
        if(row < _startRowEventAfterDeadline) {
            changeLineEventColor(_calendarData._listOfEvents, mapTableRowToEventLine(row), color);
        } else {
            changeLineEventColor(_calendarData._listOfEventsAfterDeadline, mapTableRowToEventLine(row), color);
        }

    } else if(table == _tableRoom) {
        changeRoomColor(row, color, CalendarData::room);
    } else if(table == _tableCoach) {
        changeRoomColor(row, color, CalendarData::coach);
    }
}

void CustomTabWidget::changeLineEventColor(listOfListOfEvents &list, int index, QColor color) {

    QList<CrEvent>::iterator eventIterator = list[index].begin();

    while (eventIterator < list[index].end()) {
        eventIterator->setColor(color);
        eventIterator++;
    }
}

void CustomTabWidget::changeRoomColor(int index, QColor color, CalendarData::RoomOrCoach what) {
    trainingRoomMap *trainingSet;

    if(what == CalendarData::room) {
        trainingSet = &_calendarData._trainingRooms;
        qDebug() << "room";
    } else if(what == CalendarData::coach) {
        trainingSet = &_calendarData._trainingCoach;
        qDebug() << "coach";
    } else {
        assert(false);
    }


    trainingRoomMap::iterator roomIterator;
    int row = 0;
    for(roomIterator = trainingSet->begin(); roomIterator != trainingSet->end(); ++roomIterator) {

        if(row == index) {
            roomIterator->setColor(color);
            qDebug() << "zmeneny index=" << index;
            break;
        }
        row++;
    }
}

void CustomTabWidget::clearTables() {

    delete _tableCoach;
    delete _tableEvent;
    delete _tableRoom;

    initTableEvent();
    initTableRoom();
    initTableCoach();
}

void CustomTabWidget::loadData() {
    this->loadEvents();
    this->loadRooms(CalendarData::room);
    this->loadRooms(CalendarData::coach);
}


bool CustomTabWidget::isActiveRow() {
    if(_tableCoach->currentRow() < 0 && _tableEvent->currentRow() < 0 && _tableRoom->currentRow() < 0) {
        return false;
    }
    return true;
}
