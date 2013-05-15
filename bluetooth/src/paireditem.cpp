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

#include "paireditem.h"
#include "audiosink.h"
#include "audiosource.h"
#include "bluemanager.h"
#include "blueadapter.h"
#include "profileactionpage.h"
#include "headset.h"
#include "pacontrol.h"

#include <QXmlQuery>
#include <QDomDocument>
#include <MAction>
#include <MDialog>
#include <MLabel>
#include <MApplicationPage>

PairedItem::PairedItem(QString text, QGraphicsItem* parent):
		MContentItem(MContentItem::SingleTextLabel, parent),
		m_connected(false),
		isProbing(false)
{
	//setObjectName("NearbyItem");
	MAction * m_remove = new MAction("Remove", this);

//	label->setObjectName("PairedItem");
	device = new OrgBluezDeviceInterface("org.bluez",text,QDBusConnection::systemBus());

	m_remove->setLocation(MAction::ObjectMenuLocation);

	addAction(m_remove);

	qDBusRegisterMetaType<UIntStringMap>();

	//getCapabilities();

	//connect(this,SIGNAL(clicked()),this,SLOT(activateProfi()));

	connect(m_remove, SIGNAL(triggered()), this, SLOT(remove()));

	connect(device, SIGNAL(PropertyChanged(QString,QDBusVariant)),
		this, SLOT(properyChanged(QString,QDBusVariant)));

	QVariantMap props = device->GetProperties();

	setTitle(props["Alias"].toString());
//	const QPixmap *foo = MTheme::pixmap(props["Icon"].toString());
//	qDebug()<<"icon: "<<props["Icon"].toString();
//	setPixmap(*foo);
//	delete foo;
}

QString PairedItem::alias()
{
	QVariantMap props = device->GetProperties();
	return props["Alias"].toString();
}
QString PairedItem::address()
{
	QVariantMap props = device->GetProperties();
	return props["Address"].toString();
}

bool PairedItem::hasProfile(QString profile)
{
	QVariantMap props = device->GetProperties();

	QStringList uuidlist = props["UUIDs"].toStringList();

	foreach(QString uuid, uuidlist)
	{
		if(uuid == profile)
			return true;
	}

	return false;
}

void PairedItem::connectPanDevice()
{
	qDebug("connecting to device...");

}

void PairedItem::browse()
{
	/// TODO: not sure who will be doing the ftp browsing bits
}

void PairedItem::connectAudio()
{
	OrgBluezAudioSinkInterface interface("org.bluez",
				 device->path(), QDBusConnection::systemBus());

	if(!interface.isValid())
	{
		qCritical()<<"Error: "<<interface.lastError().message();
		return;
	}

	bool isConnected = interface.IsConnected();

	if(isConnected)
	{
		interface.Disconnect();
	}
	else interface.Connect();

	if(interface.lastError().isValid())
		qCritical()<<"Error: "<<interface.lastError().message();
}

void PairedItem::connectAudioSource()
{
	OrgBluezAudioSourceInterface interface("org.bluez",
				 device->path(), QDBusConnection::systemBus());

	if(!interface.isValid())
	{
		qCritical()<<"Error: "<<interface.lastError().message();
		return;
	}

	QVariantMap props = interface.GetProperties();

	if(props["State"] == "disconnected")
	{
		interface.Connect();

		parouted = false;

		PAControl* paControl = PAControl::instance();

		connect(paControl,SIGNAL(sourceAdded(PADevice*)),this,SLOT(paDeviceAdded(PADevice*)));
	}
	else interface.Disconnect();

	if(interface.lastError().isValid())
		qCritical()<<"Error: "<<interface.lastError().message();


	/// TODO: set up pulse to use the device


}

void PairedItem::connectHeadset()
{
	OrgBluezHeadsetInterface interface("org.bluez",
					   device->path(), QDBusConnection::systemBus());

	if(!interface.isValid())
	{
		qCritical()<<"Error: "<<interface.lastError().message();
		return;
	}

	QVariantMap props = interface.GetProperties();

	if(props["State"] == "disconnected")
	{
		interface.Connect();
	}
	else interface.Disconnect();

	if(interface.lastError().isValid())
		qCritical()<<"Error: "<<interface.lastError().message();
}

void PairedItem::initiateSync()
{
	///FIXME:

	/*QVariantMap properties = device->GetProperties();
	QString mac = properties["Address"].toString();
	BluetoothSyncPage *syncPage = new BluetoothSyncPage(mac);
	syncPage->appear(MSceneWindow::DestroyWhenDone);*/
}

void PairedItem::remove()
{
	MDialog* dialog = new MDialog("Remove device",
				      M::CancelButton| M::OkButton);

	MLabel* dialogDialog = new MLabel(this);
	dialogDialog->setText(QString("Do you want to remove %1?").arg(title()));

	dialog->setCentralWidget(dialogDialog);

	int status = dialog->exec();

	if (status != MDialog::Accepted)
		return;

	qDebug()<<"removing device: "<<device->path();
	OrgBluezManagerInterface manager(
			"org.bluez",
			"/", QDBusConnection::systemBus());

	QDBusObjectPath adapterpath = manager.DefaultAdapter();

	OrgBluezAdapterInterface adapter(
			"org.bluez",
			adapterpath.path(),
			QDBusConnection::systemBus());

	adapter.RemoveDevice(QDBusObjectPath(device->path()));

	emit removed();

	//qDebug("removed");

}

void PairedItem::properyChanged(QString property, QDBusVariant value)
{
	qDebug()<<"Property on device changed: "<<property<<" "<<value.variant();

	if(property == "Connected")
	{
		if(value.variant().toBool() != m_connected)
		{
			m_connected = value.variant().toBool();
			emit connectedChanged();
		}
	}
	/*else if(property == "UUIDs")
	{
		QStringList uuidlist = value.variant().toStringList();

		foreach(QString uuid, uuidlist)
		{
			profiles<<uuid;
			emit profileAdded(uuid);
		}
	}*/
	else if(property == "Paired" && value.variant().toBool())
	{
		activateProfilePage();
	}
}

void PairedItem::activateProfilePage()
{
	ProfileActionWidget *widget = new ProfileActionWidget(device->path(), false, this);

	MApplicationPage *page = new MApplicationPage();

	page->setCentralWidget(widget);

	page->appear(MSceneWindow::DestroyWhenDone);
	// page->setEscapeButtonMode(MEscapeButtonPanelModel::BackMode);

	connect(page, SIGNAL(backButtonClicked()), page, SLOT(dismiss()));
	connect(widget, SIGNAL(connectAudioClicked()),this,SLOT(connectAudio()));
	connect(widget, SIGNAL(connectPanClicked()),
		this, SLOT(connectPanDevice()));
	connect(widget, SIGNAL(syncClicked()),this,SLOT(initiateSync()));
	connect(widget, SIGNAL(removeButtonClicked()), this, SLOT(remove()));
	connect(widget, SIGNAL(removeButtonClicked()), page, SLOT(dismiss()));
	connect(widget, SIGNAL(connectAudioSrcClicked()), this, SLOT(connectAudioSource()));
	connect(widget, SIGNAL(connectHeadsetClicked()), this, SLOT(connectHeadset()));
}

void PairedItem::getCapabilities()
{
	QDBusPendingReply<UIntStringMap> call = device->DiscoverServices("");

	QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);

	connect(watcher,SIGNAL(finished(QDBusPendingCallWatcher*)),
		this,
		SLOT(callFinishedSlot(QDBusPendingCallWatcher*)));

	isProbing = true;
	emit deviceProbeStarted();
}

void PairedItem::paDeviceAdded(PADevice* )
{
	if(parouted)return;
	PAControl *paControl = PAControl::instance();
	PADevice* bluezdevice = paControl->findBluezSource();
	if(bluezdevice == NULL)
	{
//		Q_ASSERT(0); ///die!!!
		return;
	}

	///Check to see if we have a bluez sink:
	PADevice* bluezsink = paControl->findBluezSink();
	if(bluezsink)
	{
		PAModule* module = paControl->findLoopbackModule(bluezdevice);
		if(module) paControl->unloadLoopback(module);

		paControl->routeSourceWithSink(bluezdevice,bluezsink);
		parouted = true;
	}
	else
	{
		PADevice* alsa = paControl->findAlsaSink();
		if(alsa)
		{
			//PAModule* module = paControl->findLoopbackModule(bluezdevice);
			//if(module) paControl->unloadLoopback(module);

			paControl->routeSourceWithSink(bluezdevice,alsa);
			parouted = true;
		}
	}

}
