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


#include "profileactionpage.h"
#include <MLayout>
#include <MWidget>
#include <MAction>
#include "audiosink.h"
#include "audiosource.h"
#include "bluedevice.h"
#include "headset.h"

ProfileActionWidget::ProfileActionWidget(QString devicePath, bool noRemoveButton, MWidget * parent) :
    MWidget(parent), device(devicePath)
{
//	setTitle("Device Actions:");

	//% "Connect Audio Device"
	profilesMap[BluetoothProfiles::a2sink] = qtTrId("Connect Audio Device");
	//% "Connect Audio Source"
	profilesMap[BluetoothProfiles::a2src] = qtTrId("Connect Audio Source");
	//% "Connect Headset"
	profilesMap[BluetoothProfiles::hs] = qtTrId("Connect Headset");

	///Other potentially supported profiles:
	//profilesMap[BluetoothProfiles::hf] ="Connect Hands Free";
	//profilesMap[panu] = "Connect Internet";
	//profilesMap[spp] = "Connect Serial";
	//profilesMap[ftp] = "Browse Files";
	//profilesMap[opush] = "Send File(s)";
	//profilesMap[syncml]= "Sync Device";

	///create profileWidget
	MLayout* layout = new MLayout(this);

	profilePolicy = new MLinearLayoutPolicy(layout,Qt::Vertical);
	layout->setPolicy(profilePolicy);

	setLayout(layout);

	OrgBluezDeviceInterface * dbusdevice = new OrgBluezDeviceInterface("org.bluez", device,
							QDBusConnection::systemBus(),this);

	connect(dbusdevice,SIGNAL(PropertyChanged(QString,QDBusVariant)),this,
		SLOT(propertyChanged(QString,QDBusVariant)));

	QVariantMap props = dbusdevice->GetProperties();

	QStringList uuidlist = props["UUIDs"].toStringList();

	foreach(QString uuid, uuidlist)
	{
		enableProfile(uuid.toLower());
	}

	if(!noRemoveButton)
	{
		MButton* removeButton = new MButton("Remove",this);

		profilePolicy->addItem(removeButton);

		connect(removeButton, SIGNAL(clicked()),
			this, SIGNAL(removeButtonClicked()));
	}
}


void ProfileActionWidget::enableProfile(QString profile)
{
	if(!profilesMap.contains(profile))
	{
		/// This means we don't know about this profile or we can't do
		/// anything with it.
		return;
	}

	MButton* button = new MButton(profilesMap[profile], this);

	buttonMap[profile] = button;

	profilePolicy->insertItem(profilePolicy->count()-1, button);

	if(profile == BluetoothProfiles::panu)
	{
		connect(button,
			SIGNAL(clicked()),
			this,
			SIGNAL(connectPanClicked()));
	}
	else if(profile == BluetoothProfiles::a2sink)
	{
		connect(button,
			SIGNAL(clicked()),
			this,
			SIGNAL(connectAudioClicked()));

		OrgBluezAudioSinkInterface *interface = new OrgBluezAudioSinkInterface(
									"org.bluez",
									device,
									QDBusConnection::systemBus(),this);

		connect(interface,SIGNAL(PropertyChanged(QString,QDBusVariant)),
			this,SLOT(audioSinkPropertiesChanged(QString,QDBusVariant)));

		audioSinkPropertiesChanged("",QDBusVariant());

	}
	else if(profile == BluetoothProfiles::a2src)
	{
		connect(button,
			SIGNAL(clicked()),
			this,
			SIGNAL(connectAudioSrcClicked()));

		OrgBluezAudioSourceInterface *interface = new OrgBluezAudioSourceInterface(
									"org.bluez",
									device,
									QDBusConnection::systemBus(),this);

		connect(interface,SIGNAL(PropertyChanged(QString,QDBusVariant)),
			this,SLOT(audioSourcePropertiesChanged(QString,QDBusVariant)));

	}
	else if(profile == BluetoothProfiles::syncml)
	{
		connect(button,
			SIGNAL(clicked()),
			this,
			SIGNAL(syncClicked()));
	}
	else if(profile == BluetoothProfiles::hs)
	{
		connect(button,
			SIGNAL(clicked()),
			this,
			SIGNAL(connectHeadsetClicked()));

		OrgBluezHeadsetInterface *interface = new OrgBluezHeadsetInterface(
				"org.bluez",
				device,
				QDBusConnection::systemBus(), this);

		connect(interface,SIGNAL(PropertyChanged(QString,QDBusVariant)),
			this, SLOT(headsetPropertiesChanged(QString,QDBusVariant)));

		if(interface->IsConnected())
			headsetPropertiesChanged("Connected",QDBusVariant(1));
	}
	else { }

}

void ProfileActionWidget::propertyChanged(QString property, QDBusVariant value)
{
	if(property == "UUIDs")
	{
		QStringList uuidlist = value.variant().toStringList();

		foreach(QString uuid, uuidlist)
		{
			if(!buttonMap.contains(uuid))
				enableProfile(uuid);
		}
	}
}

void ProfileActionWidget::audioSinkPropertiesChanged(QString key,QDBusVariant val)
{
	if(!buttonMap.contains(BluetoothProfiles::a2sink))
		return;

	OrgBluezAudioSinkInterface interface(
			"org.bluez",
			device,
			QDBusConnection::systemBus());

	//% "Connect"
	QString connectString = qtTrId("Connect");
	//% "Disconnect"
	QString disconnectString = qtTrId("Disconnect");

	if(interface.IsConnected())
	{
		buttonMap[BluetoothProfiles::a2sink]->setText(
				profilesMap[BluetoothProfiles::a2sink].replace(connectString,disconnectString));
	}
	else if(!interface.IsConnected())
	{
		buttonMap[BluetoothProfiles::a2sink]->setText(
				profilesMap[BluetoothProfiles::a2sink].replace(disconnectString,connectString));
	}
}

void ProfileActionWidget::audioSourcePropertiesChanged(QString key, QDBusVariant val)
{
	if(!buttonMap.contains(BluetoothProfiles::a2src) || key != "State")
		return;

	//% "Connect"
	QString connectString = qtTrId("Connect");
	//% "Disconnect"
	QString disconnectString = qtTrId("Disconnect");

	if(val.variant().toString() == "connected")
	{
		buttonMap[BluetoothProfiles::a2src]->setText(
				profilesMap[BluetoothProfiles::a2src].replace(connectString,disconnectString));
	}
	else
	{
		buttonMap[BluetoothProfiles::a2src]->setText(
				profilesMap[BluetoothProfiles::a2src].replace(disconnectString,connectString));
	}
}

void ProfileActionWidget::headsetPropertiesChanged(QString key, QDBusVariant val)
{
	if(!buttonMap.contains(BluetoothProfiles::a2src) || key != "Connected")
		return;

	//% "Connect"
	QString connectString = qtTrId("Connect");
	//% "Disconnect"
	QString disconnectString = qtTrId("Disconnect");

	if(val.variant().toInt() == 1)
	{
		buttonMap[BluetoothProfiles::hs]->setText(
				profilesMap[BluetoothProfiles::hs].replace(connectString,disconnectString));
	}
	else
	{
		buttonMap[BluetoothProfiles::hs]->setText(
				profilesMap[BluetoothProfiles::hs].replace(disconnectString,connectString));
	}
}
