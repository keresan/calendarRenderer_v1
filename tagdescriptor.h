#ifndef TAGDESCRIPTOR_H
#define TAGDESCRIPTOR_H

#include <QTableWidget>
#include <QHeaderView>
#include <QFile>
#include <QDebug>

class TagDescriptor : public QTableWidget
{
public:
	TagDescriptor(QStringList &captions);
	void loadData();
private:


};

#endif // TAGDESCRIPTOR_H
