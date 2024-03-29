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

#include "pacontrol.h"
#include <string.h>
#include <QString>
#include <QList>
#include <QDebug>

// Define our pulse audio loop and connection variables
static PAControl* paControl = new PAControl;

static void pa_subscribed_events_cb(pa_context *c, enum pa_subscription_event_type t, uint32_t , void *);

static void operation_callback(pa_context *, int success, void *) {
    if (!success) {
        qDebug() << QString("Operation Failed");
        paControl->setErrorMsg(QString("Operation Failed"));
    }
}

static void module_callback(pa_context *, uint32_t index, void *) {

    if (index == PA_INVALID_INDEX) {
        qDebug() << QString("Load module failed");
        paControl->setErrorMsg(QString("Load module failed"));
    }
}



static void pa_sourcelist_cb(pa_context *, const pa_source_info *l, int is_last, void *) {
    PADevice *source;

    if (is_last > 0) {
        //end of the list
        return;
    }

    source = new PADevice();
    source->index = l->index;
    if(l->name != NULL)
	source->name = l->name;
    if(l->description != NULL)
	source->description = l->description;
    paControl->addSource(source);

}

static void pa_sinklist_cb(pa_context *, const pa_sink_info *l, int is_last, void *) {
    PADevice *sink;

    if (is_last > 0) {
        //end of the list
        return;
    }

    sink = new PADevice();
    sink->index = l->index;
    if(l->name != NULL)
	sink->name = l->name;
    if(l->description != NULL)
	sink->description = l->description;
    paControl->addSink(sink);

}

static void pa_modulelist_cb(pa_context *, const pa_module_info *i, int is_last, void *) {
    PAModule *module;

    if (is_last > 0) {
        //end of the list
        return;
    }

    module = new PAModule();
    module->index = i->index;
    if(i->name != NULL)
	module->name = i->name;
    if(i->argument != NULL)
	module->argument = i->argument;
    paControl->addModule(module);
}

static void pa_state_cb(pa_context *c, void *) {

	pa_context_state_t state = pa_context_get_state(c);
	if( state == PA_CONTEXT_READY)
	{
		paControl->setState(true);
		pa_context_set_subscribe_callback(c, pa_subscribed_events_cb, NULL);
		pa_operation *o;
		if (!(o = pa_context_subscribe(c, (pa_subscription_mask_t)
			(PA_SUBSCRIPTION_MASK_SINK|
			 PA_SUBSCRIPTION_MASK_SOURCE|
			 PA_SUBSCRIPTION_MASK_SINK_INPUT|
			 PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT), NULL, NULL))) {
			qWarning("pa_context_subscribe() failed");
		}
		if(o) pa_operation_unref(o);
		///Get an initial list of sinks, sources and modules.
		pa_context_get_sink_info_list(c, pa_sinklist_cb, NULL);
		pa_context_get_source_info_list(c, pa_sourcelist_cb, NULL);
		pa_context_get_module_info_list(c, pa_modulelist_cb, NULL);
	}
	else
	{
		qDebug()<<"PA state: "<<(int) state;
	}
}

static void pa_subscribed_events_cb(pa_context *c, enum pa_subscription_event_type t, uint32_t , void *)
{
	switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK)
	{
	    case PA_SUBSCRIPTION_EVENT_SINK:
		qDebug("PA_SUBSCRIPTION_EVENT_SINK event triggered");
		foreach(PADevice* dev, paControl->sinkList)
		{
			delete dev;
		}
		paControl->sinkList.clear();
		pa_context_get_sink_info_list(c, pa_sinklist_cb, NULL);
		break;
	    case PA_SUBSCRIPTION_EVENT_SOURCE:
		qDebug("PA_SUBSCRIPTION_EVENT_SOURCE event triggered");
		foreach(PADevice* dev, paControl->sourceList)
		{
			delete dev;
		}
		paControl->sourceList.clear();
		pa_context_get_source_info_list(c, pa_sourcelist_cb, NULL);
		break;
	    case PA_SUBSCRIPTION_EVENT_MODULE:
		qDebug("PA_SUBSCRIPTION_EVENT_MODULE event triggered");
		foreach(PAModule* dev, paControl->moduleList)
		{
			delete dev;
		}
		paControl->moduleList.clear();
		pa_context_get_module_info_list(c, pa_modulelist_cb, NULL);
		break;
	}
}

PAControl::PAControl(QObject *parent)
    :QObject(parent) {
    paControl = this;
    status = SUCCESS;
    m_paState=false;

    paInit();
}

PAControl::~PAControl()
{
    foreach (PADevice *source, sourceList) {
        qDebug() << QString("delete source");
        delete source;
    }
    foreach (PADevice *sink, sinkList) {
        qDebug() << QString("delete sink");
        delete sink;
    }
    foreach (PAModule *module, moduleList) {
        qDebug() << QString("delete module");
        delete module;
    }
    qDebug() << QString("~PAControl()");
}

PAControl* PAControl::instance()
{
	return paControl;
}

void PAControl::paInit() {
    // Create a mainloop API and connection to the default server
    pa_ml = pa_glib_mainloop_new(NULL);
    pa_ctx = pa_context_new(
		    pa_glib_mainloop_get_api(pa_ml),
		    "com.meego.pacontrol");

    // This function connects to the pulse server
    if (pa_context_connect(pa_ctx,
			   NULL,
			   PA_CONTEXT_NOFAIL, NULL) < 0)
    {
	    qCritical("PulseAudioService: pa_context_connect() failed");
	    paCleanup();
	    return;
    }

    pa_context_set_state_callback(pa_ctx, pa_state_cb, NULL);
}

void PAControl::paCleanup() {
    if(pa_ctx)
        pa_context_disconnect(pa_ctx);
    if(pa_ctx)
        pa_context_unref(pa_ctx);
    if(pa_ml)
	pa_glib_mainloop_free(pa_ml);
}

PADevice* PAControl::findBluezSource() {

    if (sourceList.size() == 0)
    {
	pa_op = pa_context_get_source_info_list(pa_ctx, pa_sourcelist_cb, NULL);
	if(pa_op) pa_operation_unref(pa_op);

	return NULL;
    }

    foreach (PADevice *source, sourceList) {
	QString name = source->name;

        if (name.startsWith(QString("bluez_source."), Qt::CaseSensitive)) {
            qDebug() << QString("   Matched Bluez source: ") << name;
            return source;
        }
    }

    qDebug() << QString("Bluez source: none found");
    return NULL;
}

PADevice*  PAControl::findBluezSink() {

    if (sinkList.size() == 0)
    {
	pa_op = pa_context_get_sink_info_list(pa_ctx, pa_sinklist_cb, NULL);
	if(pa_op) pa_operation_unref(pa_op);

	return NULL;
    }

    foreach (PADevice *sink, sinkList) {
	QString name = sink->name;

        if (name.startsWith(QString("bluez_sink."), Qt::CaseSensitive)) {
            qDebug() << QString("   Matched Bluez sink: ") << name;
            return sink;
        }
    }

    qDebug() << QString("Bluez sink: none found");
    return NULL;
}

PADevice* PAControl::findAlsaSource() {

    if (sourceList.size() == 0)
    {
	pa_op = pa_context_get_source_info_list(pa_ctx, pa_sourcelist_cb, NULL);
	if(pa_op) pa_operation_unref(pa_op);

	return NULL;
    }

    foreach (PADevice *source, sourceList) {
        qDebug() << QString("Alsa source: ") << source->name;
	QString name = source->name;

        if (name.startsWith(QString("alsa_input."), Qt::CaseSensitive) &&
            name.endsWith(QString("analog-stereo"), Qt::CaseSensitive) &&
            !name.contains(QString("timb"))) {
            qDebug() << QString("   Matched Alsa source: ") << name;
            return source;
        }
    }

    qDebug() << QString("Alsa source: none found");
    return NULL;
}

PADevice*  PAControl::findAlsaSink() {

    if (sinkList.size() == 0)
    {
	pa_op = pa_context_get_sink_info_list(pa_ctx, pa_sinklist_cb, NULL);
	if(pa_op) pa_operation_unref(pa_op);

	return NULL;
    }

    foreach (PADevice *sink, sinkList) {
        qDebug() << QString("Alsa sink: ") << sink->name;
	QString name = sink->name;

        if (name.startsWith(QString("alsa_output."), Qt::CaseSensitive) &&
            name.endsWith(QString("analog-stereo"), Qt::CaseSensitive) &&
            !name.contains(QString("timb"))) {
            qDebug() << QString("   Matched Alsa sink: ") << name;
            return sink;
        }
    }

    qDebug() << QString("Alsa sink: none found");
    return NULL;
}

PAModule*  PAControl::findLoopbackModule(PADevice* device) {
	///TODO: idk why a device is needed here?

    if (moduleList.size() == 0)
    {
	pa_op = pa_context_get_module_info_list(pa_ctx, pa_modulelist_cb, NULL);
	if(pa_op) pa_operation_unref(pa_op);

	return NULL;
    }

    foreach (PAModule *module, moduleList) {
	if (module->name.contains("module-loopback") && module->argument.contains(device->name)) {
            qDebug() << QString("   Matched module: ") << module->name;
            return module;
        }
    }

    qDebug() << QString("Module: none found");
    return NULL;
}

void PAControl::addSource(PADevice* device)
{
	bool notfound=false;
	foreach(PADevice* dev, sourceList)
	{
		if(dev->name == device->name)
		{
			//delete device;
			//return; /// already exists
			notfound = true;
			break;
		}
	}

	if(!notfound)
		sourceList.append(device);
	emit sourceAdded(device);
}

void PAControl::addSink(PADevice* device)
{
	foreach(PADevice* dev, sinkList)
	{
		if(dev->name == device->name)
		{
			delete device;
			return; /// already exists
		}
	}

	sinkList.append(device);
	emit sourceAdded(device);

}

void PAControl::addModule(PAModule *module)
{
	foreach(PAModule* dev, moduleList)
	{
		if(dev->name == module->name)
		{
			delete module;
			return; /// already exists
		}
	}

	moduleList.append(module);
}

void PAControl::routeSourceWithSink(PADevice *source, PADevice *sink) {

    if (source != NULL && sink != NULL) {
	QString arg = "source=\"%1\" sink=\"%2\"";
	arg.arg(source->name).arg(sink->name);

	pa_op = pa_context_load_module(pa_ctx, "module-loopback", arg.toAscii().data(), module_callback, NULL);

	if(pa_op) pa_operation_unref(pa_op);

        qDebug() << QString("load-module module-loopback ") << arg;        
    }
}

void PAControl::toggleMuteSource(PADevice *source, bool isMute) {

    if (source != NULL) {
	pa_operation_unref(pa_context_set_source_mute_by_name(pa_ctx, source->name.toAscii().data(), isMute, operation_callback, NULL));
    }
}

void  PAControl::unloadLoopback(PAModule* module) {

    if (module != NULL && module->index >= 0) {
	pa_operation_unref(pa_context_unload_module(pa_ctx, module->index, operation_callback, NULL));
        qDebug() << QString("unload-module module-loopback ") << module->index;
    }
}

PAStatus PAControl::getStatus() {
    return this->status;
}

void PAControl::setErrorMsg(QString msg) {
    if (msg != NULL)
    {
        this->status = ERROR;
        this->errorMsg = msg;
    }
}

QString PAControl::getErrorMsg() {
    return this->errorMsg;
}



