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

#ifndef PAIREDITEM_H
#define PAIREDITEM_H

#include <MContentItem>
#include <QDBusObjectPath>

#include "bluedevice.h"

class PADevice;

class PairedItem: public MContentItem
{
	Q_OBJECT
public:
	PairedItem(QString text="", QGraphicsItem* parent=NULL);
	QString devicePath() { return device->path(); }

	bool connected() { return m_connected; }

	QString alias();
	QString address();
	bool hasProfile(QString);

public slots:
//	void setText(QString text){ label->setText(text); }
	void connectPanDevice();
	void browse();
	void connectAudio();
	void connectAudioSource();
	void connectHeadset();
	void initiateSync();
	void remove();
	void properyChanged(QString, QDBusVariant);
	void activateProfilePage();

	void getCapabilities();
	void paDeviceAdded(PADevice*);
signals:
	void connectedChanged();
	void removed();
	void profileAdded(QString);
	void deviceProbeFinished();
	void deviceProbeStarted();
private:
	bool parouted;
	bool m_connected;
	bool isProbing;
	QStringList profiles;
	void parseServices(QString xml);
	OrgBluezDeviceInterface* device;
};

#endif // PAIREDITEM_H
