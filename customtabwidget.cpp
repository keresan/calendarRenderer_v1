#include "customtabwidget.h"

CustomTabWidget::CustomTabWidget(QWidget *parent): QTabWidget(parent) {

    initTableEvent();
    initTagDescription();

   // qDebug() << "CustomTabWidget(): size" << _tableEvent->horizontalHeader()->size();
    //qDebug() << "CustomTabWidget(): horizontalHeader" << _tableEvent->horizontalHeader()->width();
   // _tableEvent->resize(_tableEvent->horizontalHeader()->size());

   // _tableEvent->col
    //QTableWidget::horizontalHeader().setStretchLastSection(true);
    //_tableEvent->horizontalHeader()->.setStretchLastSection(true);


    //qDebug() << "CustomTabWidget(): horizontalHeader" << _tableEvent->horizontalHeader()->width();



    //_tableEvent->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    //qDebug() << "CustomTabWidget(): _tableEvent sizeHint" <<_tableEvent->sizeHint();
    //qDebug() << "CustomTabWidget(): _tableEvent size" <<_tableEvent->size();


}

void CustomTabWidget::initTagDescription() {

    QStringList rowCaptions;

    rowCaptions.append("tag");
    rowCaptions.append("hodnota");
    rowCaptions.append("význam");
    _tagDescription = new TagDescriptor(rowCaptions);

    this->addTab(_tagDescription, "podporované tagy");
    _tagDescription->loadData();

}

void CustomTabWidget::initTableEvent() {

    QStringList rowCaptions;

    rowCaptions.append("farba");
    rowCaptions.append("názov školenia");

    _tableEvent = new CrTableWidget(rowCaptions);
    _tableEvent->setColumnWidth(0,150);
    _tableEvent->setColumnWidth(1,300);

    //vlozit vzdy na prve miesto
    this->insertTab(0,_tableEvent, "školenia");
    this->setCurrentIndex(0);
}

void CustomTabWidget::loadEvents() {

    int counter = 0;
    _tableEvent->setRowCount(0);

    loadEventsFromList(_calendarData._listOfEvents,counter);

    _startRowEventAdded = counter;
    loadEventsFromList(_calendarData._listOfEventsAdded,counter);

    _startRowEventCancelled = counter;
    loadEventsFromList(_calendarData._listOfEventsCancelled,counter);

    _startRowSoftSkill = counter;
    loadEventsFromList(_calendarData._listOfSoftskill,counter);

    _startRowSoftSkillAdded = counter;
    loadEventsFromList(_calendarData._listOfSoftskillAdded,counter);

    _startRowSoftSkillCancelled = counter;
    loadEventsFromList(_calendarData._listOfSoftskillCancelled,counter);
}

void CustomTabWidget::loadEventsFromList(listOfListOfEvents &list, int &counter) {
    listOfListOfEvents::iterator listOfListIterator;

    //int row = 0;

     //rows count
    _tableEvent->setRowCount(_tableEvent->rowCount() + list.count());

    //listOfEvents
    for(listOfListIterator = list.begin(); listOfListIterator != list.end(); ++listOfListIterator) {

        //row number
        _tableEvent->setVerticalHeaderItem(counter, new QTableWidgetItem(QString::number(counter+1)));

        //color
        QTableWidgetItem *newItem = new QTableWidgetItem(listOfListIterator->first().colorDay().name());
        newItem->setBackgroundColor(listOfListIterator->first().colorDay());
        newItem->setTextAlignment(Qt::AlignCenter);
        newItem->setFlags( Qt::ItemIsEnabled);
        _tableEvent->setItem(counter, 0, newItem);

        //title
        newItem = new QTableWidgetItem(listOfListIterator->first().title());
        newItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        _tableEvent->setItem(counter, 1, newItem);
        counter++;
    }


}

void CustomTabWidget::selectedEventHandler(QColor *color) {

    qDebug() << "hey hou, lets go";

    //CrTableWidget *table = (CrTableWidget*)this->currentWidget();

    *color = _tableEvent->currentItem()->backgroundColor();

   // qDebug() << "selectedEventHandler() current row: " << row;
}

int CustomTabWidget::mapTableRowToEventLine(int tableRow) {

    if(tableRow < _startRowEventAdded) {
        return tableRow;
    } else if(tableRow >= _startRowEventAdded && tableRow < _startRowEventCancelled) {
        return tableRow - _startRowEventAdded;
    } else if(tableRow >= _startRowEventCancelled && tableRow < _startRowSoftSkill) {
        return tableRow - _startRowEventCancelled;
    } else if(tableRow >= _startRowSoftSkill && tableRow < _startRowSoftSkillAdded) {
        return tableRow - _startRowSoftSkill;
    } else if(tableRow >= _startRowSoftSkillAdded && tableRow < _startRowSoftSkillCancelled) {
        return tableRow - _startRowSoftSkillAdded;
    } else {
        return tableRow - _startRowSoftSkillCancelled;
    }
}

QColor CustomTabWidget::getCurrentRowColor() {

    //CrTableWidget *table = (CrTableWidget*)this->currentWidget();

    int row = _tableEvent->currentRow();
    if(row < 0) {
        qDebug() << "return color: " << QColor().name();
        return QColor();
    }
    return _tableEvent->item(row,0)->backgroundColor();
}

void CustomTabWidget::setCurrentRowColor(QColor color) {

    //CrTableWidget *table =  (CrTableWidget*)this->currentWidget();

    //change color in table
    int row = _tableEvent->currentRow();

    _tableEvent->item(row,0)->setBackgroundColor(color);
    _tableEvent->item(row,0)->setText(color.name());

    //change color in calendar data
    int tableRow;

    if(row < _startRowEventAdded) {
        tableRow = row;
        changeLineEventColor(_calendarData._listOfEvents, tableRow, color);
    } else if(row >= _startRowEventAdded && row < _startRowEventCancelled) {
        tableRow = row - _startRowEventAdded;
        changeLineEventColor(_calendarData._listOfEventsAdded, tableRow, color);
    } else if(row >= _startRowEventCancelled && row < _startRowSoftSkill) {
        tableRow = row - _startRowEventCancelled;
        changeLineEventColor(_calendarData._listOfEventsCancelled, tableRow, color);
    } else if(row >= _startRowSoftSkill && row < _startRowSoftSkillAdded) {
        tableRow = row - _startRowSoftSkill;
        changeLineEventColor(_calendarData._listOfSoftskill, tableRow, color);
    } else if(row >= _startRowSoftSkillAdded && row < _startRowSoftSkillCancelled) {
        tableRow = row - _startRowSoftSkillAdded;
        changeLineEventColor(_calendarData._listOfSoftskillAdded, tableRow, color);
    } else {
        tableRow = row  - _startRowSoftSkillCancelled;
        changeLineEventColor(_calendarData._listOfSoftskillCancelled, tableRow, color);
    }


}

void CustomTabWidget::changeLineEventColor(listOfListOfEvents &list, int index, QColor color) {

    QList<CrEvent>::iterator eventIterator = list[index].begin();

    while (eventIterator < list[index].end()) {
        eventIterator->setColor(color);
        eventIterator++;
    }
}

void CustomTabWidget::changeRoomColor(int index, QColor color, CalendarData::RoomOrInstructor what) {
    trainingRoomMap *trainingSet;

    if(what == CalendarData::room) {
        trainingSet = &_calendarData._trainingRooms;
        qDebug() << "room";
    } else if(what == CalendarData::instructor) {
        trainingSet = &_calendarData._trainingInstructor;
        qDebug() << "instructor";
    } else {
        assert(false);
    }


    trainingRoomMap::iterator roomIterator;
    int row = 0;
    for(roomIterator = trainingSet->begin(); roomIterator != trainingSet->end(); ++roomIterator) {

        if(row == index) {
            //roomIterator->setColor(color);
            qDebug() << "zmeneny index=" << index;
            break;
        }
        row++;
    }
}

void CustomTabWidget::clearTables() {

    delete _tableEvent;

    initTableEvent();
}

void CustomTabWidget::loadData() {
    this->loadEvents();
}

bool CustomTabWidget::isActiveRow() {
    if(_tableEvent->currentRow() < 0) {
        return false;
    }
    return true;
}
