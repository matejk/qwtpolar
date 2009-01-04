/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QwtPolar Widget Library
 * Copyright (C) 2008   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include <qpainter.h>
#include "qwt_polar.h"
#include "qwt_scale_map.h"
#include "qwt_polar_spectrogram.h"

class QwtPolarSpectrogram::PrivateData
{
public:
    PrivateData()
    {
    }

    ~PrivateData()
    {
    }
};

QwtPolarSpectrogram::QwtPolarSpectrogram():
    QwtPolarItem(QwtText("Spectrogram"))
{
    d_data = new PrivateData;

    setItemAttribute(QwtPolarItem::AutoScale);
    setItemAttribute(QwtPolarItem::Legend, false);

    setZ(20.0);
}

//! Destructor
QwtPolarSpectrogram::~QwtPolarSpectrogram()
{
    delete d_data;
}

//! \return QwtPolarItem::Rtti_PolarSpectrogram
int QwtPolarSpectrogram::rtti() const
{
    return QwtPolarItem::Rtti_PolarSpectrogram;
}

void QwtPolarSpectrogram::draw(QPainter *painter,
    const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
    const QwtDoublePoint &pole, double /* radius */,
    const QwtDoubleRect & /* canvasRect */) const
{
}

/*! 
   Interval, that is necessary to display the item
   This interval can be useful for operations like clipping or autoscaling

   \param scaleId Scale index
   \return bounding interval ( == position )

   \sa position()
*/
QwtDoubleInterval QwtPolarSpectrogram::boundingInterval(int scaleId) const
{
    return QwtPolarItem::boundingInterval(scaleId);
}
