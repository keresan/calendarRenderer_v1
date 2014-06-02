#include "crtablewidget.h"


CrTableWidget::CrTableWidget(QStringList &captions, QWidget *parent): QTableWidget(parent) {


    this->setColumnCount(captions.count());
    this->setRowCount(1);

    this->setHorizontalHeaderLabels(captions);
    this->horizontalHeader()->setStretchLastSection(true);
    //this->verticalHeader()->setVisible(false);

    //connect(this, SIGNAL(cellClicked(int,int)), this, SLOT(activateItemHandler(int,int)));
    //connect(this, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(itemChangedHandler(int,int,int,int)));

    this->setFont(QFont("Verdana"));



}

/*
void CrTableWidget::activateItemHandler(int rows, int column) {
    qDebug() << "item in row" << rows << "is active";


}

void CrTableWidget::itemChangedHandler(int rows, int columns, int, int) {

    //if(columns == 0) {
    this->setCurrentCell(rows, 1);
    //}

    qDebug() << "row col" << rows << columns;
}
*/
