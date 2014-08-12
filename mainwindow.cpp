#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{

	Settings::setConstants();

	initDock();

	initTable();

    _logConsole = new LogWindow();

    _calendar = 0;
    setDates(MainWindow::Q1c);

    _colorDialog = new ColorDialog();
    _colorDialog->hide();

	connect(_btnColor, SIGNAL(clicked()), this, SLOT(btnColorHandler()));
    connect(_comboQuartals,SIGNAL(currentIndexChanged(int)), this, SLOT(comboQuartalsHandler(int)));
	connect(_colorDialog, SIGNAL(newColor(QColor)), this, SLOT(newColorHandler(QColor)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::loadFileAsString(QString path) {

    QFile file(path);
    bool result;
    result = file.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);

    QTextStream in(&file);

    return in.readAll();

}


void MainWindow::initDock() {

    //label
	_labTitle = new QLabel("Calendar Renderer "+ Settings::calendarVersion);

    //open
    _btnOpenFile = new QPushButton("otvoriť xml súbor");
   // _btnOpenFile->setStyleSheet("padding: 10px; corner-radius: 10px;");

    //qDebug() << "css" << _btnOpenFile->styleSheet();
    //_btnOpenFile->setStyleSheet("padding: 10px;" + _btnOpenFile->styleSheet()); //nefunguje
    //save
    _btnSaveFile = new QPushButton("uložiť html súbor");
    //_btnSaveFile->setStyleSheet("padding: 10px;");
    _btnSaveFile->setEnabled(false);
	//show web page button
	_btnShowWebPage = new QPushButton("zobraziť v prehliadači");
	_btnShowWebPage->setEnabled(false);
    //logs
    _btnError = new QPushButton("logs");
    //color
    _btnColor = new QPushButton("farba");
    _btnColor->setEnabled(false);

    //date
    _editDateBegin = new QDateEdit();
    _editDateBegin->setDisplayFormat("dd.MM.yyyy");
    _editDateEnd = new QDateEdit();
    _editDateEnd->setDisplayFormat("dd.MM.yyyy");

    _labQuart = new QLabel("kvartál:");

    _comboQuartals = new QComboBox();
    _comboQuartals->addItem(QString::number(QDate::currentDate().year()) + " Q1");
    _comboQuartals->addItem(QString::number(QDate::currentDate().year()) + " Q2");
    _comboQuartals->addItem(QString::number(QDate::currentDate().year()) + " Q3");
    _comboQuartals->addItem(QString::number(QDate::currentDate().year()) + " Q4");
    _comboQuartals->addItem(QString::number(QDate::currentDate().year()+1) + " Q1");
    _comboQuartals->addItem(QString::number(QDate::currentDate().year()+1) + " Q2");
    _comboQuartals->addItem(QString::number(QDate::currentDate().year()+1) + " Q3");
    _comboQuartals->addItem(QString::number(QDate::currentDate().year()+1) + " Q4");


    _gridLayoutDate = new QGridLayout;
    _gridLayoutDate->addWidget(_labQuart,0,0,Qt::AlignRight);
    _gridLayoutDate->addWidget(_comboQuartals,0,1);
    _gridLayoutDate->addWidget(_editDateBegin,1,0);
    _gridLayoutDate->addWidget(_editDateEnd,1,1);

    _boxDate = new QGroupBox("dátum");
    _boxDate->setLayout(_gridLayoutDate);

    //title edit
	_lineTitle = new QLineEdit(Settings::calendarTitleLabel,this);
    _lineTitle->setStyleSheet("padding-top: 3px; padding-bottom: 1px");
    _checkTitleDate = new QCheckBox("pridať dátum",this);
    _checkTitleDate->setChecked(true);

    _boxTitle =  new QGroupBox("nadpis kalendára",this);
    _gridLayoutTitle = new QGridLayout;
    _gridLayoutTitle->addWidget(_lineTitle,0,0);
    _gridLayoutTitle->addWidget(_checkTitleDate,1,0,1,2);
    _boxTitle->setLayout(_gridLayoutTitle);

    // event title width
    _spinWidth = new QDoubleSpinBox(this);
	_spinWidth->setValue(Settings::widthEventTitle);
    _spinWidth->setDecimals(1);
    _spinWidth->setMinimum(5.0);
    _spinWidth->setMaximum(99.0);

    /*
    _lineWidth = new QLineEdit("80", this);
    //_lineWidth->setStyleSheet("padding-top: 3px; padding-bottom: 1px");
    _lineWidth->setInputMask("009");
    _lineWidth->setFixedSize(QSize(60,30));
    _lineWidth->setAlignment(Qt::AlignCenter);
    _lineWidth->setFont(QFont("Courier New",17,QFont::Bold));
    */

    _checkWidth = new QCheckBox("max šírka [cm]",this);
    _checkWidth->setChecked(true);

    _boxWidth =  new QGroupBox("názov školenia",this);
    _gridLayoutWidth = new QGridLayout;
    _gridLayoutWidth->addWidget(_checkWidth,0,0);
    _gridLayoutWidth->addWidget(_spinWidth,0,1);
    _boxWidth->setLayout(_gridLayoutWidth);

    //checkboxes
	_checkEvent = new QCheckBox("",this);
	_checkSoftskill = new QCheckBox("",this);
	_checkRoom = new QCheckBox("",this);
	_checkInstructor = new QCheckBox("",this);

	_checkEvent->setChecked(Settings::generateEvent);
	_checkSoftskill->setChecked(Settings::generateSoftskill);
	_checkRoom->setChecked(Settings::generateRoom);
	_checkInstructor->setChecked(Settings::generateInstructor);

	setExportCheckboxiesEnable(true);

	_lineEventLabel = new QLineEdit(Settings::eventLabel, this);
	_lineEventAfterDeadlineLabel = new QLineEdit(Settings::eventAfterDeadlineLabel,this);
	_lineSoftskillLabel = new QLineEdit(Settings::softSkillLabel, this);
	_lineSoftskillAfterDeadlineLabel = new QLineEdit(Settings::softSkillAfterDeadlineLabel,this);
	_lineRoomLabel = new QLineEdit(Settings::roomLabel, this);
	_lineInstructorLabel = new QLineEdit(Settings::instructorLabel, this);

	//ak je text dlhsi ako sirka editu
	_lineEventLabel->setCursorPosition(0);
	_lineEventAfterDeadlineLabel->setCursorPosition(0);
	_lineSoftskillLabel->setCursorPosition(0);
	_lineSoftskillAfterDeadlineLabel->setCursorPosition(0);
	_lineRoomLabel->setCursorPosition(0);
	_lineInstructorLabel->setCursorPosition(0);


    _boxExport = new QGroupBox("export",this);

    _gridLayoutOption = new QGridLayout;
	_gridLayoutOption->addWidget(_checkEvent,0,0);
	_gridLayoutOption->addWidget(_lineEventLabel,0,1);
	_gridLayoutOption->addWidget(_lineEventAfterDeadlineLabel,1,1);

	_gridLayoutOption->addWidget(_checkSoftskill,2,0);
	_gridLayoutOption->addWidget(_lineSoftskillLabel,2,1);
	_gridLayoutOption->addWidget(_lineSoftskillAfterDeadlineLabel,3,1);

	_gridLayoutOption->addWidget(_checkRoom,4,0);
	_gridLayoutOption->addWidget(_lineRoomLabel,4,1);
	_gridLayoutOption->addWidget(_checkInstructor,5,0);
	_gridLayoutOption->addWidget(_lineInstructorLabel,5,1);
    _boxExport->setLayout(_gridLayoutOption);

    //space
    _space = new QSpacerItem(5,5,QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    //grid
    _gridLayout = new QGridLayout;

    _gridLayout->addWidget(_labTitle,0,0,1,2);

	_gridLayout->addWidget(_boxExport,1,0,6,2);

	_gridLayout->addWidget(_btnOpenFile,7,0,1,2);
	_gridLayout->addWidget(_boxDate,8,0,2,2);

	_gridLayout->addWidget(_boxTitle,10,0,2,2);

	_gridLayout->addWidget(_boxWidth,12,0,1,2);

	_gridLayout->addWidget(_btnColor,13,0);
	_gridLayout->addWidget(_btnError,13,1);

	_gridLayout->addWidget(_btnSaveFile,14,0,1,2);

	_gridLayout->addWidget(_btnShowWebPage,15,0,1,2);

	_gridLayout->addItem(_space,16,1,1,2);
	_gridLayout->setColumnStretch(0,5);
	_gridLayout->setColumnStretch(1,5);


    _optionMenu = new QWidget();
    _optionMenu->setLayout(_gridLayout);

    _dockWidget = new QDockWidget("menu", this);
    _dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    _dockWidget->setWidget(_optionMenu);

    this->addDockWidget(Qt::RightDockWidgetArea, _dockWidget);

    connect(_btnOpenFile, SIGNAL(clicked()), this, SLOT(openHandler()));
    connect(_btnSaveFile, SIGNAL(clicked()), this, SLOT(saveHandler()));
	connect(_btnShowWebPage, SIGNAL(clicked()), this, SLOT(showWebPageHandler()));
    connect(_btnError, SIGNAL(clicked()), this, SLOT(btnErrorHandler()));

}

void MainWindow::btnColorHandler() {

    if(_tab->isActiveRow()) {

        QColor initColor = _tab->getCurrentRowColor();
        _colorDialog->showWithColor(initColor);
    }
    /*
    newColor = QColorDialog::getColor(initColor);
    _tab->setCurrentRowColor(newColor);
    */
}

void MainWindow::tmpHandler(QString msg) {
    qDebug() << "sprava: " << msg;
}

void MainWindow::btnErrorHandler() {
    _logConsole->show();
}

void MainWindow::errorMsgHandler(QString) {
    _btnError->setStyleSheet("QPushButton { color : red; }");
}

void MainWindow::openHandler() {


	Settings::generateEvent = _checkEvent->isChecked();
	Settings::generateSoftskill = _checkSoftskill->isChecked();
	Settings::generateRoom = _checkRoom->isChecked();
	Settings::generateInstructor = _checkInstructor->isChecked();

    if(_calendar) {
        //delete old calendar
        qDebug() << "delete calendar";

        disconnect(_calendar, SIGNAL(signalErrorMsg(QString)), _logConsole, SLOT(addErrorMsg(QString)));
        disconnect(_calendar, SIGNAL(signalWarningMsg(QString)), _logConsole, SLOT(addWarningMsg(QString)));
        disconnect(_calendar, SIGNAL(signalInfoMsg(QString)), _logConsole, SLOT(addInfoMsg(QString)));
        disconnect(_calendar, SIGNAL(signalErrorMsg(QString)), this, SLOT(errorMsgHandler(QString)));

        CalendarData::clearData();
        delete _calendar;


    }
     qDebug() << "create calendar";
    _calendar = new CrCalendar(_editDateBegin->date(), _editDateEnd->date(),this);

    //error handling
    _logConsole->cleanLogs();
    connect(_calendar, SIGNAL(signalErrorMsg(QString)), _logConsole, SLOT(addErrorMsg(QString)));
    connect(_calendar, SIGNAL(signalWarningMsg(QString)), _logConsole, SLOT(addWarningMsg(QString)));
    connect(_calendar, SIGNAL(signalInfoMsg(QString)), _logConsole, SLOT(addInfoMsg(QString)));
    connect(_calendar, SIGNAL(signalErrorMsg(QString)), this, SLOT(errorMsgHandler(QString)));


	//QString xmlFilePath = QFileDialog::getOpenFileName(this,"open xml file",QString(),"xml file(*.xml)");
	 QString xmlFilePath = "/Users/martin/Documents/[]sklad/calendar renderer data/problem1/?Q?Q3=5Fstav_k_1=5F4=5F2014.xml";
	//QString xmlFilePath = "/Users/martin/Documents/[]sklad/calendar renderer data/EBO_2Q2014_D1_vypis_vsetky_skolenia.xml";
	//QString xmlFilePath = "/Users/martin/Documents/[]sklad/calendar renderer data/Dotaz_Vyber_2014_Q2+ZAMOK zamknuty.xml";
	//QString xmlFilePath = "/Users/martin/Documents/[]sklad/calendar renderer data/3_EMO1a2Q2014_D1_vypis_vsetky_skolenia.xml";


    //QString xmlFilePath = "/Users/martin/Documents/[] sklad/calendar renderer data/in_1.xml";


    _calendar->loadDataFromXml(xmlFilePath);
    //_calendar->readXmlFile(xmlFilePath);
    //_calendar->sortCompressedList();

    _btnSaveFile->setEnabled(true);
	//docasne vypnute kvoli prerabne TrainingRoom
	_btnColor->setEnabled(true);

	//_btnOpenFile->setEnabled(false);
    //_editDateBegin->setEnabled(false);
    //_editDateEnd->setEnabled(false);

	_tab->clearTables();
	_tab->loadData();

	setExportCheckboxiesEnable(false);
}

void MainWindow::saveHandler() {

	Settings::calendarTitleLabel = getTitle();

	Settings::eventLabel = _lineEventLabel->text();
	Settings::eventAfterDeadlineLabel = _lineEventAfterDeadlineLabel->text();
	Settings::roomLabel = _lineRoomLabel->text();
	Settings::instructorLabel = _lineInstructorLabel->text();

	Settings::widthEventTitle = _spinWidth->value();

    _calendar->setDates(_editDateBegin->date(), _editDateEnd->date());

	// .event_title, .room_title { width: 100mm; }
    QString eventTitleWidth;
    if(_checkWidth->isChecked()) {
        eventTitleWidth = "." +Cell::getCssClassAsString(Cell::event_title)
                 + ", ." +Cell::getCssClassAsString(Cell::group_title)+
                + ", ." +Cell::getCssClassAsString(Cell::room_title)+ " { width: "
				+ QString::number((int)(Settings::widthEventTitle*10))+ "mm;}\n";

    }

    QString cssFile = loadFileAsString(":default.css");
    _calendar->setCss(cssFile + eventTitleWidth);

	_calendar->generateCalendar();


	//QString filename = QFileDialog::getSaveFileName(this,"save html page",QString(), "html (*.html)");
	QString filename = "/Users/martin/Documents/[]sklad/calendar renderer data/page_1_13.html";

	//terminate, if no file path specified
	if(filename.length() == 0) {
		return;
	}
	_saveHtmlFilePath = filename;
	_btnShowWebPage->setEnabled(true);

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << _calendar->getHtml();
    file.close();

	//qDebug() << "file save to: " << filename;
    _logConsole->addInfoMsg("file save to: "+ filename);

    //_btnOpenFile->setEnabled(true);
    //_btnSaveFile->setEnabled(false);
    //_editDateBegin->setEnabled(true);
    //_editDateEnd->setEnabled(true);
    _btnError->setStyleSheet("QPushButton { color : black; }");
}

void MainWindow::showWebPageHandler() {
	bool result = false;
	QUrl url = QUrl::fromLocalFile(_saveHtmlFilePath);
	result = QDesktopServices::openUrl(url);
	qDebug() << url.toString();
	/*
	if(result) {
		qDebug() << "uspesne otvorene";
	} else {
		qDebug() << "chyba pri otvarani stranky";
	}
	*/
}

void MainWindow::initTable() {
    _tab = new CustomTabWidget(this);


	_tab->setMinimumWidth(600);


    this->setCentralWidget(_tab);
	//this->centralWidget()->resize(QSize(_tab->sizeHint().width(), this->centralWidget()->height()));

}

QString MainWindow::getTitle() {
    QString returnStr;

    if(_checkTitleDate->isChecked()) {
        returnStr = _lineTitle->text() + "  " + _editDateBegin->date().toString("dd.MM.yyyy") +" až "+_editDateEnd->date().toString("dd.MM.yyyy");
    } else {
        returnStr = _lineTitle->text();
    }

    return returnStr;
}


void MainWindow::setDates(MainWindow::Quartals q) {
    int startMonth, year;

    year = QDate::currentDate().year();

    switch(q) {
        case MainWindow::Q1c: startMonth = 1; break;
        case MainWindow::Q2c: startMonth = 4; break;
        case MainWindow::Q3c: startMonth = 7; break;
        case MainWindow::Q4c: startMonth = 10; break;
        case MainWindow::Q1n: startMonth = 1; year++; break;
        case MainWindow::Q2n: startMonth = 4; year++; break;
        case MainWindow::Q3n: startMonth = 7; year++; break;
        case MainWindow::Q4n: startMonth = 10; year++; break;
    }
    _editDateBegin->setDate(QDate(year,startMonth,1));
    _editDateEnd->setDate(QDate(year,startMonth+2,QDate(year,startMonth+2,1).daysInMonth()));
}

void MainWindow::comboQuartalsHandler(int index) {
    switch(index) {
        case 0: setDates(MainWindow::Q1c); break;
        case 1: setDates(MainWindow::Q2c); break;
        case 2: setDates(MainWindow::Q3c); break;
        case 3: setDates(MainWindow::Q4c); break;
        case 4: setDates(MainWindow::Q1n); break;
        case 5: setDates(MainWindow::Q2n); break;
        case 6: setDates(MainWindow::Q3n); break;
        case 7: setDates(MainWindow::Q4n); break;
        default: assert(false);
    }
}

void MainWindow::newColorHandler(QColor newColor) {
    _tab->setCurrentRowColor(newColor);
}

void MainWindow::setExportCheckboxiesEnable(bool state) {
	_checkEvent->setEnabled(state);
	_checkSoftskill->setEnabled(state);
}
