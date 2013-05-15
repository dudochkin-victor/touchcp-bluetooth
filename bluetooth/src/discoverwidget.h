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

#ifndef DISCOVERWIDGET_H
#define DISCOVERWIDGET_H

#include <MWidget>

#include "blueadapter.h"

class MButton;
class MLabel;
class MProgressIndicator;
class MLinearLayoutPolicy;

class DiscoverWidget : public MWidget
{
Q_OBJECT
public:
	explicit DiscoverWidget(QGraphicsItem *parent = 0);

signals:

public slots:
	void toggleDiscovery(bool);
	void propertyChanged(QString, QDBusVariant);

private:
	MButton *checkbox;
	MLabel * label;
	OrgBluezAdapterInterface *adapter;
	MProgressIndicator * status;
	MLinearLayoutPolicy* policy;
	bool isDiscovering;
};

#endif // DISCOVERWIDGET_H
