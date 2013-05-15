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

#include "bluetoothtransferpage.h"
#include <MLayout>
#include <MDialog>
#include <MApplicationWindow>

#include "../src/btprofiles.h"
#include "../src/devicelistwidget.h"
#include "../src/ftptransferwidget.h"

BluetoothTransferPage::BluetoothTransferPage()
{
	MWidget* widget = new MWidget();

	MLayout *layout = new MLayout(widget);

	policy = new MLinearLayoutPolicy(layout,Qt::Vertical);

	layout->setPolicy(policy);
	widget->setLayout(layout);

	DeviceModel* model = new DeviceModel(this);

	foreach(QDBusObjectPath device, model->devices())
	{
		PairedItem* item = new PairedItem(device.path(), widget);

		if(item->hasProfile(BluetoothProfiles::ftp) || item->hasProfile(BluetoothProfiles::opush))
		{
			items.append(item);
			policy->addItem(item);
			connect(item,SIGNAL(clicked()),
				this,SLOT(deviceClicked()));
		}
		else item->deleteLater();
	}

	setCentralWidget(widget);
}

void BluetoothTransferPage::removeDevice(QString hwaddress)
{

}

void BluetoothTransferPage::deviceFound(QString hwaddress, QVariantMap properties)
{

}

void BluetoothTransferPage::deviceClicked()
{
	PairedItem *item = qobject_cast<PairedItem*>(sender());

	if(!item) return;

	MDialog *dialog = new MDialog("Bluetooth Transfer",M::CancelButton);

	FtpTransferWidget* transferUI = new FtpTransferWidget(item->address(), item);

	dialog->setCentralWidget(transferUI);

	connect(transferUI,SIGNAL(complete()),dialog,SLOT(dismiss()));

	dialog->appear(MSceneWindow::DestroyWhenDismissed);

	transferUI->transferFile("file.txt", "file.txt");
}

void BluetoothTransferPage::deviceCreated(QDBusObjectPath)
{

}

void BluetoothTransferPage::deviceRemoved(QDBusObjectPath)
{

}
