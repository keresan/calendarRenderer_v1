#include "colordialog.h"

ColorDialog::ColorDialog(QWidget *parent) :
    QDialog(parent) {

    _color = QColor("#FFFFFF");

    _btnCancel = new QPushButton("zrusit", this);
    _btnOk = new QPushButton("ok", this);

    _comboColors = new QComboBox();

    _lineColorInit = new QLineEdit();
    _lineColorInit->setFixedSize(QSize(120,40));
    _lineColorInit->setAlignment(Qt::AlignCenter);
    _lineColorInit->setFont(QFont("Courier New",17,QFont::Bold));
    _lineColorInit->setReadOnly(true);

    _lineColorNew = new QLineEdit();
    _lineColorNew->setFixedSize(QSize(120,40));
    _lineColorNew->setAlignment(Qt::AlignCenter);
    _lineColorNew->setFont(QFont("Courier New",17,QFont::Bold));


    _lineColorNew->setInputMask("\\#HHHHHH");

    _labColorInit = new QLabel("pôvodná:");

    _labColorNew = new QLabel("nová:");


    _gridLayout = new QGridLayout();



    _gridLayout->addWidget(_labColorInit,1,0,1,1,Qt::AlignRight);
    _gridLayout->addWidget(_lineColorInit,1,1,1,1,Qt::AlignLeft);
    _gridLayout->addWidget(_labColorNew,2,0,1,1,Qt::AlignRight);
    _gridLayout->addWidget(_lineColorNew,2,1,1,1,Qt::AlignLeft);

    _gridLayout->addWidget(_comboColors,3,0,1,2);

    _gridLayout->addWidget(_btnOk,4,0);
    _gridLayout->addWidget(_btnCancel,4,1);



    this->setLayout(_gridLayout);

     loadColors(":colors.csv");

    connect(_comboColors,SIGNAL(activated(QString)),this, SLOT(comboColorHandler(QString)));
    connect(_lineColorNew, SIGNAL(textEdited(QString)),this, SLOT(lineColorHandler(QString)));
    connect(_btnOk, SIGNAL(clicked()), this, SLOT(btnOkHandler()));
    connect(_btnCancel, SIGNAL(clicked()), this, SLOT(btnCancelHandler()));
}

void ColorDialog::showWithColor(QColor color) {

    _lineColorInit->setText(color.name());
    _lineColorInit->setStyleSheet("background-color: "+color.name()+";");

    _lineColorNew->setText(_color.name());
    _lineColorNew->setStyleSheet("background-color: "+_color.name()+";");

    this->show();
}

void ColorDialog::loadColors(QString path) {

    _comboColors->setMinimumHeight(40);
    _comboColors->setStyleSheet("icon-size: 25px;");
   // _comboColors->setStyleSheet("icon-height: 20px; icon-width:100px;");

    int index = 0;
    QFile file(path);
    bool result;
    result = file.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);

    QTextStream in(&file);

    while(!in.atEnd()) {

        QStringList line = in.readLine().split(';',QString::SkipEmptyParts);

        QPixmap pixmap(30,30);
        pixmap.fill(QColor(line.at(1)));

        _comboColors->addItem(line.at(0) + "  (" + line.at(1).toLower() + ")");
        _comboColors->setItemData(index,pixmap,Qt::DecorationRole);

        index++;
    }
    file.close();

    _comboColors->setMinimumWidth(_comboColors->sizeHint().width());
}

QIcon ColorDialog::createIcon(int w, int h, QColor color) {
    QPixmap pixmap(w,h);
    pixmap.fill(color);

    QIcon icon(pixmap);

    return icon;
}

void ColorDialog::comboColorHandler(QString text) {

    QString colorName = text.split('(').at(1);
    colorName = colorName.split(')').first();
    colorName = colorName.simplified();


    _lineColorNew->setText(colorName);
    _lineColorNew->setStyleSheet("background-color: "+colorName+";");
    _color = QColor(colorName);

}


void ColorDialog::lineColorHandler(QString text) {
    if(_lineColorNew->text().length() == 7) {

        _lineColorNew->setStyleSheet("background-color: "+_lineColorNew->text()+";");
        _color = QColor(_lineColorNew->text());
    }
}

void ColorDialog::btnOkHandler() {
    this->hide();
    emit newColor(_color);
}

void ColorDialog::btnCancelHandler() {
    this->hide();
}
