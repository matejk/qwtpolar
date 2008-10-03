/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QwtPolar Widget Library
 * Copyright (C) 2008   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_POLAR_H
#define QWT_POLAR_H 1

#include "qwt_polar_global.h"

namespace QwtPolar
{
    enum Coordinate
    {
        Azimuth,
        Radius
    };

    enum Axis
    {
        AxisAzimuth,

        AxisLeft,
        AxisRight,
        AxisTop,
        AxisBottom,

        AxesCount
    };

    enum Scale
    {
        ScaleAzimuth = Azimuth,
        ScaleRadius = Radius,

        ScaleCount 
    };

};

#endif