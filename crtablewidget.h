#ifndef CRTABLEWIDGET_H
#define CRTABLEWIDGET_H

#include <QTableWidget>
#include <QList>
#include <QHeaderView>
#include <QDebug>


class CrTableWidget : public QTableWidget {
     Q_OBJECT
public:
    explicit CrTableWidget(QStringList &captions, QWidget *parent = 0);

private:

public slots:
    //void activateItemHandler(int rows, int column);

    //void itemChangedHandler(int rows, int columns, int, int);

};

#endif // CRTABLEWIDGET_H
