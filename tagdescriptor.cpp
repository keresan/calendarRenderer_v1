#include "tagdescriptor.h"

TagDescriptor::TagDescriptor(QStringList &captions) {

	this->setColumnCount(captions.count());
	this->setRowCount(0);

	this->setHorizontalHeaderLabels(captions);
	this->horizontalHeader()->setStretchLastSection(true);

	this->verticalHeader()->hide();


	this->setFont(QFont("Verdana"));
}


void TagDescriptor::loadData() {

	QFile file(":/tag_desc.txt");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	int actualRow = 0;
	QTableWidgetItem *newItem;

	this->setWordWrap(true);

	while (!file.atEnd()) {

		QString line = file.readLine();

		if(line.length() < 5 || line.trimmed().startsWith('%')) {
			continue;
		}

		QStringList data;
		data = line.split(";",QString::KeepEmptyParts);

		//increment row count
		this->setRowCount(this->rowCount()+1);

		//tag
		newItem = new QTableWidgetItem(data[0].trimmed());
		newItem->setTextAlignment(Qt::AlignLeft);
		newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		QFont font = newItem->font();
		font.setBold(true);
		newItem->setFont(font);
		this->setItem(actualRow, 0, newItem);

		//value
		newItem = new QTableWidgetItem(data[1].trimmed());
		newItem->setTextAlignment(Qt::AlignLeft);
		newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		this->setItem(actualRow, 1, newItem);

		//description
		newItem = new QTableWidgetItem(data[2].trimmed());
		newItem->setTextAlignment(Qt::AlignLeft);
		newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		this->setItem(actualRow, 2, newItem);

		actualRow++;

	}
	file.close();

	this->resizeColumnToContents(0);
	this->resizeColumnToContents(1);
	this->resizeRowsToContents();
}
