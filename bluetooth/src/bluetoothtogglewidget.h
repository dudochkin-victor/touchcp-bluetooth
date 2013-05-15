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


#ifndef BLUETOOTHTOGGLEWIDGET_H
#define BLUETOOTHTOGGLEWIDGET_H

#include <MWidget>

class BluetoothToggleWidget : public MWidget
{
	Q_OBJECT
public:
	BluetoothToggleWidget(MWidget* parent=NULL);

signals:
	void toggled(bool);
};

#endif // BLUETOOTHTOGGLEWIDGET_H
