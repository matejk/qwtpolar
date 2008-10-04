/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QwtPolar Widget Library
 * Copyright (C) 2008   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_POLAR_FITTER_H
#define QWT_POLAR_FITTER_H

#include "qwt_global.h"
#include "qwt_curve_fitter.h"

/*!
  \brief A simple curve fitter for polar points
*/
class QWT_EXPORT QwtPolarFitter: public QwtCurveFitter
{
public:
    QwtPolarFitter(int stepCount = 5);
    virtual ~QwtPolarFitter();

    void setStepCount(int size);
    int stepCount() const;

#if QT_VERSION < 0x040000
    virtual QwtArray<QwtDoublePoint> fitCurve(
        const QwtArray<QwtDoublePoint> &) const;
#else
    virtual QPolygonF fitCurve(const QPolygonF &) const;
#endif

private:
    class PrivateData;
    PrivateData *d_data;
};

#endif
