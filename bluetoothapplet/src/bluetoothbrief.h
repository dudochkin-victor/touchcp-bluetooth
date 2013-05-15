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

#ifndef BLUETOOTHBRIEF_H
#define BLUETOOTHBRIEF_H

#include <DcpBrief>

class BluetoothBrief: public DcpBrief{
    Q_OBJECT
public:
    BluetoothBrief();
    virtual QString valueText() const;
    virtual QString titleText() const;

    virtual bool toggle() const;
    virtual void setToggle (bool toggle);
    virtual int widgetTypeID() const;

private:
    bool m_ToggleState;
    int m_Value;
};


#endif // SKELETONBRIEF
