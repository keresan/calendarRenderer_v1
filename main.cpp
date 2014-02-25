#include "mainwindow.h"
#include <QApplication>


/*
 * nejasnosti:
 * 1) format zadania casu udalosti : datum zaciatku a konca alebo datum zaciatku a trvanie v dnoch
 * problem: existuju aj specialne skolenia, ktore mozu byt cez vikendy => treba pridat flag,
 * ktorym sa odlisia od normalnych udalosti, ktore vikend preskakuju
 *
 *
 *
 *
 *
 */



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();




    return a.exec();
}
