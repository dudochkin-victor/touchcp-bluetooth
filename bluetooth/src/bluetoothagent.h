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


#ifndef BLUETOOTHAGENT_H
#define BLUETOOTHAGENT_H

#include <QObject>
#include <QDBusContext>
#include "bluedevice.h"

class BluetoothAgent : public QObject, public QDBusContext
{
	Q_OBJECT
public:
	BluetoothAgent(QString path="/pairing/agent", QObject *parent=0);
public slots:
	void authorize(OrgBluezDeviceInterface &device, QString);
	void cancel()
	{
		sendErrorReply("org.bluez.Error.Canceled","The request was canceled");
	}
	void confirmModeChange(QString);
	void displayPasskey(OrgBluezDeviceInterface &device, uint key);
	void release();
	void requestConfirmation(OrgBluezDeviceInterface &device, uint key);
	uint requestPasskey(OrgBluezDeviceInterface &device);
	QString requestPidCode(OrgBluezDeviceInterface &device);

	void reject() { sendErrorReply("org.bluez.Error.Rejected", "The request was rejected"); }

	void accept();
	void doReject();
private:
	bool requestAccepted;
};

#endif // BLUETOOTHAGENT_H
