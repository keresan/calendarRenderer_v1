#include "crcalendar.h"



CrCalendar::CrCalendar(QDate first, QDate last, QObject *parent) :
    QObject(parent) {
    _dateFirst = first;
    _dateLast = last;

    //loadMonthNames();
}

QString CrCalendar::getHtml() { return _html; }
QString CrCalendar::css() {
    return _css;
}
void CrCalendar::setCss(QString css) {
    _css = css;
}

void CrCalendar::addCss(QString css) {
    _addedCss += css;
}


void CrCalendar::setDates(QDate first, QDate last) {
    _dateFirst = first;
    _dateLast = last;
}


void CrCalendar::loadDataFromXml(QString path) {

    readXmlFile(path);

    _calendarData.sortCompressedList(_calendarData._listOfEvents);
    _calendarData.sortCompressedList(_calendarData._listOfEventsAfterDeadline);

}

/***********************************************
 * load data
 ***********************************************/
QString CrCalendar::loadTagValue(QString tagName, QDomNode *parent) {

    QString tagValue;
    QDomElement e = parent->toElement();

     if(!e.isNull()) {
         QDomNodeList tag = e.elementsByTagName(tagName);
         if(tag.count() == 1) {
             QDomElement eTag = tag.at(0).toElement();
             if(!eTag.isNull()) {

                 tagValue = eTag.text();
             }
         }
     }

    return tagValue;

}

/**
 * @brief CalendarData::loadDataFromXmlFile
 * @param path
 * @return ci dopadlo vsetko okej
 *
 */
bool CrCalendar::readXmlFile(QString path) {
    bool result;
    bool state,convertResult;

    int uniqeId = 1;
    int counter = 1;
    int errCounter = 0;
    QFile file(path);
    result = file.open(QIODevice::ReadOnly | QIODevice::Text);

    if(!result){

        emit signalErrorMsg("subor sa nepodarilo otvorit");
        return false;
    }

    emit signalInfoMsg("otvoreny subor: "+path);

    QDomDocument mDocument;
    QDomElement  rootElement;
    result = mDocument.setContent(file.readAll());

    if(!result){

        return false;
    }

    rootElement = mDocument.documentElement();


    QDomNodeList eventList = rootElement.childNodes();
    QList<CrEvent> listEvents;
    QList<CrEvent> listEventsAfterDeadline;
    QList<CrEvent> listWorkAction;

    emit signalInfoMsg("zaciatok nacitania dat z xml suboru");

    for(int i = 0; i < eventList.count(); i++) {
        state = true;
        convertResult = true;
        QColor color;
        QColor colorCoach, colorRoom;
        /*
         * treba error handling - ak vrateny QString je prazdny, tak doslo ku chybe
         * pripadne, ak sa string neda skonvertovat na int
         */


        QDomNode node = eventList.at(i);


        QString courseName =  loadTagValue("kurz_nazov", &node);
        if(courseName.isEmpty()) {
            state = false;
        }

        QString courseIdStr =  loadTagValue("kurz_id", &node);
        int courseId;
        if(courseIdStr.isEmpty()) {
            state = false;
        } else {
            courseId = courseIdStr.toInt(&convertResult);
            if(!convertResult) {
                state = false;
            }
        }

        QString durationDaysStr =  loadTagValue("dni_trvanie", &node);
        int durationDays;
        if(durationDaysStr.isEmpty()) {
            state = false;
        } else {
            durationDays = durationDaysStr.toInt(&convertResult);
            if(!convertResult) {
                state = false;
            }
        }



        QString durationHoursStr =  loadTagValue("hod_trvanie", &node);
        int durationHours;
        if(durationHoursStr.isEmpty()) {
        } else {
            durationHours = durationHoursStr.toInt(&convertResult);
            if(!convertResult) {
                state = false;
            }
        }


        QString eventTypeStr = loadTagValue("typ_skolenia", &node);
        if(eventTypeStr.isEmpty()) {
            state = false;
        }

        CrEvent::EventType eventType;
        if(eventTypeStr.compare("prac_aktivita",Qt::CaseInsensitive) == 0) {
            eventType = CrEvent::workAction;
        } else {
            eventType = CrEvent::Course;
        }

        QString dateBeginStr =  loadTagValue("datum_zaciatok", &node);
        if(dateBeginStr.isEmpty()) {
            state = false;
        }
        QDate dateBegin = QDate::fromString(dateBeginStr, "yyyy-MM-ddT00:00:00");
        //v pripade chyby vrati 1.1.1900 !!!
        if (dateBegin == QDate(1900,1,1)) {
                state = false;
        }


        QString roomName =  loadTagValue("skoliaca_miestnost_nazov", &node);
        if(roomName.isEmpty() && eventType != CrEvent::workAction) {
            state = false;
        }

        QString roomIdStr =  loadTagValue("skoliaca_miestnost_id", &node);
        int roomId;
        if(roomIdStr.isEmpty() && eventType != CrEvent::workAction) {
            state = false;
        } else {
            roomId = roomIdStr.toInt(&convertResult);
            if(!convertResult) {
                state = false;
            }
        }

        QString coachName =  loadTagValue("skolitel_nazov", &node);
        if(coachName.isEmpty() && eventType != CrEvent::workAction) {

            state = false;
        }

        QString coachIdStr =  loadTagValue("skolitel_id", &node);
        int coachId;
        if(coachIdStr.isEmpty() && eventType != CrEvent::workAction) {
            state = false;
        } else {
            coachId = coachIdStr.toInt(&convertResult);
            if(!convertResult) {
                state = false;
            }
        }

        QString activeDays = loadTagValue("aktiv_den", &node);
        if(activeDays.isEmpty()) {
            state = false;

        }

        QString rgbColorStr = loadTagValue("farba_skolenia", &node);
        if(rgbColorStr.isEmpty()) {
            emit signalErrorMsg("udalost zacinajuca v "+dateBeginStr+" nema farbu ! bude pouzita #000000.");
        }
        //v pripade chyby sa nacita defaultna farba = #000000
        color.setNamedColor(rgbColorStr);

        QString department = loadTagValue("utvar_cislo", &node);
        if(department.isEmpty() && eventType != CrEvent::workAction) {
            state = false;
        }

        QString deadlineStr = loadTagValue("zamok_stav", &node);
        //bool afterDeadline = false;
        CalendarData::Deadline deadline = CalendarData::before;
        if(!deadlineStr.isEmpty()) {
            if(deadlineStr.compare("po_zamknuti",Qt::CaseInsensitive) == 0) {
                deadline = CalendarData::after;
            }
        }

        QString colorCoachStr = loadTagValue("farba_skolitela", &node);
        if(colorCoachStr.isEmpty()) {
            emit signalErrorMsg("udalost zacinajuca v "+dateBeginStr+" nema farbu skolitela ! bude pouzita #000000.");
        }
        //v pripade chyby sa nacita defaultna farba = #000000
        colorCoach.setNamedColor(colorCoachStr);

        QString colorRoomStr = loadTagValue("farba_miestnosti", &node);
        if(colorRoomStr.isEmpty()) {
            emit signalErrorMsg("udalost zacinajuca v "+dateBeginStr+" nema farbu miestnosti ! bude pouzita #000000.");
        }
        //v pripade chyby sa nacita defaultna farba = #000000
        colorRoom.setNamedColor(colorRoomStr);

        /*
        qDebug() << "courseName:" << courseName;
        qDebug() << "courseId:" << courseId;
        qDebug() << "durationDays:" << durationDays;
        qDebug() << "durationHours:" << durationHours;
        qDebug() << "dateBegin:" << dateBegin.toString("dd.MM.yyyy");
        qDebug() << "roomName:" << roomName;
        qDebug() << "roomId:" << roomId;
        qDebug() << "coachName:" << coachName;
        qDebug() << "coachId:" << coachId;

        qDebug() << "--------";
        */

        //error msg
        if(!state) {
            QString msg;
            msg += "==chyba v zazname:<br>";
            msg += "kurz_nazov= \""+courseName+"\"<br>";
            msg += "kurz_id= \""+courseIdStr+"\"<br>";
            msg += "utvar= \""+department+"\"<br>";
            msg += "dni_trvanie= \""+durationDaysStr+"\"<br>";
            msg += "hod_trvanie= \""+durationHoursStr+"\"<br>";
            msg += "typ_skolenia= \""+eventTypeStr+"\"<br>";
            msg += "datum_zaciatok= \""+dateBeginStr+"\"<br>";
            msg += "skoliaca_miestnost_nazov= \""+roomName+"\"<br>";
            msg += "skoliaca_miestnost_id= \""+roomIdStr+"\"<br>";
            msg += "skolitel_nazov= \""+coachName+"\"<br>";
            msg += "skolitel_id= \""+coachIdStr+"\"<br>";
            msg += "aktiv_den= \""+activeDays+"\"<br>";
            msg += "=> zaznam bude vynechany !!<br>";
            msg += "----------------";
            emit signalErrorMsg(msg);
            //skip this event
            errCounter++;
            continue;
        }
        counter++;

        /**
          * + active days
          */

        QList<CrEvent> *list;

        if(eventType == CrEvent::workAction) {
            list = &listWorkAction;
        } else {
            if(deadline == CalendarData::after) {
                list = &listEventsAfterDeadline;
            } else {
                list = &listEvents;
            }
        }

        //add event to list
        list->append(CrEvent(courseName,courseId, dateBegin, durationDays, roomId, durationHours, color, department, uniqeId));
        list->last().addActiveDay(activeDays);
        list->last().calculateDateEnd();
        list->last().setEventType(eventType);


        if(eventType != CrEvent::workAction) {

            //room staffs
            if( _calendarData._trainingRooms.contains(roomId)) {
                _calendarData._trainingRooms.operator [](roomId).addDay(list->last().getEventDays());
            } else {
                TrainingRoom newRoom(roomName, roomId, colorRoom);
                newRoom.addDay(list->last().getEventDays());
                _calendarData._trainingRooms.insert(roomId, newRoom);
            }

            //coach staffs
            if( _calendarData._trainingCoach.contains(coachId)) {
               _calendarData._trainingCoach.operator [](coachId).addDay(list->last().getEventDays());
            } else {
                TrainingRoom newCoach(coachName, coachId, colorCoach);
                newCoach.addDay(list->last().getEventDays());
               _calendarData._trainingCoach.insert(coachId, newCoach);
            }

        }
        uniqeId++;
    }

    emit signalInfoMsg("koniec nacitania dat z xml suboru");
    emit signalInfoMsg("nacitanych zaznamov: "+QString::number(counter));
    emit signalInfoMsg("preskocenych zaznamov: "+QString::number(errCounter));

    //create compressed list
    //sortList(list); //presunute do funkcie createCompresed...
    //sortList(listWorkAction);

    _calendarData._listOfEvents = CalendarData::createCompressedEventList(listEvents);
    _calendarData._listOfEventsAfterDeadline = CalendarData::createCompressedEventList(listEventsAfterDeadline);
    _calendarData.createCompressedWorkActivityList(listWorkAction);

	qDebug() << "list of work actions:";
	_calendarData.printCompressedList(_calendarData._listOfWorkAction);

    return result;

}


/***********************************************
 * generate calendar
 ***********************************************/


void CrCalendar::generateCalendar(QString title, bool genEvent, bool genRoom, bool genCoach) {

    _html.clear();
    _html += "<!DOCTYPE html>";
    _html += "<html>";
    _html += "<head>";

    _html += "<style type=\"text/css\">\n";
    _html += getListOfClassAndColors();
    _html += getListOfClassAndColors(CalendarData::room);
    _html += getListOfClassAndColors(CalendarData::coach);
    _html += getListOfClassAndColorsOfWorkActivity();
    _html += css();
    _html += "</style>\n";

    _html += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">";

    _html += "<title>";
    _html += title;
    _html += "</title>";

    _html += "</head>";
    _html += "<body>";
    _html += "<table>";

    _html += "<thead>";

    //add title
    _html += "<tr>";
    _html += "<td colspan=\""+ QString::number(_dateFirst.daysTo(_dateLast)+5) +"\"";
    _html += "class=\""+ Cell::getCssClassAsString(Cell::calendar_title) +"\">";
    _html += title;
    _html += "</td>";
    _html += "</tr>";


    //add month names
    _html += "<tr>";
    _html += "<td></td>";
    _html += "<td></td>";
    _html += "<td></td>";
    _html += "<td></td>";
    _html += generateMonthTitle();
    _html += "</tr>";


    //add day numbers
    _html += "<tr>";
    _html += "<td></td>";
    _html += "<td></td>";
    _html += "<td></td>";
    _html += "<td></td>";
    _html += generateDayNumber();
    _html += "</tr>";
    _html += "</thead>";


    _html += "<tbody>";

    if(genEvent) {
        //add work activity
        _html += generateWorkActivity();

        //add events
        _html += generateEvents();
        _html += generateEventsAfterDeadline();
    }

    if(genRoom) {
        //add rooms
        _html += generateRooms(CalendarData::room);
    }

    if(genCoach) {
        //add coaches
        _html += generateRooms(CalendarData::coach);
    }

    _html += "</tbody>";
    _html += "</table>\n";
    _html += "</body>\n";
    _html += "</html>\n";




}

QString CrCalendar::generateMonthTitle() {
    QString str;

    QDate daysIterator;
    QDate actualMonth = QDate(_dateFirst.year(),_dateFirst.month(),1);
    int daysCount = 0;

    if(_dateFirst.month() == _dateLast.month() && _dateFirst.year() == _dateLast.year()) {
        str += "<td colspan=\""+ QString::number(_dateFirst.daysTo(_dateLast.addDays(1)))+"\"";;
        str += " class=\""+ Cell::getCssClassAsString(Cell::month_title) +"\">";
        str += getMonthString(_dateFirst.month());
        str += " ";
        str += _dateFirst.toString("yyyy");
        str += "</td>\n";
    } else {
        for(daysIterator = _dateFirst; daysIterator.operator <=(_dateLast); daysIterator = daysIterator.addDays(1)) {

            if(daysIterator.operator ==(QDate(daysIterator.year(), daysIterator.month(),daysIterator.daysInMonth()))
                    || daysIterator.operator ==(_dateLast)) {

                str += "<td colspan=\""+ QString::number(++daysCount) +"\"";
                str += " class=\""+ Cell::getCssClassAsString(Cell::month_title) + "\">";
                str += getMonthString(actualMonth.month());
                str += " ";
                str += actualMonth.toString("yyyy");

                str += "</td>\n";

                daysCount = 0;
                actualMonth = actualMonth.addMonths(1);
            } else {
                daysCount++;
            }
        }
    }

    return str;
}

QString CrCalendar::generateDayNumber() {
    QString str;
    QDate daysIterator;

    for(daysIterator = _dateFirst; daysIterator.operator <=(_dateLast); daysIterator = daysIterator.addDays(1)) {
        Cell cell;
        cell.addCssClass(Cell::day_number);
        if(daysIterator.dayOfWeek() == Qt::Saturday
            || daysIterator.dayOfWeek() == Qt::Sunday) {
            //weekend => nothing
            ;
        } else {
            //add day number
            cell.addText(daysIterator.toString("dd"));
        }

        str += "<td><div ";
        str += cell.getCssClasses();
        str += ">";
        str += cell.text();
        str += "</div></td>\n";
    }

    return str;
}

/**
 * @brief vygeneruje pracovne aktivity. Nazov pracovnej aktivity je v bunkach.
 * Neberie do uvahy aktive dni => vsetky su aktivne
 * @return
 */
QString CrCalendar::generateWorkActivity() {

    QString returnStr;
    listOfListOfEvents::iterator listOfListIterator;

    for(listOfListIterator = _calendarData._listOfWorkAction.begin(); listOfListIterator != _calendarData._listOfWorkAction.end(); ++listOfListIterator) {
        returnStr += "<tr>";
        returnStr += "<td></td>";
        returnStr += "<td></td>";
        returnStr += "<td></td>";
        returnStr += "<td></td>";

        QDate dayIterator;
        QList<CrEvent>::iterator eventIterator = listOfListIterator->begin();
        //prva udalost

        int colspan = 0;
        for(dayIterator = _dateFirst; dayIterator <= _dateLast; dayIterator = dayIterator.addDays(1)) {

			//qDebug() << QString("%1: %2").arg(eventIterator->title()).arg(eventIterator->dateBegin().toString("dd.MM.yyyy"));

            if( eventIterator != listOfListIterator->end()
                    && dayIterator >= eventIterator->dateBegin()
                    && dayIterator <= eventIterator->dateEnd()) {
                //iteracia cez udalost
                colspan++;

                //posledny den udalosti
                if(dayIterator == eventIterator->dateEnd()
                        || dayIterator == _dateLast) {
                    returnStr += "<td colspan=\""+ QString::number(colspan) +"\"";
                    returnStr += " class=\""+ eventIterator->getClassNameOfId();
                    returnStr += " "+ Cell::getCssClassAsString(Cell::workActivity_cell)+ "\">";
                    returnStr += eventIterator->title();
                    returnStr += "</td>";

                    colspan = 0;
                    eventIterator++;
                }

            //prazdny den
            } else {
                Cell cell;
                if(dayIterator.dayOfWeek() == Qt::Saturday
                    || dayIterator.dayOfWeek() == Qt::Sunday) {

                    cell.addCssClass(Cell::non_work_day);
                }

                returnStr += "<td ";
                returnStr += cell.getCssClasses();
                returnStr += "></td>\n";
            }
        }
        returnStr += "</tr>";

    }

    return returnStr;


}

QString CrCalendar::generateEvents() {

    _eventCounter = 1;

    QString str;

    str += "<tr>";
    //serial numeber
    str += "<td class=\""+Cell::getCssClassAsString(Cell::group_title)+"\">#</td>";
    //department
    str += "<td class=\""+Cell::getCssClassAsString(Cell::group_title)+"\">Útvar</td>";
    //event title
    str += "<td colspan=\"2\" class=\""+Cell::getCssClassAsString(Cell::group_title)+"\">" + "Školenie" +"</td>";
    str += "<td colspan=\" "+ QString::number(_dateFirst.daysTo(_dateLast)+1) +"\"></td>";
    str += "</tr>";

    str += generateEventsFromList(_calendarData._listOfEvents, false);

    return str;

}

QString CrCalendar::generateEventsAfterDeadline() {

    QString str;

    str += "<tr>";
    //serial numeber
    str += "<td></td>";
    //department
    str += "<td></td>";

    //event title
    str += "<td class=\""+Cell::getCssClassAsString(Cell::group_title)+"\">Školenie doplnené po uzamknutí plánu</td>";
    //color
    str += "<td></td>";
    //line
    str += generateEmptyLine();
    str += "</tr>";

    if(_calendarData._listOfEventsAfterDeadline.isEmpty()) {
        str += "<tr class=\"" +Cell::getCssClassAsString(Cell::tr_end)+ "\">";;
        //serial number
        str += "<td></td>";
        //department
        str += "<td></td>";
        //event title
        str += "<td></td>";
        //color
        str += "<td></td>";
        //line
        str += generateEmptyLine();
        str += "</tr>";
    } else {

        str += generateEventsFromList(_calendarData._listOfEventsAfterDeadline, true);
    }



    return str;
}


QString CrCalendar::generateEventsFromList(listOfListOfEvents &list, bool highlightEndTr) {

    QString str;
    listOfListOfEvents::iterator listOfListIterator;



    for(listOfListIterator = list.begin(); listOfListIterator != list.end(); ++listOfListIterator) {

        Cell cell;

        str += "<tr ";
        if(_eventCounter % 3 == 0) {
            cell.addCssClass(Cell::tr_highlight);
        }

        if(listOfListIterator == --list.end()) {
            if(highlightEndTr) {
                cell.addCssClass(Cell::tr_end);
            } else if( ! cell.isClass(Cell::tr_highlight)) {
                cell.addCssClass(Cell::tr_highlight);
            }
        }

        str += cell.getCssClasses();
        str += ">";

        //add serial number
        str += "<td class=\""+Cell::getCssClassAsString(Cell::event_serial_number)+"\">"+ QString::number(_eventCounter) +"</td>";

        //add department
        str += "<td class=\""+Cell::getCssClassAsString(Cell::department_number)+"\">"+ listOfListIterator->first().department() +"</td>";


        //add event title
        str += "<td><div class=\""+Cell::getCssClassAsString(Cell::event_title)+"\">";
        str += listOfListIterator->first().title();
        str += "</div></td>\n";

        //add color cell
        str += "<td class=\""+getClassNameOfRow(_eventCounter)+"\">&nbsp&nbsp</td>";

        //add event line
        str += generateEventLine(listOfListIterator.operator *(), getClassNameOfRow(_eventCounter));

        str += "</tr>\n";

        _eventCounter++;
    }

    return str;
}

/**
 * @brief Vygeneruje vsetky dni pre 1 riadok kalendara pre danu udalost.
 * Ak ma skolenie len 1 den a trva menej ako 8h, vypise pocet hodin
 * @param list
 * @param className - nazov triedy, ktora sa prida k vsetkym polickam udalosti (class_row_#)
 * @return
 */
QString CrCalendar::generateEventLine(QList<CrEvent> &list, QString className) {
    QString returnStr;
    QDate dayIterator;
    QList<CrEvent>::iterator eventIterator = list.begin();



    for(dayIterator = _dateFirst; dayIterator <= _dateLast; dayIterator = dayIterator.addDays(1)) {
        Cell cell;

        if(dayIterator.dayOfWeek() == Qt::Saturday
            || dayIterator.dayOfWeek() == Qt::Sunday) {

            cell.addCssClass(Cell::non_work_day);
        }

        if( eventIterator != list.end()
                && eventIterator->dateBegin() <= dayIterator
                && eventIterator->dateEnd() >= dayIterator
                && eventIterator->testActiveDay( eventIterator->convertFromQtDayOfWeek((Qt::DayOfWeek)dayIterator.dayOfWeek()))) {

            cell.addCssClass(Cell::event_day);

            //event continue, but calendar ends => insert ">"
            if(dayIterator == _dateLast && eventIterator->dateEnd() > _dateLast) {
                cell.addText(">");
            }

            //last day of event => hop to next
            if(eventIterator->dateEnd()  == dayIterator) {
                //check, if event has only 1 day
                if(eventIterator->dateBegin() == eventIterator->dateEnd()
                        && eventIterator->hours() < 8) {
                    cell.addText(QString::number(eventIterator->hours()));
                }

                //next event
                eventIterator++;
            }
        }
        returnStr += "<td ";
        if(cell.isClass(Cell::event_day)) {
            returnStr += cell.getCssClasses(className);
        } else {
            returnStr += cell.getCssClasses();
        }
        returnStr += ">";
        returnStr += cell.text();
        returnStr += "</td>\n";

    }

    return returnStr;
}

QString CrCalendar::generateRooms(CalendarData::RoomOrCoach what) {
    QString str;

    QString title;
    trainingRoomMap trainingMap;
    if(what == CalendarData::room) {
        title = "Školiaca miestnosť";
        trainingMap = _calendarData._trainingRooms;
    } else if(what == CalendarData::coach) {
        title = "Inštruktor";
        trainingMap = _calendarData._trainingCoach;
    }

    str += "<tr>";
    //str += "<td></td><td></td>";
    str += "<td colspan=\"4\" class=\""+Cell::getCssClassAsString(Cell::group_title)+"\">" + title +"</td>";
    //str += "<td> <div class=\""+Cell::getCssClassAsString(Cell::group_title)+"\">" + title +"</div></td>";
    //str += "<td></td>";

    str += "<td colspan=\" "+ QString::number(_dateFirst.daysTo(_dateLast)+1) +"\"></td>";

    str += "</tr>";


    int counter = 1;

    trainingRoomMap::iterator room = trainingMap.begin();
    while (room != trainingMap.end() ) {

        Cell cellTitle;
        Cell cell;
        cellTitle.addCssClass(Cell::room_title);
        cellTitle.addText(room->title());
        str += "<tr";

        if(counter % 3 == 0) {
            cell.addCssClass(Cell::tr_highlight);
        }

        if(room == --trainingMap.end()) {
            cell.addCssClass(Cell::tr_end);

        }
        str += cell.getCssClasses();
         str += ">";

        //add serial number
         str += "<td class=\""+Cell::getCssClassAsString(Cell::event_serial_number)+"\">"+ QString::number(counter) +"</td>";
         //add department
         str += "<td></td>\n";

         //add room title
         str += "<td><div "+ cellTitle.getCssClasses()+ ">";
         str += cellTitle.text();
         str += "</div></td>\n";

         //add color cell
         str += "<td class=\""+getClassNameOfRow(counter,what)+"\">&nbsp&nbsp</td>\n";

         //add rooms line
         for(QDate daysIterator = _dateFirst; daysIterator <= _dateLast; daysIterator = daysIterator.addDays(1)) {
             Cell cellBody;

             if(daysIterator.dayOfWeek() == Qt::Saturday
                 || daysIterator.dayOfWeek() == Qt::Sunday) {

                 cellBody.addCssClass(Cell::non_work_day);
             }

             if(room->isOccupied(daysIterator)) {
                 cellBody.addCssClass(Cell::event_day);
             }

             str += "<td ";

             if(cellBody.isClass(Cell::event_day)) {
                 str += cellBody.getCssClasses(getClassNameOfRow(counter,what));
             } else {
                 str += cellBody.getCssClasses();
             }
             str += " >";
             str += "</td>\n";
         }
         str += "</tr>\n";


        counter++;
        room++;
    }

    return str;
}


QString CrCalendar::generateEmptyLine() {
    QString str;
    QDate dayIterator;
    for(dayIterator = _dateFirst; dayIterator <= _dateLast; dayIterator = dayIterator.addDays(1))

        if(dayIterator.dayOfWeek() == Qt::Saturday
            || dayIterator.dayOfWeek() == Qt::Sunday) {

            str += "<td class=\"" +Cell::getCssClassAsString(Cell::non_work_day)+ "\"></td>";
        } else {
            str += "<td>&nbsp</td>";
        }
    return str;
}

void CrCalendar::loadMonthNames() {
    QFile file(":/mesiace.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    while (!file.atEnd()) {
        _monthNames.append(file.readLine());
    }
    file.close();

}

QString CrCalendar::getMonthString(int monthNumber) {
    QString returnStr;
    /*
    if(monthNumber >= 1 && monthNumber <= 12) {
        returnStr = _monthNames.at(monthNumber-1);
    }
    */
    switch(monthNumber) {
    case 1: returnStr= "január"; break;
    case 2: returnStr= "február"; break;
    case 3: returnStr= "marec"; break;
    case 4: returnStr= "apríl"; break;
    case 5: returnStr= "máj"; break;
    case 6: returnStr= "jún"; break;
    case 7: returnStr= "júl"; break;
    case 8: returnStr= "august"; break;
    case 9: returnStr= "september"; break;
    case 10: returnStr= "október"; break;
    case 11: returnStr= "november"; break;
    case 12: returnStr= "december"; break;


    }

    return returnStr;
}

/**
 * @brief podla indexu vrati nazov triedy v tvare class_row_#
 * @param index - poradove cislo v _compressedListOfEvents
 * @return
 */
QString CrCalendar::getClassNameOfRow(int index) {
    return "class_row_"+QString::number(index);
}

/**
 * @brief podla indexu vrati nazov triedy v tvare class_WHAT_row_#
 * @param index - poradove cislo v _compressedListOfEvents
 * @param what = room alebo coach
 * @return
 */
QString CrCalendar::getClassNameOfRow(int index, CalendarData::RoomOrCoach what) {

    if(what == CalendarData::room) {
        return "class_room_row_"+QString::number(index);
    } else if(what == CalendarData::coach) {
        return "class_coach_row_"+QString::number(index);
    } else {
        assert(false);
    }

}

/**
 * @brief zisti jednotlive farby riadkov a vrati zoznam css tried s nastavenou touto farbou
 * @return
 */
QString CrCalendar::getListOfClassAndColorsFromList(listOfListOfEvents &list) {
    QString returnStr;


    listOfListOfEvents::iterator listOfListIterator;
    for(listOfListIterator = list.begin(); listOfListIterator != list.end(); ++listOfListIterator) {

        //get hex color
        QString colorName = listOfListIterator->first().color().name();

        //get class name
        QString className = getClassNameOfRow(_eventCounter);

        //put it together
       // vzor: .class_row_1 {background-color: #FF00FF;}
        returnStr += "."+className+" { background-color: "+colorName+"; }\n";

        _eventCounter++;
    }

    return returnStr;
}

QString CrCalendar::getListOfClassAndColors() {
    QString str;
    _eventCounter = 1;
    str += getListOfClassAndColorsFromList(_calendarData._listOfEvents);
    str += getListOfClassAndColorsFromList(_calendarData._listOfEventsAfterDeadline);

    return str;

}

QString CrCalendar::getListOfClassAndColors(CalendarData::RoomOrCoach what) {
    QString returnStr;
    int index = 1;


    trainingRoomMap trainingSet;
    if(what == CalendarData::room) {
        trainingSet = _calendarData._trainingRooms;
    } else if(what == CalendarData::coach) {
        trainingSet = _calendarData._trainingCoach;
    } else {
        assert(false);
    }

    trainingRoomMap::iterator room = trainingSet.begin();

    for(room = trainingSet.begin(); room != trainingSet.end(); ++room) {
        //get hex color
        QString colorName = room->color().name();

        //get class name
        QString className = getClassNameOfRow(index, what);

        //put it together
       // vzor: .class_WHAT_row_1 {background-color: #FF00FF;}
        returnStr += "."+className+" { background-color: "+colorName+"; }\n";

        index ++;
    }

    return returnStr;
}

QString CrCalendar::getListOfClassAndColorsOfWorkActivity() {

    QString returnStr;

    listOfListOfEvents::iterator listOfListIterator;
    for(listOfListIterator = _calendarData._listOfWorkAction.begin(); listOfListIterator != _calendarData._listOfWorkAction.end(); ++listOfListIterator) {

        listOfEvents::iterator eventIterator;

        for(eventIterator = listOfListIterator->begin(); eventIterator != listOfListIterator->end(); ++eventIterator) {
            //get color
            QString colorName = eventIterator->color().name();
            //get class
            QString className = eventIterator->getClassNameOfId();

            // vzor: .class_id_1 {background-color: #FF00FF;}
            returnStr += "."+className+" { background-color: "+colorName+"; }\n";
        }
    }
    return returnStr;
}









