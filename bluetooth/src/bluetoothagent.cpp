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


#include "bluetoothagent.h"

#include "agentadaptor.h"
#include "bluemanager.h"
#include "blueadapter.h"
#include <QTimer>
#include <MInfoBanner>
#include <MDialog>
#include <MLabel>
#include <MTextEdit>
#include <QGraphicsLinearLayout>

BluetoothAgent::BluetoothAgent(QString path, QObject *parent):QObject(parent),requestAccepted(false)
{
	new AgentAdaptor(this);
	QDBusConnection::systemBus().registerObject(path,this);

	OrgBluezManagerInterface manager(
			"org.bluez",
			"/", QDBusConnection::systemBus());

	QDBusObjectPath adapterpath = manager.DefaultAdapter();

	OrgBluezAdapterInterface adapter(
			"org.bluez",
			adapterpath.path(),
			QDBusConnection::systemBus());

	adapter.RegisterAgent(QDBusObjectPath("/pairing/agent"),"");

	qDebug()<<"last error: "<<adapter.lastError().message();

}

void BluetoothAgent::authorize(OrgBluezDeviceInterface &device, QString uuid)
{
	Q_UNUSED(uuid);
	Q_UNUSED(device);
	qDebug("authorize");

/*	MDialog *banner = new MDialog(MInfoBanner::Event);

	banner->setBodyText("Authorize pair with: "+device.path());
	banner->setButtonText("Accept");
	banner->appear(MSceneWindow::DestroyWhenDone);*/
}

void BluetoothAgent::confirmModeChange(QString mode)
{
	qDebug()<<"mode changed "<<mode;
}

void BluetoothAgent::displayPasskey(OrgBluezDeviceInterface &device, uint key)
{
	qDebug()<<"display key "<<device.path()<<" "<<key;
}

void BluetoothAgent::release()
{
	qDebug("release called");
}

void BluetoothAgent::requestConfirmation(OrgBluezDeviceInterface &device, uint key)
{
	QVariantMap props = device.GetProperties();

	QString alias = props["Alias"].toString();

	//% "Bluetooth Pair"
	MDialog * dialog = new MDialog(qtTrId("Bluetooth Pair"),
				       M::YesButton | M::NoButton);

	//% "Pairing with %1 with key: %2" where %1 is the alias and %2 is the key
	dialog->setCentralWidget( new MLabel(qtTrId("Pairing with %1 with key: %2").arg(alias).arg(QString::number(key))));

	connect(dialog,SIGNAL(rejected()),this,SLOT(cancel()));

	dialog->exec();
}

uint BluetoothAgent::requestPasskey(OrgBluezDeviceInterface &device)
{
        Q_UNUSED(device);
	qDebug("requestKey");

	MWidget *centralWidget = new MWidget;
	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);

	QVariantMap props = device.GetProperties();

	//% "Please Enter PassKey on %1" where %1 is the bluetooth device alias.
	QString text = qtTrId("Please Enter PassKey on  %1:").arg(props["Alias"].toString());

	MLabel *label = new MLabel(text, centralWidget);
	MTextEdit *textEdit = new MTextEdit(MTextEditModel::SingleLine,
						QString(),
						centralWidget);
	centralWidget->setLayout(layout);

	layout->addItem(label);
	layout->addItem(textEdit);

	//% "Device Passkey"
	MDialog *dialog = new MDialog(qtTrId("Device Passkey"),
			       M::OkButton | M::CancelButton);

	dialog->setCentralWidget(centralWidget);

	dialog->exec();

	return (uint)textEdit->text().toInt();
}

QString BluetoothAgent::requestPidCode(OrgBluezDeviceInterface &device)
{
	Q_UNUSED(device);
	MWidget *centralWidget = new MWidget;
	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	QVariantMap props = device.GetProperties();

	//%"Please enter the PIN for %1:" where %1 is the device alias.
	QString text = qtTrId("Please enter the PIN for %1:").arg(props["Alias"].toString());

	MLabel *label = new MLabel(text, centralWidget);

	MTextEdit *textEdit = new MTextEdit(MTextEditModel::SingleLine,
						QString(),
						centralWidget);
	centralWidget->setLayout(layout);

	layout->addItem(label);
	layout->addItem(textEdit);

	//% "Device PIN Entry"
	MDialog *dialog = new MDialog(qtTrId("Device PIN Entry"),
			       M::OkButton | M::CancelButton);

	dialog->setCentralWidget(centralWidget);

	dialog->exec();

	return textEdit->text();
}

void BluetoothAgent::accept()
{
	requestAccepted = true;
}

void BluetoothAgent::doReject()
{
	if(requestAccepted)
	{
		requestAccepted=false;
		return;
	}
	reject();
}
