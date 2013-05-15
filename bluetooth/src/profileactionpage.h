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

#ifndef PROFILEACTIONPAGE_H
#define PROFILEACTIONPAGE_H

#include <MButton>
#include <MWidget>
#include <MLinearLayoutPolicy>
#include <QMap>
#include <QDBusVariant>

#include "btprofiles.h"

class ProfileActionWidget : public MWidget
{
Q_OBJECT
public:
	explicit ProfileActionWidget(QString devicePath, bool noRemoveButton = false, MWidget* parent=NULL);

signals:
	void connectAudioClicked();
	void connectAudioSrcClicked();
	void connectHeadsetClicked();
	void connectHandsfreeClicked();
	void browseClicked();
	void connectPanClicked();
	void syncClicked();
	void refreshCapabilitiesClicked();
	void removeButtonClicked();
public slots:
	void enableProfile(QString profile);
	void propertyChanged(QString property, QDBusVariant value);
	void audioSinkPropertiesChanged(QString,QDBusVariant);
	void audioSourcePropertiesChanged(QString,QDBusVariant);
	void headsetPropertiesChanged(QString key,QDBusVariant val);
private:
	QMap<QString,QString> profilesMap;
	QMap<QString,MButton*> buttonMap;
	MLinearLayoutPolicy *profilePolicy;
	QString device;
};

#endif // PROFILEACTIONPAGE_H
