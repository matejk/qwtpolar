/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QwtPolar Widget Library
 * Copyright (C) 2008   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_polar_spectrogram.h"
#include "qwt_polar.h"
#include "qwt_polar_plot.h"
#include <qwt_color_map.h>
#include <qwt_scale_map.h>
#include <qwt_raster_data.h>
#include <qpainter.h>

static bool qwtNeedsClipping( const QRectF &plotRect, const QRectF &rect )
{
    QPointF points[4];
    points[0] = rect.topLeft();
    points[1] = rect.topRight();
    points[2] = rect.bottomLeft();
    points[3] = rect.bottomRight();

    const double radius = plotRect.width() / 2.0;
    const QPointF pole = plotRect.center();

    for ( int i = 0; i < 4; i++ )
    {
        const double dx = points[i].x() - pole.x();
        const double dy = points[i].y() - pole.y();

        if ( qSqrt( dx * dx + dy * dy ) > radius )
            return true;
    }

    return false;
}

class QwtPolarSpectrogram::PrivateData
{
public:
    PrivateData():
        data( NULL )
    {
        colorMap = new QwtLinearColorMap();
    }

    ~PrivateData()
    {
    }

    QwtRasterData *data;
    QwtColorMap *colorMap;
};

//!  Constructor
QwtPolarSpectrogram::QwtPolarSpectrogram():
    QwtPolarItem( QwtText( "Spectrogram" ) )
{
    d_data = new PrivateData;

    setItemAttribute( QwtPolarItem::AutoScale );
    setItemAttribute( QwtPolarItem::Legend, false );

    setZ( 20.0 );
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

  \warning QwtRasterData::initRaster() is called each time before the
           image is rendered, but without any useful parameters.
           Also QwtRasterData::rasterHint() is not used.
*/
void QwtPolarSpectrogram::setData( QwtRasterData *data )
{
    if ( data != d_data->data )
    {
        delete d_data->data;
        d_data->data = data;

        itemChanged();
    }
}

/*!
  \return Spectrogram data
  \sa setData()
*/
const QwtRasterData *QwtPolarSpectrogram::data() const
{
    return d_data->data;
}

/*!
  Change the color map

  Often it is useful to display the mapping between intensities and
  colors as an additional plot axis, showing a color bar.

  \param colorMap Color Map

  \sa colorMap(), QwtScaleWidget::setColorBarEnabled(),
      QwtScaleWidget::setColorMap()
*/
void QwtPolarSpectrogram::setColorMap( QwtColorMap *colorMap )
{
    if ( d_data->colorMap != colorMap )
    {
        delete d_data->colorMap;
        d_data->colorMap = colorMap;
    }

    itemChanged();
}

/*!
   \return Color Map used for mapping the intensity values to colors
   \sa setColorMap()
*/
const QwtColorMap *QwtPolarSpectrogram::colorMap() const
{
    return d_data->colorMap;
}

/*!
  Draw the spectrogram

  \param painter Painter
  \param azimuthMap Maps azimuth values to values related to 0.0, M_2PI
  \param radialMap Maps radius values into painter coordinates.
  \param pole Position of the pole in painter coordinates
  \param radius Radius of the complete plot area in painter coordinates
  \param canvasRect Contents rect of the canvas in painter coordinates
*/
void QwtPolarSpectrogram::draw( QPainter *painter,
    const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
    const QPointF &pole, double,
    const QRectF &canvasRect ) const
{
    const QRectF plotRect = plot()->plotRect( canvasRect.toRect() );

    QRegion clipRegion( canvasRect.toRect() );
    if ( qwtNeedsClipping( plotRect, canvasRect ) )
    {
        // For large plotRects the ellipse becomes a huge polygon.
        // So we better clip only, when we really need to.

        clipRegion &= QRegion( plotRect.toRect(), QRegion::Ellipse );
    }

    QRect imageRect = canvasRect.toRect();

    const QwtInterval radialInterval =
        boundingInterval( QwtPolar::ScaleRadius );
    if ( radialInterval.isValid() )
    {
        const double radius = radialMap.transform( radialInterval.maxValue() ) -
                              radialMap.transform( radialInterval.minValue() );

        QRectF r( 0, 0, 2 * radius, 2 * radius );
        r.moveCenter( pole );

        clipRegion &= QRegion( r.toRect(), QRegion::Ellipse );;

        imageRect &= r.toRect();
    }

    const QImage image = renderImage( azimuthMap, radialMap, pole, imageRect );

    painter->save();
    painter->setClipRegion( clipRegion );

    painter->drawImage( imageRect, image );

    painter->restore();
}

/*!
   \brief Render an image from the data and color map.

   The area is translated into a rect of the paint device.
   For each pixel of this rect the intensity is mapped
   into a color.

  \param azimuthMap Maps azimuth values to values related to 0.0, M_2PI
  \param radialMap Maps radius values into painter coordinates.
  \param pole Position of the pole in painter coordinates
  \param rect Target rectangle of the image in painter coordinates

   \return A QImage::Format_Indexed8 or QImage::Format_ARGB32 depending
           on the color map.

   \sa QwtRasterData::intensity(), QwtColorMap::rgb(),
       QwtColorMap::colorIndex()
*/
QImage QwtPolarSpectrogram::renderImage(
    const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
    const QPointF &pole, const QRect &rect ) const
{
    if ( d_data->data == NULL || d_data->colorMap == NULL )
        return QImage();

    QImage image( rect.size(), d_data->colorMap->format() == QwtColorMap::RGB
                  ? QImage::Format_ARGB32 : QImage::Format_Indexed8 );

    const QwtInterval intensityRange = d_data->data->interval( Qt::ZAxis );
    if ( !intensityRange.isValid() )
        return image;

    /*
     For the moment we only announce the composition of the image by
     calling initRaster(), but we don't pass any useful parameters.
     ( How to map rect into something, that is useful to initialize a matrix
       of values in polar coordinates ? )
     */
    d_data->data->initRaster( QRectF(), QSize() );

    // Now we can collect the values and calculate the colors
    // using the color map.

    if ( d_data->colorMap->format() == QwtColorMap::RGB )
    {
        for ( int y = rect.top(); y <= rect.bottom(); y++ )
        {
            const double dy = pole.y() - y;
            const double dy2 = qwtSqr( dy );

            QRgb *line = ( QRgb * )image.scanLine( y - rect.top() );
            for ( int x = rect.left(); x <= rect.right(); x++ )
            {
                const double dx = x - pole.x();

                double a = qAtan2( dy, dx );
                if ( a < 0.0 )
                    a += 2 * M_PI;
                if ( a < azimuthMap.p1() )
                    a += 2 * M_PI;

                const double r = qSqrt( qwtSqr( dx ) + dy2 );

                const double azimuth = azimuthMap.invTransform( a );
                const double radius = radialMap.invTransform( r );

                const double value = d_data->data->value( azimuth, radius );
                *line++ = d_data->colorMap->rgb( intensityRange, value );
            }
        }
    }
    else if ( d_data->colorMap->format() == QwtColorMap::Indexed )
    {
        image.setColorTable( d_data->colorMap->colorTable( intensityRange ) );

        for ( int y = rect.top(); y <= rect.bottom(); y++ )
        {
            const double dy = pole.y() - y;
            const double dy2 = qwtSqr( dy );

            unsigned char *line = image.scanLine( y - rect.top() );
            for ( int x = rect.left(); x <= rect.right(); x++ )
            {
                const double dx = x - pole.x();

                double a = qAtan2( dy, dx );
                if ( a < 0.0 )
                    a += 2 * M_PI;

                const double r = qSqrt( qwtSqr( dx ) + dy2 );

                const double azimuth = azimuthMap.invTransform( a );
                const double radius = radialMap.invTransform( r );

                const double value = d_data->data->value( azimuth, radius );
                *line++ = d_data->colorMap->colorIndex( intensityRange, value );
            }
        }
    }

    d_data->data->discardRaster();

    return image;
}

/*!
   Interval, that is necessary to display the item
   This interval can be useful for operations like clipping or autoscaling

   \param scaleId Scale index
   \return bounding interval ( == position )

   \sa position()
*/
QwtInterval QwtPolarSpectrogram::boundingInterval( int scaleId ) const
{
    if ( scaleId == QwtPolar::ScaleRadius )
        return d_data->data->interval( Qt::YAxis );

    return QwtPolarItem::boundingInterval( scaleId );
}
