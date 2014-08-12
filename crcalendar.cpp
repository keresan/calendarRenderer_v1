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
	_calendarData.sortCompressedList(_calendarData._listOfEventsAdded);

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
	QList<CrEvent> listEventsAdded;
	QList<CrEvent> listEventsCancelled;
    QList<CrEvent> listWorkAction;
	QList<CrEvent> listEventsSoftskill;
	QList<CrEvent> listEventsSoftskillAdded;
	QList<CrEvent> listEventsSoftskillCancelled;

    emit signalInfoMsg("zaciatok nacitania dat z xml suboru");

    for(int i = 0; i < eventList.count(); i++) {
        state = true;
        convertResult = true;
        QColor color;
		QColor colorInstructor, colorRoom;
        /*
         * treba error handling - ak vrateny QString je prazdny, tak doslo ku chybe
         * pripadne, ak sa string neda skonvertovat na int
         */


        QDomNode node = eventList.at(i);


		QString courseName =  loadTagValue(Settings::tagKurzNazov, &node);
        if(courseName.isEmpty()) {
            state = false;
        }

		QString courseIdStr =  loadTagValue(Settings::tagKurzId, &node);
        int courseId;
        if(courseIdStr.isEmpty()) {
            state = false;
        } else {
            courseId = courseIdStr.toInt(&convertResult);
            if(!convertResult) {
                state = false;
            }
        }

		QString durationDaysStr =  loadTagValue(Settings::tagDniTrvanie, &node);
        int durationDays;
        if(durationDaysStr.isEmpty()) {
            state = false;
        } else {
            durationDays = durationDaysStr.toInt(&convertResult);
            if(!convertResult) {
                state = false;
            }
        }

		QString durationHoursStr =  loadTagValue(Settings::tagHodTrvanie, &node);
        int durationHours;
        if(durationHoursStr.isEmpty()) {
        } else {
            durationHours = durationHoursStr.toInt(&convertResult);
            if(!convertResult) {
                state = false;
            }
        }


		//typ udalosti
		QString eventTypeStr = loadTagValue(Settings::tagTypSkolenia, &node);
        if(eventTypeStr.isEmpty()) {
            state = false;
        }

        CrEvent::EventType eventType;
		if(eventTypeStr.compare(Settings::tagOptPracAktivita,Qt::CaseInsensitive) == 0) {
            eventType = CrEvent::workAction;
		} else if(eventTypeStr.compare(Settings::tagOptSoftskill,Qt::CaseInsensitive) == 0) {
			eventType = CrEvent::softskill;
		}else {
            eventType = CrEvent::Course;
        }

		QString dateBeginStr =  loadTagValue(Settings::tagDatumZaciatok, &node);
        if(dateBeginStr.isEmpty()) {
            state = false;
        }
        QDate dateBegin = QDate::fromString(dateBeginStr, "yyyy-MM-ddT00:00:00");
        //v pripade chyby vrati 1.1.1900 !!!
        if (dateBegin == QDate(1900,1,1)) {
                state = false;
        }

		QString roomName =  loadTagValue(Settings::tagSkoliacaMiestnostNazov, &node);
        if(roomName.isEmpty() && eventType != CrEvent::workAction) {
            state = false;
        }

		QString roomIdStr =  loadTagValue(Settings::tagSkoliacaMiestnostId, &node);
        int roomId;
        if(roomIdStr.isEmpty() && eventType != CrEvent::workAction) {
            state = false;
        } else {
            roomId = roomIdStr.toInt(&convertResult);
            if(!convertResult) {
                state = false;
            }
        }

		QString instructorName =  loadTagValue(Settings::tagSkolitelNazov, &node);
		if(instructorName.isEmpty() && eventType != CrEvent::workAction) {
            state = false;
        }

		QString instructorIdStr =  loadTagValue(Settings::tagSkolitelId, &node);
		int instructorId;
		if(instructorIdStr.isEmpty() && eventType != CrEvent::workAction) {
            state = false;
        } else {
			instructorId = instructorIdStr.toInt(&convertResult);
            if(!convertResult) {
                state = false;
            }
        }

		QString activeDays = loadTagValue(Settings::tagAktivDen, &node);
        if(activeDays.isEmpty()) {
            state = false;

        }

		QString rgbColorStr = loadTagValue(Settings::tagFarbaSkolenia, &node);
        if(rgbColorStr.isEmpty()) {
            emit signalErrorMsg("udalost zacinajuca v "+dateBeginStr+" nema farbu ! bude pouzita #000000.");
        }
        //v pripade chyby sa nacita defaultna farba = #000000
        color.setNamedColor(rgbColorStr);

		QString department = loadTagValue(Settings::tagUtvarCislo, &node);
        if(department.isEmpty() && eventType != CrEvent::workAction) {
            state = false;
        }

		QString deadlineStr = loadTagValue(Settings::tagZamokStav, &node);
        //bool afterDeadline = false;
        CalendarData::Deadline deadline = CalendarData::before;
        if(!deadlineStr.isEmpty()) {
			if(deadlineStr.compare(Settings::tagOptPridaneSkolenie,Qt::CaseInsensitive) == 0) {
				//pridane po uzamknuti
				deadline = CalendarData::added;
			} else if(deadlineStr.compare(Settings::tagOptZruseneSkolenie,Qt::CaseInsensitive) == 0) {
				//odstanene po uzamknuti
				deadline = CalendarData::cancelled;
			}
        }

		QString courseOptionalText = loadTagValue(Settings::tagKurzVolitelnyText, &node);

        //error msg
        if(!state) {
            QString msg;
            msg += "==chyba v zazname:<br>";
            msg += "kurz_nazov= \""+courseName+"\"<br>";
            msg += "kurz_id= \""+courseIdStr+"\"<br>";
			msg += Settings::tagKurzVolitelnyText +"= "+courseOptionalText+"\"<br>";
            msg += "utvar= \""+department+"\"<br>";
            msg += "dni_trvanie= \""+durationDaysStr+"\"<br>";
            msg += "hod_trvanie= \""+durationHoursStr+"\"<br>";
            msg += "typ_skolenia= \""+eventTypeStr+"\"<br>";
            msg += "datum_zaciatok= \""+dateBeginStr+"\"<br>";
            msg += "skoliaca_miestnost_nazov= \""+roomName+"\"<br>";
            msg += "skoliaca_miestnost_id= \""+roomIdStr+"\"<br>";
			msg += "skolitel_nazov= \""+instructorName+"\"<br>";
			msg += "skolitel_id= \""+instructorIdStr+"\"<br>";
            msg += "aktiv_den= \""+activeDays+"\"<br>";
            msg += "=> zaznam bude vynechany !!<br>";
            msg += "----------------";
            emit signalErrorMsg(msg);
            //skip this event
            errCounter++;
            continue;
        }
        counter++;


        QList<CrEvent> *list;

        if(eventType == CrEvent::workAction) {
			//pracovna udalost
            list = &listWorkAction;
        } else {
			//obycajne udalosti
			if(eventType == CrEvent::Course && Settings::generateEvent) {
				if(deadline == CalendarData::added) {
					// udalost s priznakom, ze bola pridana az po uzamknuti planu
					list = &listEventsAdded;
				} else if(deadline == CalendarData::cancelled) {
					// udalost s priznakom, ze bola zrusena az po uzamknuti planu
					list = &listEventsCancelled;
				} else {
					list = &listEvents;
				}
			//soft skills
			} else if(eventType == CrEvent::softskill && Settings::generateSoftskill) {
				if(deadline == CalendarData::added) {
					// softskill s priznakom, ze bola pridana az po uzamknuti planu
					list = &listEventsSoftskillAdded;
				} else if(deadline == CalendarData::cancelled) {
					// softskill s priznakom, ze bola zrusena az po uzamknuti planu
					list = &listEventsSoftskillCancelled;
				}else {
					list = &listEventsSoftskill;
				}
			} else {
				continue;
			}
        }

        //add event to list
		list->append(CrEvent(courseName,courseId, dateBegin, durationDays, durationHours, color,courseOptionalText, department, uniqeId));
        list->last().addActiveDay(activeDays);
        list->last().calculateDateEnd();
        list->last().setEventType(eventType);

		//add room and instructor staffs
        if(eventType != CrEvent::workAction) {

            //room staffs
            if( _calendarData._trainingRooms.contains(roomId)) {
				_calendarData._trainingRooms.operator [](roomId).addDay(list->last().getEventDays(), uniqeId);
            } else {
                TrainingRoom newRoom(roomName, roomId, colorRoom);
				newRoom.addDay(list->last().getEventDays(),uniqeId);
                _calendarData._trainingRooms.insert(roomId, newRoom);
            }

			//instructor staffs
			if( _calendarData._trainingInstructor.contains(instructorId)) {
			   _calendarData._trainingInstructor.operator [](instructorId).addDay(list->last().getEventDays(), uniqeId);
            } else {
				TrainingRoom newInstructor(instructorName, instructorId, colorInstructor);
				newInstructor.addDay(list->last().getEventDays(), uniqeId);
			   _calendarData._trainingInstructor.insert(instructorId, newInstructor);
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
	_calendarData._listOfEventsAdded = CalendarData::createCompressedEventList(listEventsAdded);
	_calendarData._listOfEventsCancelled = CalendarData::createCompressedEventList(listEventsCancelled);

	_calendarData._listOfSoftskill = CalendarData::createCompressedEventList(listEventsSoftskill);
	_calendarData._listOfSoftskillAdded = CalendarData::createCompressedEventList(listEventsSoftskillAdded);
	_calendarData._listOfSoftskillCancelled = CalendarData::createCompressedEventList(listEventsSoftskillCancelled);

	_calendarData.createCompressedWorkActivityList(listWorkAction);

	//qDebug() << "list of work actions:";
	//_calendarData.printCompressedList(_calendarData._listOfWorkAction);

    return result;

}


/***********************************************
 * generate calendar
 ***********************************************/
void CrCalendar::generateCalendar() {

    _html.clear();
    _html += "<!DOCTYPE html>";
    _html += "<html>";
    _html += "<head>";

    _html += "<style type=\"text/css\">\n";
    _html += getListOfClassAndColors();
    _html += getListOfClassAndColorsOfWorkActivity();
    _html += css();
    _html += "</style>\n";

    _html += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">";

    _html += "<title>";
	_html += Settings::calendarTitleLabel;
    _html += "</title>";

    _html += "</head>";
    _html += "<body>";
    _html += "<table>";

    _html += "<thead>";

    //add title
    _html += "<tr>";
    _html += "<td colspan=\""+ QString::number(_dateFirst.daysTo(_dateLast)+5) +"\"";
    _html += "class=\""+ Cell::getCssClassAsString(Cell::calendar_title) +"\">";
	_html += Settings::calendarTitleLabel;
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

	//add work activity
	_html += generateWorkActivity();

	 _eventCounter = 1;
	//add events
	if(Settings::generateEvent) {
		_html += generateEvents(_calendarData._listOfEvents, Settings::eventLabel, true);

		QString kpiStr = getKPI(_calendarData._listOfEvents,
								_calendarData._listOfEventsAdded,
								_calendarData._listOfEventsCancelled);

		_html += generateEventsAfterDeadline(_calendarData._listOfEventsAdded,
											 _calendarData._listOfEventsCancelled,
											 Settings::eventAfterDeadlineLabel,
											 kpiStr);
	}
	//add soft skills
	if(Settings::generateSoftskill) {
		bool showHeadTitle = false;
		if(!Settings::generateEvent) {
			showHeadTitle = true;
		}
		_html += generateEvents(_calendarData._listOfSoftskill, Settings::softSkillLabel, showHeadTitle);

		QString kpiStr = getKPI(_calendarData._listOfSoftskill,
								_calendarData._listOfSoftskillAdded,
								_calendarData._listOfSoftskillCancelled);

		_html += generateEventsAfterDeadline(_calendarData._listOfSoftskillAdded,
											 _calendarData._listOfSoftskillCancelled,
											 Settings::softSkillAfterDeadlineLabel,
											 kpiStr);
	}

	if(Settings::generateRoom) {
        //add rooms
        _html += generateRooms(CalendarData::room);
    }

	if(Settings::generateInstructor) {
		//add instructors
		_html += generateRooms(CalendarData::instructor);
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

QString CrCalendar::generateEvents(listOfListOfEvents &list, QString title, bool showHeadTitle) {

    QString str;

	if(showHeadTitle) {
	}
    str += "<tr>";
	//serial number
	str += "<td class=\""+Cell::getCssClassAsString(Cell::group_title)+"\">";
	if(showHeadTitle) {
		str += "#";
	}
	str += "</td>";

    //department
	str += "<td class=\""+Cell::getCssClassAsString(Cell::group_title)+"\">";
	if(showHeadTitle) {
		str += Settings::departmentLabel;
	}
	str += "</td>";


	//event title
	str += "<td class=\""+Cell::getCssClassAsString(Cell::group_title)+"\">" +title+ "</td>";
	str += "<td></td>";
	str += "<td colspan=\" "+ QString::number(_dateFirst.daysTo(_dateLast)+1) +"\"></td>";
    str += "</tr>";

	if(list.isEmpty()) {
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
		str += generateEventsFromList(list, false);
	}

    return str;

}
QString CrCalendar::generateEventsAfterDeadline(listOfListOfEvents &added, listOfListOfEvents &cancelled, QString title, QString kpi) {

    QString str;

    str += "<tr>";
	//kpi
	str += "<td colspan=\"2\">";
	//str += kpi;
	str += "</td>";


    //event title
	str += "<td class=\""+Cell::getCssClassAsString(Cell::group_title)+"\">" +title+ "</td>";
    //color
    str += "<td></td>";
    //line
	str += "<td colspan=\" "+ QString::number(_dateFirst.daysTo(_dateLast)+1) +"\">";
	str += kpi;
	str += "</td>";
    str += "</tr>";

	if(added.isEmpty()) {
        str += "<tr class=\"" +Cell::getCssClassAsString(Cell::tr_end)+ "\">";;
        //serial number
		str += "<td>&nbsp</td>";
        //department
		str += "<td>&nbsp</td>";
        //event title
		str += "<td>&nbsp</td>";
        //color
		str += "<td>&nbsp</td>";
        //line
        str += generateEmptyLine();
        str += "</tr>";
    } else {

		str += generateEventsFromList(added, false);
		str += generateEventsFromList(cancelled, true, true); //highlight last tr and striken cancelled events
    }
    return str;
}


QString CrCalendar::generateEventsFromList(listOfListOfEvents &list, bool highlightEndTr, bool StrikenTitle) {

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
		Cell titleCell;
		titleCell.addCssClass(Cell::event_title);
		if(StrikenTitle) {
			titleCell.addCssClass(Cell::canceled_event);
		}
		str += "<td><div ";
		str += titleCell.getCssClasses() +"\">";
        str += listOfListIterator->first().title();
        str += "</div></td>\n";

		//add color cell with optional text
		str += "<td class=\""+getClassNameOfRow(_eventCounter)+"\">&nbsp";
		str += listOfListIterator->first().optionalText();
		str += "&nbsp</td>";
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

QString CrCalendar::generateRooms(CalendarData::RoomOrInstructor what) {
    QString str;

    QString title;
    trainingRoomMap trainingMap;


    if(what == CalendarData::room) {
		title = Settings::roomLabel;
        trainingMap = _calendarData._trainingRooms;
	} else if(what == CalendarData::instructor) {
		title = Settings::instructorLabel;
		trainingMap = _calendarData._trainingInstructor;
	} else {
		throw std::runtime_error("generateRooms(): unknown RoomOrInstructor");
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

		 //add department + room title + color cell
		 str += "<td colspan=\"3\"><div "+ cellTitle.getCssClasses()+ ">";
         str += cellTitle.text();
         str += "</div></td>\n";

         //add rooms line
         for(QDate daysIterator = _dateFirst; daysIterator <= _dateLast; daysIterator = daysIterator.addDays(1)) {
             Cell cellBody;

             if(daysIterator.dayOfWeek() == Qt::Saturday
                 || daysIterator.dayOfWeek() == Qt::Sunday) {

                 cellBody.addCssClass(Cell::non_work_day);
             }

			 int row = -6666;
			 int eventId;

			 if(room->isOccupied(daysIterator)) {
				 cellBody.addCssClass(Cell::event_day);

				 //look up color of cell
				 eventId = room->eventId(daysIterator);
				 if(eventId < 0) {
					 qDebug() << "NEZNAME ID";
				 }
				_calendarData.getRowColor(eventId,row);

             }
             str += "<td ";
             if(cellBody.isClass(Cell::event_day)) {
				 //qDebug() << cellTitle.text() << "eventId:"<< eventId << ", row:" << row+1 ;
				 str += cellBody.getCssClasses(getClassNameOfRow(row+1));
				 //str += cellBody.getCssClasses(getClassNameOfRow(row,what));
             } else {
                 str += cellBody.getCssClasses();
             }
			 str += " >";

			 /*
			 if(cellBody.isClass(Cell::event_day)) {
				str += "x";
			 }
			*/
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
 * @brief zisti jednotlive farby riadkov a vrati zoznam css tried s nastavenou touto farbou
 * @return
 */
QString CrCalendar::getListOfClassAndColorsFromList(listOfListOfEvents &list, int &counter) {
    QString returnStr;


    listOfListOfEvents::iterator listOfListIterator;
    for(listOfListIterator = list.begin(); listOfListIterator != list.end(); ++listOfListIterator) {

        //get hex color
		QString colorName = listOfListIterator->first().colorDay().name();

        //get class name
		QString className = getClassNameOfRow(counter);

        //put it together
       // vzor: .class_row_1 {background-color: #FF00FF;}
        returnStr += "."+className+" { background-color: "+colorName+"; }\n";

		counter++;
    }

    return returnStr;
}

QString CrCalendar::getListOfClassAndColors() {
    QString str;
	int counter = 1;
	str += getListOfClassAndColorsFromList(_calendarData._listOfEvents, counter);
	str += getListOfClassAndColorsFromList(_calendarData._listOfEventsAdded,counter);
	str += getListOfClassAndColorsFromList(_calendarData._listOfEventsCancelled,counter);
	str += getListOfClassAndColorsFromList(_calendarData._listOfSoftskill, counter);
	str += getListOfClassAndColorsFromList(_calendarData._listOfSoftskillAdded,counter);
	str += getListOfClassAndColorsFromList(_calendarData._listOfSoftskillCancelled,counter);

	return str;
}

QString CrCalendar::getListOfClassAndColorsOfWorkActivity() {

    QString returnStr;

    listOfListOfEvents::iterator listOfListIterator;
    for(listOfListIterator = _calendarData._listOfWorkAction.begin(); listOfListIterator != _calendarData._listOfWorkAction.end(); ++listOfListIterator) {

        listOfEvents::iterator eventIterator;

        for(eventIterator = listOfListIterator->begin(); eventIterator != listOfListIterator->end(); ++eventIterator) {
            //get color
			QString colorName = eventIterator->colorDay().name();
            //get class
            QString className = eventIterator->getClassNameOfId();

            // vzor: .class_id_1 {background-color: #FF00FF;}
            returnStr += "."+className+" { background-color: "+colorName+"; }\n";
        }
    }
    return returnStr;
}

QString CrCalendar::getKPI(listOfListOfEvents &beforeLock, listOfListOfEvents &afterLockAdded, listOfListOfEvents &afterLockCancelled) {


	//int numerator = afterLock.size();
	//int denominator =  beforeLock.size() + afterLock.size();
	int numerator = CalendarData::getEventsCount(beforeLock);
	int denominator =  numerator + CalendarData::getEventsCount(afterLockAdded) + CalendarData::getEventsCount(afterLockCancelled);

	float kpi;
	if(denominator == 0) {
		kpi = 0;
	} else {
		kpi = numerator / (double)denominator;
		kpi = kpi * 100;
	}

	QString kpiStr = QString::number(kpi,'f',1);
	kpiStr = kpiStr.replace('.',',');
	kpiStr = "KPI = " + kpiStr + "%";
	kpiStr += " (" + QString::number(numerator) + "/" + QString::number(denominator) + ")";

	return kpiStr;
}







