#-------------------------------------------------
#
# Project created by QtCreator 2013-11-30T20:40:08
#
#-------------------------------------------------

QT	+= core gui
QT	+= printsupport
QT	+= xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CalRend
TEMPLATE = app

macx {
    ICON = myicon.icns
}

win32 {
    RC_ICONS = myicon.ico
}

SOURCES += main.cpp\
        mainwindow.cpp \
    crevent.cpp \
    crcalendar.cpp \
    cell.cpp \
    trainingroom.cpp \
    logwindow.cpp \
        crtablewidget.cpp \
    calendardata.cpp \
    customtabwidget.cpp \
    colordialog.cpp \
    settings.cpp \
    tagdescriptor.cpp

HEADERS  += mainwindow.h \
    crevent.h \
    crcalendar.h \
    cell.h \
    trainingroom.h \
    logwindow.h \
    crtablewidget.h \
    calendardata.h \
    customtabwidget.h \
    colordialog.h \
    settings.h \
    tagdescriptor.h

OTHER_FILES +=

RESOURCES += \
    resorces.qrc
