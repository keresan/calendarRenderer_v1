#include "cell.h"

Cell::Cell() {

}

void Cell::addCssClass(cssClassName name) {
    _class.append(name);

}

QString Cell::getCssClassAsString(cssClassName name) {
    QString str;
    switch(name) {
        case Cell::non_work_day: str.append("non_work_day"); break;
        case Cell::event_day: str.append("event_day"); break;
        case Cell::event_title: str.append("event_title"); break;
        case Cell::day_number: str.append("day_number"); break;
        case Cell::room_day: str.append("room_day"); break;
        case Cell::room_title: str.append("room_title"); break;
        case Cell::month_title: str.append("month_title"); break;
        case Cell::calendar_title: str.append("calendar_title"); break;
        case Cell::tr_highlight: str.append("tr_highlight"); break;
        case Cell::tr_end: str.append("tr_end"); break;
        case Cell::group_title: str.append("group_title"); break;
        case Cell::workActivity_cell: str.append("workActivity_cell"); break;
        case Cell::event_serial_number: str.append("serial_number"); break;
        case Cell::department_number: str.append("department_number"); break;
        case Cell::canceled_event: str.append("canceled_event"); break;


    default:
        assert(false);
    }
    return str;
}

QString Cell::getCssClasses() {
    QString str;
    str += " class=\"";
    str += getCssClassesList();
    str += "\" ";
    return str;
}

/**
 * @brief vrati string vsetkych tried + prida k nim stringClass
 * @param stringClass
 * @return
 */
QString Cell::getCssClasses(QString stringClass) {
    QString str;
    str += " class=\"";
    str += getCssClassesList();
    str += stringClass + " ";
    str += "\" ";
    return str;
}

/**
 * @brief vrati zoznam tried oddelenych medzerou
 * @return
 */
QString Cell::getCssClassesList() {
    QString str;


    QList<cssClassName>::iterator iterator = _class.begin();

    for(iterator = _class.begin(); iterator != _class.end(); ++iterator) {
        str.append(this->getCssClassAsString(iterator.operator *()));
        str.append(" ");
    }

    return str;
}

/**
 * @brief return true, if name is in _class
 * @param name
 * @return
 */
bool Cell::isClass(Cell::cssClassName name) {
    QList<cssClassName>::iterator iterator = _class.begin();

    for(iterator = _class.begin(); iterator != _class.end(); ++iterator) {
        if(iterator.operator *() == name  ) {

            return true;
        }
    }
    return false;
}

void Cell::addText(QString text) {
    _text.append(text);
}

QString Cell::text() {return _text;}
