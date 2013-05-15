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


#ifndef BLUETOOTHVISIBILITYBUTTON_H
#define BLUETOOTHVISIBILITYBUTTON_H

#include <MButton>
#include <MWidget>
#include <MLabel>

#include "blueadapter.h"

class BluetoothVisibilityButton : public MWidget
{
Q_OBJECT
public:
	explicit BluetoothVisibilityButton(QGraphicsItem *parent = 0);

signals:

public slots:
	void toggleVisibility(bool);
	void propertyChanged(QString,QDBusVariant);
private:
	OrgBluezAdapterInterface *adapter;
	MButton* checkbox;
	MLabel * label;
};

#endif // BLUETOOTHVISIBILITYBUTTON_H
