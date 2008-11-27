/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QwtPolar Widget Library
 * Copyright (C) 2008   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_POLAR_LAYOUT_H
#define QWT_POLAR_LAYOUT_H

#include "qwt_global.h"
#include "qwt_polar_plot.h"

/*!
  \brief Layout class for QwtPolarPlot.

  Organizes the geometry for the different QwtPolarPlot components.
*/

class QWT_POLAR_EXPORT QwtPolarLayout
{
public:
    enum Options
    {
        IgnoreScrollbars = 1,
        IgnoreFrames = 2,
        IgnoreTitle = 4,
        IgnoreLegend = 8
    };

    explicit QwtPolarLayout();
    virtual ~QwtPolarLayout();

    void setLegendPosition(QwtPolarPlot::LegendPosition pos, double ratio);
    void setLegendPosition(QwtPolarPlot::LegendPosition pos);
    QwtPolarPlot::LegendPosition legendPosition() const;

    void setLegendRatio(double ratio);
    double legendRatio() const;

    virtual QSize minimumSizeHint(const QwtPolarPlot *) const;    

    virtual void activate(const QwtPolarPlot *, 
        const QRect &rect, int options = 0);

    virtual void invalidate();

    const QRect &titleRect() const;
    const QRect &legendRect() const;
    const QRect &canvasRect() const;

    class LayoutData;

protected:
    QRect layoutLegend(int options, const QRect &) const;

private:
    class PrivateData;
    PrivateData *d_data;
};

#endif
