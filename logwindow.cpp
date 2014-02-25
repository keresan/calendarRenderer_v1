#include "logwindow.h"

LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent){

  initWindow();
}



void LogWindow::initWindow() {
    _btnOk = new QPushButton("ok",this);

    _labTitle = new QLabel("log file:", this);

    _textConsole = new QTextEdit();
    _textConsole->setReadOnly(true);
    _textConsole->acceptRichText();

    _gridLayout = new QGridLayout(this);

    _gridLayout->addWidget(_labTitle,0,0,1,2);
    _gridLayout->addWidget(_textConsole,1,0,1,2);
    _gridLayout->addWidget(_btnOk,2,1);

    connect(_btnOk,SIGNAL(clicked()),this,SLOT(btnOkHandler()));

}

void LogWindow::btnOkHandler() {
    this->hide();
}


void LogWindow::addErrorMsg(QString msg) {
    _textConsole->append("<font color=\"red\">"+msg+"</font>");
}

void LogWindow::addWarningMsg(QString msg) {
    _textConsole->append("<font color=\"orange\">"+msg+"</font>");
}

void LogWindow::addInfoMsg(QString msg) {
    _textConsole->append("<font color=\"black\">"+msg+"</font>");
}


void LogWindow::cleanLogs() {
    _textConsole->clear();
}
