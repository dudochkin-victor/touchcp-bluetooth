/*  -*- Mode: C++ -*-
 *
 * meego handset bluetooth applet
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */


#include <DcpWidgetTypes>
#include "bluetoothbrief.h"
#include <QtDebug>

BluetoothBrief::BluetoothBrief()
{

}

QString BluetoothBrief::valueText() const
{
    return "";
}

QString BluetoothBrief::titleText() const
{
    return "Bluetooth Devices";
}

bool BluetoothBrief::toggle() const
{
    return m_ToggleState;
}

void BluetoothBrief::setToggle (bool toggle)
{
    m_ToggleState = toggle;
    qDebug() << "bluetooth brief got toggle state:" << toggle;
}

int BluetoothBrief::widgetTypeID() const
{
    return DCPLABELBUTTON;
}

