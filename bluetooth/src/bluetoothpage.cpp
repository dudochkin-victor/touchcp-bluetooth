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



#include <MLabel>

#include "bluetoothpage.h"
#include "bluetoothwidget.h"

#include "bluetoothagent.h"
#include "blueadapter.h"
#include "bluemanager.h"

BlueToothPage::BlueToothPage(QGraphicsItem *parent) :
        MApplicationPage(parent)
{
	new BluetoothAgent();
}

BlueToothPage::~BlueToothPage()
{
}

void BlueToothPage::createContent()
{
   MApplicationPage::createContent();
   setPannable(true);
   setContentsMargins(0,0,0,0);
   setAutoMarginsForComponentsEnabled(true);
   //% "Bluetooth"
   setTitle(qtTrId("BlueTooth"));
   setCentralWidget(new BluetoothWidget());
}

