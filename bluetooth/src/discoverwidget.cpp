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

#include "discoverwidget.h"
#include <MButton>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MProgressIndicator>

#include "bluemanager.h"
#include "blueadapter.h"

DiscoverWidget::DiscoverWidget(QGraphicsItem *parent) :
    MWidget(parent)
{
	checkbox = new MButton(this);
	checkbox->setViewType(MButton::switchType);
	checkbox->setCheckable(true);

	connect(checkbox, SIGNAL(toggled(bool)),
		this, SLOT(toggleDiscovery(bool)));

	//% "Discover Devices"
	label = new MLabel(qtTrId("Discover Devices"));

	label->setAlignment(Qt::AlignLeft);

	OrgBluezManagerInterface manager(
			"org.bluez",
			"/", QDBusConnection::systemBus());

	QDBusObjectPath adapterpath = manager.DefaultAdapter();

	adapter = new OrgBluezAdapterInterface(
			"org.bluez",
			adapterpath.path(),
			QDBusConnection::systemBus(), this);

	connect(adapter,SIGNAL(PropertyChanged(QString,QDBusVariant)),this,
		SLOT(propertyChanged(QString,QDBusVariant)));

	QVariantMap props = adapter->GetProperties();

	isDiscovering = props["Discovering"].toBool();

	if(isDiscovering) checkbox->setChecked(true);

	MLayout *layout = new MLayout(this);

	policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);

	status = new MProgressIndicator(this,MProgressIndicator::spinnerType);

	status->setUnknownDuration(true);

	status->hide();

	policy->addItem(label, Qt::AlignLeft);

	policy->addItem(checkbox,Qt::AlignLeft);

	layout->setPolicy(policy);

	setLayout(layout);
}

void DiscoverWidget::toggleDiscovery(bool on)
{
	if(on && !isDiscovering)
		adapter->StartDiscovery();
	else adapter->StopDiscovery();
}

void DiscoverWidget::propertyChanged(QString name, QDBusVariant val)
{
	if(name == "Discovering")
	{
		isDiscovering = val.variant().toBool();
		checkbox->setChecked(val.variant().toBool());

		if(val.variant().toBool())
		{
			policy->insertItem(1,status);
			status->show();
			//% "Discovering..."
			label->setText(qtTrId("Discovering..."));
		}
		else
		{
			policy->removeItem(status);
			status->hide();
			//% "Discover Devices"
			label->setText(qtTrId("Discover Devices"));
		}
	}
}
