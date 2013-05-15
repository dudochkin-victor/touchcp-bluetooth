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


#include "nearbydeviceswidget.h"
#include "bluedevice.h"
#include "bluetoothagent.h"
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MList>
#include <MPannableViewport>

void NearbyDevicesWidget::init()
{
	manager = new OrgBluezManagerInterface(
			"org.bluez",
			"/", QDBusConnection::systemBus(), this);

	connect(manager,
		SIGNAL(AdapterAdded(QDBusObjectPath)),
		this,
		SLOT(hookUpWithAdapter(QDBusObjectPath)));

	QDBusObjectPath adapterpath = manager->DefaultAdapter();

	if(adapterpath.path() == "")
		return;

	adapter = new OrgBluezAdapterInterface(
			"org.bluez",
			adapterpath.path(),
			QDBusConnection::systemBus(), this);

	adapter->StartDiscovery();

	connect(adapter,
		SIGNAL(DeviceFound(QString,QVariantMap)),
		this,
		SLOT(deviceFound(QString,QVariantMap)));
	connect(adapter,
		SIGNAL(DeviceDisappeared(QString)),
		this,
		SLOT(deviceDisappeared(QString)));

	/*foreach(QDBusObjectPath path, devicepathlist)
	{
		OrgBluezDeviceInterface device("org.bluez",path,
					       QDBusConnection::systemBus());
	}*/
}

NearbyDevicesWidget::~NearbyDevicesWidget()
{
}

void NearbyDevicesWidget::removeAll(bool)
{
	for(int i=0;i<devicepathlist.size();i++)
	{
		devicepathlist.removeAt(i);
		policy->removeAt(i+1);
	}
}

void NearbyDevicesWidget::deviceFound(QString hwaddy,QVariantMap properties)
{
	bool found = false;
	foreach(QString path, devicepathlist)
	{
		if(path == hwaddy)
		{
			found=true;
			break;
		}
	}

	if(!found)
	{
		/*foreach(QString var, properties.keys())
		{
			qDebug()<<"name: "<<var<<" value: "<<properties[var];
		}*/

		devicepathlist.append(hwaddy);

		QString alias = properties["Alias"].toString();

		NearbyItem* item = new NearbyItem(hwaddy, alias, this);
		connect(item,SIGNAL(clicked()),item,SLOT(pair()));

		policy->addItem(item);
	}
}

void NearbyDevicesWidget::deviceDisappeared(QString hwaddy)
{
	int i=-1;
	if((i = devicepathlist.indexOf(hwaddy)) >=0)

	{
		devicepathlist.removeAt(i);
		policy->removeAt(i+1);
	}

}

void NearbyDevicesWidget::hookUpWithAdapter(QDBusObjectPath)
{
	QDBusObjectPath adapterpath = manager->DefaultAdapter();

	if(adapterpath.path() == "")
		return;

	if(adapter) return;

	adapter = new OrgBluezAdapterInterface(
			"org.bluez",
			adapterpath.path(),
			QDBusConnection::systemBus(), this);

	connect(adapter,
		SIGNAL(DeviceFound(QString, QVariantMap)),
		this,
		SLOT(deviceFound(QString, QVariantMap)));
	connect(adapter,
		SIGNAL(DeviceDisappeared(QString)),
		this,
		SLOT(deviceDisappeared(QString)));
}

void NearbyDevicesWidget::adapterRemoved(QDBusObjectPath)
{
	QDBusObjectPath adapterpath = manager->DefaultAdapter();

	if(adapterpath.path() == "")
	{
		removeAll(true);
		return;
	}
}

NearbyDevicesWidget::NearbyDevicesWidget(MWidget* parent)
	:MWidget(parent),adapter(NULL)
{
	init();
	MLayout* layout = new MLayout(this);

	policy = new MLinearLayoutPolicy(layout,Qt::Vertical);
	layout->setPolicy(policy);

	setLayout(layout);

	//% "Nearby Devices:"
	policy->addItem(new MLabel(qtTrId("Nearby Devices:"),this));
}

