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

#ifndef PACONTROL_H
#define PACONTROL_H
#include <pulse/context.h>
#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>
#include <QObject>

class PADevice {
public:
    QString name;
    int index;
    QString description;
};

class PAModule {
public:
    QString name;
    int index;
    QString argument;
};

enum PAStatus {
    SUCCESS = 0,
    ERROR = 1,
};

class PAControl : public QObject
{
    Q_OBJECT
public:
    PAControl(QObject *parent = 0);
    ~PAControl();

    static PAControl* instance();

    PADevice* findBluezSource();
    PADevice* findBluezSink();
    PADevice* findAlsaSource();
    PADevice* findAlsaSink();
    PAModule* findLoopbackModule(PADevice *device);
    void routeSourceWithSink(PADevice *source, PADevice *sink);

    void unloadLoopback(PAModule* module);
    void toggleMuteSource(PADevice *source, bool isMute);

    PAStatus getStatus();
    void setErrorMsg(QString msg);
    QString getErrorMsg();

    void setState(bool state) { m_paState = state; }

    void addSource(PADevice* device);
    void addSink(PADevice* device);
    void addModule(PAModule* module);

    QList<PADevice*> sourceList;
    QList<PADevice*> sinkList;
    QList<PAModule*> moduleList;

signals:
    void sourceAdded(PADevice* device);
    void sinkAdded(PADevice* device);
    void sourceRemoved(PADevice* device);
    void sinkRemoved(PADevice* device);

private:
    pa_glib_mainloop *pa_ml;


    pa_mainloop_api *pa_mlapi;
    pa_operation *pa_op;
    pa_context *pa_ctx;
    PAStatus status;
    QString errorMsg;

    void paInit();
    void paCleanup();


    bool m_paState;

};

#endif // PACONTROL_H
