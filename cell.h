#ifndef CELL_H
#define CELL_H

#include <QList>
#include <QString>

#include <assert.h>

class Cell {
public:
    Cell();

    enum cssClassName{
        non_work_day,
        event_day,
        room_day,
        event_title,
        room_title,
        day_number,
        month_title,
        calendar_title,
        tr_highlight,
        tr_end,
        group_title,
        workActivity_cell,
        event_serial_number,
        department_number
    };

    void addCssClass(Cell::cssClassName name);

    static QString getCssClassAsString(Cell::cssClassName name);
    QString getCssClasses();
    QString getCssClasses(QString stringClass);
    QString getCssClassesList();

    bool isClass(Cell::cssClassName name);

    void addText(QString text);
    QString text();



private:
    QString _text;
    QList<cssClassName> _class;


};

#endif // CELL_H
