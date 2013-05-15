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

#include "ftptransferwidget.h"

#include <MLayout>
#include <MLinearLayoutPolicy>

FtpTransferWidget::FtpTransferWidget(QString device, MWidget *parent) : MWidget(parent)
{
	MLayout* layout = new MLayout(this);

	MLinearLayoutPolicy* policy = new MLinearLayoutPolicy(layout,
							      Qt::Vertical);
	layout->setPolicy(policy);

	setLayout(layout);

	progressSlider = new MProgressIndicator(this, MProgressIndicator::barType);
	progressSlider->setRange(0, 100);
	connect(progressSlider, SIGNAL(valueChanged(int)), SLOT(valueChangedSlot(int)));

	statusLabel = new MLabel(this);
	stateLabel = new MLabel(this);

	policy->addItem(statusLabel);
	policy->addItem(progressSlider);
	policy->addItem(stateLabel);

	bluetoothftp = new ObexFtp(device,this);

	connect(bluetoothftp,SIGNAL(error(QString)),statusLabel,SLOT(setText(QString)));
	connect(bluetoothftp,SIGNAL(error(QString)),this,SLOT(error(QString)));

	connect(bluetoothftp,SIGNAL(transferCompleted()),
		this, SLOT(transferComplete()));

	connect(bluetoothftp, SIGNAL(transferProgress(quint64,quint64)),
		this, SLOT(transferProgress(quint64,quint64)));

	/*connect(bluetoothftp, SIGNAL(error(OBEXFTP::Error)),
		this, SLOT(error(OBEXFTP::Error)));
	connect(bluetoothftp, SIGNAL(stateChanged(OBEXFTP::State,OBEXFTP::State)),
		this, SLOT(stateChanged(OBEXFTP::State,OBEXFTP::State)));*/

	connect(this, SIGNAL(connected()),this, SLOT(start()));
}

void FtpTransferWidget::transferFile(QString file, QString targetfile)
{
	m_file = file;
	m_remotefile = targetfile == "" ? file:targetfile;

	bluetoothftp->connect();
	connect(bluetoothftp,SIGNAL(connected()),this,SLOT(makeTransfer()));


	//% "Connecting..."
	stateLabel->setText(qtTrId("Connecting..."));
}

void FtpTransferWidget::cancel()
{
	bluetoothftp->cancel();
}

void FtpTransferWidget::transferComplete()
{
	bluetoothftp->disconnect();
	//% "Transfered"
	stateLabel->setText(qtTrId("Transfered"));
	//% "Transfer Complete"
	statusLabel->setText(qtTrId("Transfer Complete"));
	emit complete();
}

void FtpTransferWidget::transferProgress(quint64 done, quint64 total)
{
	if(total == 0 ) return;
	progressSlider->setValue(done / total * 100);
}

void FtpTransferWidget::error(QString err)
{
	qDebug()<<"Error: "<<err;
	//% "Transfer Failed"
	stateLabel->setText(qtTrId("Transfer Failed"));
}

void FtpTransferWidget::makeTransfer()
{
	//% Transferring...
	stateLabel->setText(qtTrId("Transferring..."));
	bluetoothftp->transferFile(m_file, m_remotefile);
}
