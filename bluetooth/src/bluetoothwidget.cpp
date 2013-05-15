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



#include "bluetoothwidget.h"
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MComboBox>
#include <MCalendar>
#include <MLabel>
#include <MContainer>
#include <MButton>

#include <QFile>
#include <QStringList>
#include <QMap>

#include "bluemanager.h"
#include "bluetoothtogglewidget.h"
#include "devicelistwidget.h"
#include "nearbydeviceswidget.h"
#include "bluetoothvisibilitybutton.h"
#include "discoverwidget.h"

BluetoothWidget::BluetoothWidget(MWidget* parent)
	:MWidget(parent)
{
	MLayout *layout = new MLayout(this);

	setLayout(layout);

	MLinearLayoutPolicy * policy =
			new MLinearLayoutPolicy(layout,Qt::Vertical);

	policy->setObjectName("BluetoothWidget");

	OrgBluezManagerInterface manager(
			"org.bluez",
			"/", QDBusConnection::systemBus());

	QDBusObjectPath adapterpath = manager.DefaultAdapter();

	if(adapterpath.path().isEmpty() || adapterpath.path().isNull())
	{
		//% "No bluetooth adapter found."
		policy->addItem(new MLabel(qtTrId("No bluetooth adapter found"),this));
		qDebug()<<"Adapter path: "<<adapterpath.path();
		return;
	}

	BluetoothToggleWidget *toggle = new BluetoothToggleWidget(this);
	NearbyDevicesWidget *nearby = new NearbyDevicesWidget(this);

	connect(toggle,SIGNAL(toggled(bool)),nearby,SLOT(removeAll(bool)));

	policy->addItem(toggle);
	policy->addItem(new BluetoothVisibilityButton(this));
	policy->addItem(new DiscoverWidget(this));
	policy->addItem(new DeviceListWidget(this));
	policy->addItem(nearby);

}

