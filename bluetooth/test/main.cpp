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
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MApplication>
#include <MWidget>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButton>

#include "../src/bluetoothwidget.h"
#include "bluetoothtransferpage.h"

int main(int argc, char** argv)
{
	MApplication app(argc,argv);

	MApplicationWindow window;

	MApplicationPage mainpage;

	window.show();

	MWidget *widget = new MWidget();

	MLayout* layout = new MLayout(widget);

	MLinearLayoutPolicy* policy = new MLinearLayoutPolicy(layout,Qt::Vertical);

	widget->setLayout(layout);

	mainpage.setCentralWidget(widget);

	MApplicationPage bluetoothsettingspage;

	BluetoothTransferPage bluetoothtransferpage;

	bluetoothsettingspage.setCentralWidget(new BluetoothWidget());

	///add all the buttons and stuff:

	MButton* button1 = new MButton("Bluetooth Settings");
	policy->addItem(button1);
	QObject::connect(button1,SIGNAL(clicked()),
		&bluetoothsettingspage,
		SLOT(appear()));

	button1 = new MButton("Transfer");
	policy->addItem(button1);
	QObject::connect(button1,SIGNAL(clicked()),
		&bluetoothtransferpage,
		SLOT(appear()));


	mainpage.appear();

	app.exec();
}
