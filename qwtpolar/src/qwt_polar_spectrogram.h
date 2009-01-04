/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QwtPolar Widget Library
 * Copyright (C) 2008   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_POLAR_SPECTROGRAM_H
#define QWT_POLAR_SPECTROGRAM_H

#include "qwt_polar_global.h"
#include "qwt_polar_item.h"

class QWT_POLAR_EXPORT QwtPolarSpectrogram: public QwtPolarItem
{
public:
    explicit QwtPolarSpectrogram();
    virtual ~QwtPolarSpectrogram();

    virtual int rtti() const;

    virtual void draw(QPainter *painter,
        const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
        const QwtDoublePoint &pole, double radius,
        const QwtDoubleRect &canvasRect) const;

    virtual QwtDoubleInterval boundingInterval(int scaleId) const;

private:
    class PrivateData;
    PrivateData *d_data;
};

#endif
