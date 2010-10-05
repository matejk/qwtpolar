/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QwtPolar Widget Library
 * Copyright (C) 2008   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4786 )
#endif

#include <qglobal.h>
#include <qvaluelist.h>
#include <qmime.h>
#include <qdragobject.h>

#include "qwt_polar_plugin.h"
#include "qwt_polar_plot.h"

QWidget* QwtPolarPlugin::create(const QString &, 
    QWidget* parent, const char* name)
{
    QwtPolarPlot *plot = new QwtPolarPlot(parent);
    plot->setName(name);

    return plot;
}

QStringList QwtPolarPlugin::keys() const
{
    QStringList list;
	list += "QwtPolarPlot";
    return list;
}

QString QwtPolarPlugin::group( const QString&) const
{
	return QString("QwtPolar"); 
}

QIconSet QwtPolarPlugin::iconSet( const QString&) const
{
    const QMimeSource *ms =
        QMimeSourceFactory::defaultFactory()->data("qwt_polar_plot.png");

    QPixmap pixmap;
    QImageDrag::decode(ms, pixmap);

    return QIconSet(pixmap);
}

QString QwtPolarPlugin::includeFile(const QString&) const
{
	return QString("qwt_plot_plot.h");
}

QString QwtPolarPlugin::toolTip( const QString&) const
{
	return QString("QwtPolarPlot");
}

QString QwtPolarPlugin::whatsThis( const QString&) const
{
	return QString("QwtPolarPlot");
}

bool QwtPolarPlugin::isContainer( const QString& ) const
{
    return false;
}

Q_EXPORT_PLUGIN( QwtPolarPlugin )
