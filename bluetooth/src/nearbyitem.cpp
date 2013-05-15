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

#include "nearbyitem.h"
#include "bluemanager.h"
#include "blueadapter.h"
#include "bluetoothagent.h"

#include <MLayout>
#include <MLinearLayoutPolicy>

NearbyItem::NearbyItem(QString text, QString alias, MWidget* parent):
		MContentItem(MContentItem::SingleTextLabel, parent),
		label(new MLabel(text,parent)),
		m_pair(new MAction("Pair",this))
{
	//setObjectName("NearbyItem");
	m_pair->setLocation(MAction::ObjectMenuLocation);

	addAction(m_pair);


	if(alias != "")
		setTitle(alias);
	else setTitle(text);

	//connect(this, SIGNAL(clicked()), this, SLOT(pair()));
	connect(m_pair, SIGNAL(triggered()), this, SLOT(pair()));


	hwaddy = text;

	MLayout* layout = new MLayout(this);

	MLinearLayoutPolicy *policy =
			new MLinearLayoutPolicy(layout,Qt::Vertical);
	policy->addItem(label);

	setLayout(layout);
}

void NearbyItem::pair()
{
	qDebug("attempting to pair...");
	OrgBluezManagerInterface manager(
			"org.bluez",
			"/", QDBusConnection::systemBus());

	QDBusObjectPath adapterpath = manager.DefaultAdapter();

	OrgBluezAdapterInterface adapter(
			"org.bluez",
			adapterpath.path(),
			QDBusConnection::systemBus());

	new BluetoothAgent("/temp/agent",this);

	//QDBusPendingCall response =
	adapter.CreatePairedDevice(hwaddy,
				   QDBusObjectPath("/temp/agent"),"");

	//qDebug()<<"new object created: "<<object.path();
}

void NearbyItem::browse()
{

}
