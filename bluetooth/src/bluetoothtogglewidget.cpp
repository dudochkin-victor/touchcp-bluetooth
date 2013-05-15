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


#include "bluetoothtogglewidget.h"

#include <QGraphicsLinearLayout>
#include <MGridLayoutPolicy>
#include <MLabel>
#include "technologybutton.h"
#include "networklistmodel.h"
BluetoothToggleWidget::BluetoothToggleWidget(MWidget* parent)
	:MWidget(parent)
{
	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(
			Qt::Horizontal, this);

	setLayout(layout);
	//layout->setContentsMargins(0, 0, 0, 0);
	TechnologyButton *button = new TechnologyButton("bluetooth", NetworkListModel::instance(), this);
	//% "Bluetooth"
	layout->addItem(new MLabel(qtTrId("Bluetooth"), this));
	layout->addItem(button);

	connect(button,SIGNAL(toggled(bool)),this,SIGNAL(toggled(bool)));
}
