#include "calendardata.h"

listOfListOfEvents CalendarData::_listOfWorkAction;
listOfListOfEvents CalendarData::_listOfEvents;
listOfListOfEvents CalendarData::_listOfEventsAfterDeadline;
listOfListOfEvents CalendarData::_listOfSoftskill;
listOfListOfEvents CalendarData::_listOfSoftskillAfterDeadline;

trainingRoomMap CalendarData::_trainingInstructor;
trainingRoomMap CalendarData::_trainingRooms;


CalendarData::CalendarData() {

}

void CalendarData::clearData() {
    _listOfWorkAction.clear();
    _listOfEvents.clear();
	_trainingInstructor.clear();
    _trainingRooms.clear();

}

/***********************************************
 * create compressed list
 ***********************************************/

bool CalendarData::canAdd(CrEvent &event, QList<CrEvent> &list) {
    bool result = true;

    //check if department is the same
    if(list.count() >= 1) {
        if(list.first().department() != event.department()) {
            return false;
        }
    }

    //iterate over list
    QList<CrEvent>::iterator eventIterator;
    for(eventIterator = list.begin(); eventIterator != list.end(); ++eventIterator) {
        if( eventIterator->isOverlap(event) ) {
            result = false;
            break;
        }
    }
    return result;
}

void CalendarData::add(CrEvent &event, QList<QList<CrEvent> > &list) {

    QList<QList<CrEvent> >::iterator eventListIterator;

    if(list.count() == 0) {
        list.append(QList<CrEvent>());
        list.first().append(event);
        return;
    }

    for(eventListIterator = list.begin(); eventListIterator != list.end(); ++eventListIterator) {
        if(canAdd(event, eventListIterator.operator *())) {
            eventListIterator->append(event);
            break;
        }
    }

     if(eventListIterator == list.end()) {
         list.append(QList<CrEvent>());
         list.last().append(event);
     }
}




/**
 * @brief utriedi eventy s rovnakym titleId do zoznamu. Ak sa eventy prekrivaju, zaradi ten druhy do
 * noveho zoznamu
 * @return
 */
listOfListOfEvents CalendarData::createCompressedEventList(QList<CrEvent> &list) {

    listOfListOfEvents actualList;
    listOfListOfEvents finalList;

    if(list.isEmpty()) {
        return finalList;
    }


    //sort it
    sortList(list);
    //create compressed list


    int actualTitleId = list.first().titleId();

    QList<CrEvent>::iterator eventIterator;
    for(eventIterator = list.begin(); eventIterator != list.end(); ++eventIterator) {

        if(eventIterator->titleId() == actualTitleId) {

            add(eventIterator.operator *(), actualList);

            /*
             * riesi problem: ak je posledny event len 1x, tak sa prida do actualList,
             * ale do _compressedListOfEvents uz nie !!
             */
            if(eventIterator == --list.end()) {
                finalList.append(actualList);
            }
        } else {

            actualTitleId = eventIterator->titleId();
            finalList.append(actualList);
            actualList.clear();
            eventIterator--;

        }
    }

    return finalList;

}

/**
 * @brief utriedi workActivity do zoznamu. Ak sa workActivity prekrivaju, zaradi ten druhy do
 * noveho zoznamu
 * @return
 */
void CalendarData::createCompressedWorkActivityList(QList<CrEvent> &list) {


	qDebug() << "unsorted list:";
	for(int i=0; i < list.count(); i++) {
		qDebug("%20s %6d %10s  %10s",
			   list[i].title().toStdString().c_str(),
			   list[i].titleId(),
			   list[i].dateBegin().toString("dd.MMM").toStdString().c_str(),
			   list[i].dateEnd().toString("dd.MMM").toStdString().c_str());
	}

    //sort it
	sortList(list,true);

	qDebug() << "sorted list:";
	for(int i=0; i < list.count(); i++) {
		qDebug("%20s %6d %10s  %10s",
			   list[i].title().toStdString().c_str(),
			   list[i].titleId(),
			   list[i].dateBegin().toString("dd.MMM").toStdString().c_str(),
			   list[i].dateEnd().toString("dd.MMM").toStdString().c_str());
	}


    //create compressed list
    QList<CrEvent>::iterator eventIterator;
    for(eventIterator = list.begin(); eventIterator != list.end(); ++eventIterator) {
        add(eventIterator.operator *(), _listOfWorkAction);
    }

}

/***********************************************
 * sort
 ***********************************************/

void CalendarData::sortList(QList<CrEvent> &list, bool sortOnlyByDate) {
	if(sortOnlyByDate) {
		qSort(list.begin(), list.end(), CrEvent::lessThenDateBegin);
	} else {
		qSort(list.begin(), list.end(), CrEvent::lessThanCourseType);
	}
}


void CalendarData::sortCompressedList(listOfListOfEvents &list) {
    qSort(list.begin(), list.end(), CalendarData::LessThanDate);
}

bool CalendarData::LessThanDate(QList<CrEvent> &list1, QList<CrEvent> &list2) {
    if(list1.first().eventType() < list2.first().eventType()) {
        return true;
    }

    if(list1.first().eventType() == list2.first().eventType()) {
        return list1.first().dateBegin() < list2.first().dateBegin();
    }

    return false;
}

/***********************************************
 * print
 ***********************************************/

void CalendarData::printCompressedList() {

    QList<QList<CrEvent> >::iterator listOfListIterator;
    QList<CrEvent>::iterator eventIterator;

    for(listOfListIterator = _listOfEvents.begin(); listOfListIterator != _listOfEvents.end(); ++listOfListIterator) {
        for(eventIterator = listOfListIterator->begin(); eventIterator != listOfListIterator->end(); ++eventIterator) {
			qDebug("%20s %6d %10s  %10s", eventIterator->title().toStdString().c_str(),eventIterator->titleId(), eventIterator->dateBegin().toString("dd.MMM").toStdString().c_str(), eventIterator->dateEnd().toString("dd.MMM").toStdString().c_str());
        }
        qDebug("-next:");
    }
}

void CalendarData::printCompressedList(listOfListOfEvents &list) {
	listOfListOfEvents::iterator listOfListIterator;
	QList<CrEvent>::iterator eventIterator;

	for(listOfListIterator = list.begin(); listOfListIterator != list.end(); ++listOfListIterator) {
		for(eventIterator = listOfListIterator->begin(); eventIterator != listOfListIterator->end(); ++eventIterator) {
			qDebug("%20s %6d %10s  %10s", eventIterator->title().toStdString().c_str(),eventIterator->titleId(), eventIterator->dateBegin().toString("dd.MMM").toStdString().c_str(), eventIterator->dateEnd().toString("dd.MMM").toStdString().c_str());
		}
		qDebug("-next:");
	}
}

void CalendarData::printRoom(int id) {
    if(_trainingRooms.contains(id)) {
        TrainingRoom room = _trainingRooms.operator [](id);
		QList<QPair<QDate, int> > dateList =  room.occupiedDays();

        qDebug() << room.title();
		for(int i = 0; i < dateList.size(); i++) {
			qDebug() << dateList.at(i).first.toString("dd.MMM.yyyy");
        }
    } else {
        qDebug() << "ERROR: CrCalendar::printRoom() id=" << id << "not found";
    }

}

/***********************************************
 * get colors
 ***********************************************/

/**
 * @brief Vyhlada riadok z farbou
 * @param eventId
 * @param eventRow Najdene cislo riadku s farbou, -1 ak nenajde nic
 */
void CalendarData::getRowColor(int eventId, int &eventRow) {

	int offset = 0;
	getColorFromList(eventId, _listOfEvents, eventRow);
	if(eventRow < 0) {
		offset += _listOfEvents.size();
		getColorFromList(eventId, _listOfEventsAfterDeadline, eventRow);
		if(eventRow < 0) {
			offset += _listOfEventsAfterDeadline.size();
			getColorFromList(eventId, _listOfSoftskill, eventRow);
			if(eventRow < 0) {
				offset += _listOfSoftskill.size();
				getColorFromList(eventId, _listOfSoftskillAfterDeadline, eventRow);
			}
		}
	}
	if(eventRow > 0) {
		eventRow += offset;
	}

}

void CalendarData::getColorFromList(int eventId, listOfListOfEvents &list, int &eventRow) {

	for(int i = 0; i < list.size(); i++) {
		for(int j = 0; j < list.at(i).size(); j++) {
			if(list[i][j].eventId() == eventId) {
				eventRow = i;
				return;
			}
		}
	}
	eventRow = -1;
}
