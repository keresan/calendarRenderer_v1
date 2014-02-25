#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QTextEdit>
#include <QLabel>

class LogWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LogWindow(QWidget *parent = 0);


signals:

public slots:
    void addErrorMsg(QString msg);
    void addWarningMsg(QString msg);
    void addInfoMsg(QString msg);
    void cleanLogs();
private slots:
    void btnOkHandler();

private:

    QPushButton *_btnOk;
    QGridLayout *_gridLayout;
    QTextEdit *_textConsole;
    QLabel *_labTitle;



    void initWindow();

};

#endif // LOGWINDOW_H
