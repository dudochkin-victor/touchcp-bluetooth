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


#include "bluetoothvisibilitybutton.h"

#include "blueadapter.h"
#include "bluemanager.h"

#include <MLayout>
#include <MLinearLayoutPolicy>

BluetoothVisibilityButton::BluetoothVisibilityButton(QGraphicsItem *parent) :
		MWidget(parent)
{
	checkbox = new MButton(this);
	checkbox->setViewType(MButton::switchType);
	checkbox->setCheckable(true);

	connect(checkbox, SIGNAL(toggled(bool)),
		this, SLOT(toggleVisibility(bool)));

	OrgBluezManagerInterface manager(
			"org.bluez",
			"/", QDBusConnection::systemBus());

	QDBusObjectPath adapterpath = manager.DefaultAdapter();

	adapter = new OrgBluezAdapterInterface(
			"org.bluez",
			adapterpath.path(),
			QDBusConnection::systemBus(), this);

	QVariantMap map = adapter->GetProperties();
	checkbox->setChecked(map["Discoverable"].toBool());

	MLayout *layout = new MLayout(this);

	MLinearLayoutPolicy * policy =
			new MLinearLayoutPolicy(layout, Qt::Horizontal);

	//% "Visible"
	policy->addItem(new MLabel(qtTrId("Visible"),this),Qt::AlignLeft);
	policy->addItem(checkbox,Qt::AlignLeft);

	layout->setPolicy(policy);

	setLayout(layout);

	connect(adapter,
		SIGNAL(PropertyChanged(QString, QDBusVariant)),
		this,SLOT(propertyChanged(QString, QDBusVariant)));
}

void BluetoothVisibilityButton::toggleVisibility(bool visibility)
{
	adapter->SetProperty("Discoverable", QDBusVariant(visibility));
	adapter->SetProperty("DiscoverableTimeout", QDBusVariant(0));
}

void BluetoothVisibilityButton::propertyChanged(QString property,QDBusVariant value)
{
	if(property == "Discoverable")
	{
		checkbox->setChecked(value.variant().toBool());
	}
}
