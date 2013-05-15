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

#include <QtGui>
#include <QDebug>
#include <MAction>
#include <MTheme>
#include <MLinearLayoutPolicy>
#include <MLayout>
#include <DcpWidget>
#include "bluetoothapplet.h"
#include "bluetoothwidget.h"
#include "bluetoothbrief.h"

Q_EXPORT_PLUGIN2(bluetoothapplet, BluetoothApplet)

void BluetoothApplet::init()
{
	MTheme::loadCSS(QString(CSSDIR) + "skeletonapplet.css");
};

DcpWidget* BluetoothApplet::constructWidget(int)
{
	return pageMain();
}

DcpWidget* BluetoothApplet::pageMain()
{
	DcpWidget* widget = new DcpWidget();

	MLayout *layout = new MLayout(widget);
	MLinearLayoutPolicy * policy = new MLinearLayoutPolicy(layout,Qt::Vertical);
	layout->setPolicy(policy);
	policy->addItem(new BluetoothWidget(widget));

	widget->setLayout(layout);

	return widget;
}



QString BluetoothApplet::title() const
{
    //% "Bluetooth applet"
    return qtTrId("dcp_bluetooth_appl_titl");
}

QVector<MAction*> BluetoothApplet::viewMenuItems()
{
   /* (1);
    //% "Example action"
    vector[0] = new MAction(qtTrId("dcp_skel_appl_action"), this);
    vector[0]->setLocation(MAction::ApplicationMenuLocation);*/

	return QVector<MAction*>();
}

DcpBrief* BluetoothApplet::constructBrief(int)
{
	return new BluetoothBrief();
}

