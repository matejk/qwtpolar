/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QwtPolar Widget Library
 * Copyright (C) 2008   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include <qpainter.h>
#include <qwt_scale_map.h>
#include <qwt_raster_data.h>
#include <qwt_color_map.h>
#include "qwt_polar.h"
#include "qwt_polar_plot.h"
#include "qwt_polar_spectrogram.h"
#if 1
#include <QDebug>
#endif

class QwtPolarSpectrogram::PrivateData
{
public:
    class DummyData: public QwtRasterData
    {
    public:
        virtual QwtRasterData *copy() const
        {
            return new DummyData();
        }

        virtual double value(double, double) const
        {
            return 0.0;
        }

        virtual QwtDoubleInterval range() const
        {
            return QwtDoubleInterval(0.0, 1.0);
        }
    };

    PrivateData()
    {
        data = new DummyData();
        colorMap = new QwtLinearColorMap();
    }

    ~PrivateData()
    {
    }

    QwtRasterData *data;
    QwtColorMap *colorMap;
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

/*!
  Set the data to be displayed

  \param data Spectrogram Data
  \sa data()
*/
void QwtPolarSpectrogram::setData(const QwtRasterData &data)
{
    delete d_data->data;
    d_data->data = data.copy();

    invalidateCache();
    itemChanged();
}

/*!
  \return Spectrogram data
  \sa setData()
*/
const QwtRasterData &QwtPolarSpectrogram::data() const
{
    return *d_data->data;
}

/*!
  Change the color map

  Often it is useful to display the mapping between intensities and
  colors as an additional plot axis, showing a color bar.

  \param colorMap Color Map

  \sa colorMap(), QwtScaleWidget::setColorBarEnabled(),
      QwtScaleWidget::setColorMap()
*/
void QwtPolarSpectrogram::setColorMap(const QwtColorMap &colorMap)
{
    delete d_data->colorMap;
    d_data->colorMap = colorMap.copy();
   
    invalidateCache();
    itemChanged();
}  

/*!
   \return Color Map used for mapping the intensity values to colors
   \sa setColorMap()
*/
const QwtColorMap &QwtPolarSpectrogram::colorMap() const
{
    return *d_data->colorMap;
}

void QwtPolarSpectrogram::draw(QPainter *painter,
    const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
    const QwtDoublePoint &pole, double,
    const QwtDoubleRect &canvasRect) const
{
	const QwtDoubleRect plotRect = plot()->plotRect(canvasRect.toRect());

	QRegion clipRegion(canvasRect.toRect());
	clipRegion &= QRegion(plotRect.toRect(), QRegion::Ellipse);

    QRect imageRect = canvasRect.toRect();

	const QwtDoubleInterval radialInterval = 
		boundingInterval(QwtPolar::ScaleRadius);
	if ( radialInterval.isValid() )
	{
		const double radius = radialMap.transform(radialInterval.maxValue()) -
			radialMap.transform(radialInterval.minValue());

		QwtDoubleRect r(0, 0, 2 * radius, 2 * radius);
		r.moveCenter(pole);

		clipRegion &= QRegion(r.toRect(), QRegion::Ellipse);;

		imageRect &= r.toRect();
	}
	
	const QImage image = renderImage(azimuthMap, radialMap, pole, imageRect);

	painter->save();
	painter->setClipRegion(clipRegion);

	painter->drawImage(imageRect, image);

	painter->restore();
}

QImage QwtPolarSpectrogram::renderImage(
        const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
		const QwtDoublePoint &pole, const QRect &rect) const
{
	QImage image(rect.size(), QImage::Format_ARGB32);
	image.fill(QColor(Qt::red).rgb());

    const QwtDoubleInterval intensityRange = d_data->data->range();
    if ( !intensityRange.isValid() )
        return image;

	for ( int y = rect.top(); y <= rect.bottom(); y++ )
	{
		const double dy = pole.y() - y;

		QRgb *line = (QRgb *)image.scanLine(y - rect.top());
		for ( int x = rect.left(); x <= rect.right(); x++ )
		{
			const double dx = x - pole.x();
			
			QwtPolarPoint pos(QwtDoublePoint(dx, dy));
			pos = pos.normalized();

			const double azimuth = azimuthMap.invTransform(pos.azimuth());
			const double radius = radialMap.invTransform(pos.radius());

			const double value = d_data->data->value(azimuth, radius);
			*line++ = d_data->colorMap->rgb(intensityRange, value);
		}
	}

	return image;
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
	if ( scaleId == QwtPolar::ScaleRadius )
	{
		const QwtDoubleRect boundingRect = d_data->data->boundingRect();
		if ( boundingRect.isValid() )
		{
			QwtDoubleInterval intv(boundingRect.top(), boundingRect.bottom());
			return intv.normalized();
		}
	}
    return QwtPolarItem::boundingInterval(scaleId);
}

/*!
   Invalidate the paint cache
   \sa setCachePolicy
*/
void QwtPolarSpectrogram::invalidateCache()
{
}
