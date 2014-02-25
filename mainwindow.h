#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "crcalendar.h"
#include "logwindow.h"
#include "customtabwidget.h"
#include "colordialog.h"

#include <QMainWindow>
#include <QFileDialog>

#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintEngine>
#include <QtPrintSupport/QtPrintSupport>

#include <QTextDocument>
#include <QPainter>

#include <QPushButton>
#include <QVBoxLayout>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QGridLayout>
#include <QDoubleSpinBox>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString loadFileAsString(QString path);

    QString getTitle();

    enum Quartals {
        Q1c, //current year
        Q2c,
        Q3c,
        Q4c,
        Q1n, //next year
        Q2n,
        Q3n,
        Q4n
    };

private:
    Ui::MainWindow *ui;
    CrCalendar *_calendar;

    LogWindow *_logConsole;


    QLabel *_labTitle;
    QWidget *_optionMenu;
    QDockWidget *_dockWidget;

    QGridLayout *_gridLayout, *_gridLayoutOption, *_gridLayoutTitle, *_gridLayoutDate, *_gridLayoutWidth;

    QLabel *_labQuart;

    QPushButton *_btnOpenFile, *_btnSaveFile, *_btnError, *_btnColor;
    QGroupBox *_boxExport, *_boxTitle, *_boxDate, *_boxWidth;
    QCheckBox *_checkEvent, *_checkRoom, *_checkCoach, *_checkTitleDate;

    QCheckBox *_checkWidth;

    QComboBox *_comboQuartals;

    QSpacerItem *_space;

    QDateEdit *_editDateBegin, *_editDateEnd;

    QLineEdit *_lineTitle;
    QDoubleSpinBox *_spinWidth;

    QCalendarWidget *_calendarWidget;

    ColorDialog *_colorDialog;

    //CrTableWidget *_table;
    CustomTabWidget *_tab;
    QStringList _tableLabels;

    void initDock();
    void initTable();

    void initCalendar();

    void setDates(MainWindow::Quartals q);

private slots:
    void comboQuartalsHandler(int index);

public slots:
    void openHandler();
    void saveHandler();
    void btnErrorHandler();
    void errorMsgHandler(QString);

    void btnColorHandler();
    void tmpHandler(QString msg);
    void newColorHandler(QColor newColor);

signals:
    void currentColor(QColor *color);


};

#endif // MAINWINDOW_H
