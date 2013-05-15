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


#include "devicelistwidget.h"

#include <QGraphicsLinearLayout>
#include <QtDebug>
#include <MLabel>
#include <MList>
#include <MPannableViewport>
#include <MLinearLayoutPolicy>
#include <MLayout>
#include <MApplicationPage>
#include <MContentItem>
#include <QDBusPendingCallWatcher>
#include <QXmlQuery>
#include <QDomDocument>
#include <MImageWidget>
#include <MDialog>

#include "bluemanager.h"
#include "blueadapter.h"
#include "bluedevice.h"
#include "audiosink.h"
#include "audiosource.h"
#include "profileactionpage.h"
//#include "syncui/bluetoothsyncpage.h"



DeviceModel::DeviceModel(QObject *parent)
	: QAbstractListModel(parent)
{
	OrgBluezManagerInterface *manager = new OrgBluezManagerInterface(
			"org.bluez",
			"/", QDBusConnection::systemBus(), this);

	QDBusObjectPath adapterpath = manager->DefaultAdapter();

	if(adapterpath.path() == "")
		return;

	OrgBluezAdapterInterface *adapter = new OrgBluezAdapterInterface(
			"org.bluez",
			adapterpath.path(),
			QDBusConnection::systemBus(), this);

	devicepathlist = adapter->ListDevices();

	connect(adapter,
		SIGNAL(DeviceRemoved(QDBusObjectPath)),
		this,
		SIGNAL(deviceRemoved(QDBusObjectPath)));

	connect(adapter,
		SIGNAL(DeviceCreated(QDBusObjectPath)),
		this,
		SIGNAL(deviceCreated(QDBusObjectPath)));

}

int DeviceModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 1;
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		QString rowData;
		if(index.row() < devicepathlist.size())
		{
			rowData = devicepathlist[index.row()].path();
		}
		return QVariant(rowData);
	}

	return QVariant();
}


DeviceListWidget::DeviceListWidget(MWidget* parent)
	:MWidget(parent)
{
	MLayout* layout = new MLayout(this);

	policy = new MLinearLayoutPolicy(layout,Qt::Vertical);
	layout->setPolicy(policy);

	setLayout(layout);



	DeviceModel *model = new DeviceModel(this);

	connect(model,SIGNAL(deviceCreated(QDBusObjectPath)),
		this,SLOT(deviceCreated(QDBusObjectPath)));
	connect(model,SIGNAL(deviceRemoved(QDBusObjectPath)),this,
		SLOT(deviceRemoved(QDBusObjectPath)));

	if(model->devices().count())
	{
		//% "Devices:"
		policy->addItem(new MLabel(qtTrId("Devices:"),this));
	}

	foreach(QDBusObjectPath path, model->devices())
	{
		deviceCreated(path);
	}

}

void DeviceListWidget::deviceCreated(QDBusObjectPath path)
{
	qDebug()<<"signal caught to deviceCreated.  adding: "<<path.path()<<" to list";
	PairedItem* item = new PairedItem(path.path(),this);

	connect(item,SIGNAL(clicked()),item,SLOT(activateProfilePage()));

	if(!policy->count())
	{
		//% "Devices:"
		policy->addItem(new MLabel(qtTrId("Devices:"),this));
	}

	policy->addItem(item);
	items.append(item);

	connect(item,SIGNAL(removed()),this,SLOT(deviceRemoved()));

}

void DeviceListWidget::deviceRemoved(QDBusObjectPath removeable)
{
	for(int i=0; i<items.size(); i++)
	{
		qDebug()<<"removeable.path() "<<removeable.path()<<" devicepath: "<<items[i]->devicePath();
		if(removeable.path() == items[i]->devicePath())
		{
			qDebug()<<"signal caught to deviceRemove.  Removing: "<<removeable.path()<<" from list";
			//policy->removeAt(i+1);
			items[i]->deleteLater();
			items.removeAt(i);
			return;
		}

	}
}

void DeviceListWidget::deviceRemoved()
{
	for(int i=0; i<items.size(); i++)
	{
		if(sender() == items[i])
		{
			//policy->removeAt(i+1);
			items[i]->deleteLater();
			items.removeAt(i);
			return;
		}

	}
}
