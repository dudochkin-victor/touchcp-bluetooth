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

#ifndef FTPTRANSFERWIDGET_H
#define FTPTRANSFERWIDGET_H

#include <MWidget>
#include <MProgressIndicator>
#include <MLabel>

#include "obex/obexftp.h"

class FtpTransferWidget : public MWidget
{
Q_OBJECT
public:
	explicit FtpTransferWidget(QString device, MWidget *parent = 0);

signals:
	void complete();
	void connected();

public slots:
	void transferFile(QString file, QString targetfile="");
	void cancel();

	void transferComplete();
	void transferProgress(quint64 done, quint64 total);

protected slots:
	void error(QString err);
	void makeTransfer();

private:

	QString m_file;
	QString m_remotefile;
	ObexFtp* bluetoothftp;
	MProgressIndicator *progressSlider;
	MLabel *statusLabel;
	MLabel *stateLabel;
};

#endif // FTPTRANSFERWIDGET_H
