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


#ifndef BLUETOOTHPAGE_H
#define BLUETOOTHPAGE_H

#include <MApplicationPage>

class BlueToothPage : public MApplicationPage
{
    Q_OBJECT

public:
    BlueToothPage(QGraphicsItem *parent = NULL);
    virtual ~BlueToothPage();

protected:
    virtual void createContent();

private:
};

#endif // BLUETOOTHPAGE_H
