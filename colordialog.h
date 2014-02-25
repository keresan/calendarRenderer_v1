#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QIcon>
#include <assert.h>
#include <QDebug>

class ColorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ColorDialog(QWidget *parent = 0);

    QIcon createIcon(int w, int h, QColor color);

signals:
    void newColor(QColor);

public slots:
    void showWithColor(QColor color);
private:

    void loadColors(QString path);

    QGridLayout *_gridLayout;
    QLabel *_labColorInit, *_labColorNew;
    QLineEdit *_lineColorInit, *_lineColorNew;
    QComboBox *_comboColors;
    QPushButton *_btnOk, *_btnCancel;

    QColor _color;

private slots:
    void lineColorHandler(QString text);
    void comboColorHandler(QString text);

    void btnOkHandler();
    void btnCancelHandler();


};

#endif // COLORDIALOG_H
