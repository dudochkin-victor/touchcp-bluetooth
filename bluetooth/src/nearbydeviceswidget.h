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

#ifndef NEARBYDEVICESWIDGET_H
#define NEARBYDEVICESWIDGET_H

#include <MAbstractCellCreator>
#include <MLinearLayoutPolicy>
#include <MAction>
#include <QAbstractListModel>
#include <QList>
#include <QStringList>
#include "bluemanager.h"
#include "blueadapter.h"
#include "nearbyitem.h"

class NearbyDevicesWidget : public MWidget
{
	Q_OBJECT
public:
	NearbyDevicesWidget(MWidget* parent=NULL);
	virtual ~NearbyDevicesWidget();
	void init();

public slots:
	void removeAll(bool);

private slots:
	void deviceFound(QString ,QVariantMap);
	void deviceDisappeared(QString);
	void hookUpWithAdapter(QDBusObjectPath);
	void adapterRemoved(QDBusObjectPath);

private:
	OrgBluezManagerInterface *manager;
	MLinearLayoutPolicy* policy;
	OrgBluezAdapterInterface *adapter;
	QStringList devicepathlist;
};

#endif // NEARBYDEVICESWIDGET_H
