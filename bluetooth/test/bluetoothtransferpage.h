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

#ifndef BLUETOOTHTRANSFERPAGE_H
#define BLUETOOTHTRANSFERPAGE_H

#include <MApplicationPage>
#include <MContentItem>
#include <MLinearLayoutPolicy>
#include <QDBusObjectPath>

class BluetoothTransferPage : public MApplicationPage
{
Q_OBJECT
public:
	explicit BluetoothTransferPage();

signals:

public slots:
	void deviceCreated(QDBusObjectPath);
	void deviceRemoved(QDBusObjectPath);
	void removeDevice(QString hwaddress);
	void deviceFound(QString hwaddress, QVariantMap properties);
	void deviceClicked();
private:
	QList<MContentItem*> items;
	MLinearLayoutPolicy* policy;

};

#endif // BLUETOOTHTRANSFERPAGE_H
