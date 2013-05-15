/*  -*- Mode: C++ -*-
 *
 * meego handset bluetooth
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */


#ifndef DEVICELISTWIDGET_H
#define DEVICELISTWIDGET_H
#include <MWidget>
#include <MButton>
#include <MLabel>
#include <MContentItem>
#include <QAbstractListModel>
#include <QList>
#include <QDBusObjectPath>
#include <MAction>
#include <MLinearLayoutPolicy>

#include "paireditem.h"
#include "bluedevice.h"

/*class MLabelItemCreator : public QObject, public MAbstractCellCreator<DeviceItem>
{
	Q_OBJECT
public:
	MLabelItemCreator(QObject* parent=NULL):QObject(parent){ }
	void updateCell(const QModelIndex& index, MWidget * cell) const
	{
		DeviceItem * contentItem = qobject_cast<DeviceItem *>(cell);
		Q_ASSERT(contentItem);
		QVariant data = index.data(Qt::DisplayRole);
		QString rowData = data.value<QString>();
		contentItem->setText(rowData);
	}	
};*/

class DeviceModel : public QAbstractListModel
{
	Q_OBJECT

public:
	DeviceModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;

	QDBusObjectPath devicePath(int index) { return devicepathlist[index]; }

	QList<QDBusObjectPath> devices() { return devicepathlist; }
signals:
	void deviceCreated(QDBusObjectPath);
	void deviceRemoved(QDBusObjectPath);

private:
	QList<QDBusObjectPath> devicepathlist;
};


class DeviceListWidget : public MWidget
{
	Q_OBJECT
public:
	DeviceListWidget(MWidget* parent=NULL);

	QList<PairedItem*> pairedItems() { return items; }

private slots:
	void deviceCreated(QDBusObjectPath);
	void deviceRemoved(QDBusObjectPath);
	void deviceRemoved();
private:
	QList<PairedItem*> items;
	MLinearLayoutPolicy *policy;

};

#endif // DEVICELISTWIDGET_H
