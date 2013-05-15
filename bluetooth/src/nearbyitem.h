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

#ifndef NEARBYITEM_H
#define NEARBYITEM_H

#include <QObject>
#include <MContentItem>
#include <MLabel>
#include <MAction>

class NearbyItem: public MContentItem
{
	Q_OBJECT
public:
	NearbyItem(QString text="", QString alias="", MWidget* parent=NULL);

public slots:
	void setText(QString text){ label->setText(text); }
	void pair();
	void browse();

private:
	MLabel* label;
	QString hwaddy;
	MAction* m_pair;
	MAction* m_browse;
};

#endif // NEARBYITEM_H
