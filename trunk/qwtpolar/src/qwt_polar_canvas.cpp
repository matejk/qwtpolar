/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QwtPolar Widget Library
 * Copyright (C) 2008   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_polar_canvas.h"
#include "qwt_polar_plot.h"
#include <qpainter.h>
#include <qevent.h>
#include <qpixmap.h>
#ifdef Q_WS_X11
#include <qx11info_x11.h>
#endif

class QwtPolarCanvas::PrivateData
{
public:
    PrivateData():
        paintAttributes( 0 ),
        cache( NULL )
    {
    }

    ~PrivateData()
    {
        delete cache;
    }

    int paintAttributes;
    QPixmap *cache;
};

//! Constructor
QwtPolarCanvas::QwtPolarCanvas( QwtPolarPlot *plot ):
    QFrame( plot )
{
    d_data = new PrivateData;

    setAutoFillBackground( false );

    setAttribute( Qt::WA_NoSystemBackground, true );
#ifndef QT_NO_CURSOR
    setCursor( Qt::CrossCursor );
#endif
    setFocusPolicy( Qt::WheelFocus );

    setPaintAttribute( PaintCached, true );
}

//! Destructor
QwtPolarCanvas::~QwtPolarCanvas()
{
    delete d_data;
}

//! Return parent plot widget
QwtPolarPlot *QwtPolarCanvas::plot()
{
    QWidget *w = parentWidget();
    if ( w && w->inherits( "QwtPolarPlot" ) )
        return ( QwtPolarPlot * )w;

    return NULL;
}

//! Return parent plot widget
const QwtPolarPlot *QwtPolarCanvas::plot() const
{
    const QWidget *w = parentWidget();
    if ( w && w->inherits( "QwtPolarPlot" ) )
        return ( QwtPolarPlot * )w;

    return NULL;
}

/*!
  \brief Changing the paint attributes

  \param attribute Paint attribute
  \param on On/Off

  The default setting enables PaintCached

  \sa testPaintAttribute(), drawCanvas(), drawContents(), paintCache()
*/
void QwtPolarCanvas::setPaintAttribute( PaintAttribute attribute, bool on )
{
    if ( bool( d_data->paintAttributes & attribute ) == on )
        return;

    if ( on )
        d_data->paintAttributes |= attribute;
    else
        d_data->paintAttributes &= ~attribute;

    switch( attribute )
    {
        case PaintCached:
        {
            if ( on )
            {
                if ( d_data->cache == NULL )
                    d_data->cache = new QPixmap();

                if ( isVisible() )
                {
                    const QRect cr = contentsRect();
                    *d_data->cache = QPixmap::grabWidget( this,
                                                          cr.x(), cr.y(), cr.width(), cr.height() );
                }
            }
            else
            {
                delete d_data->cache;
                d_data->cache = NULL;
            }
            break;
        }
    }
}

/*!
  Test wether a paint attribute is enabled

  \param attribute Paint attribute
  \return true if the attribute is enabled
  \sa setPaintAttribute()
*/
bool QwtPolarCanvas::testPaintAttribute( PaintAttribute attribute ) const
{
    return ( d_data->paintAttributes & attribute ) != 0;
}

//! Return the paint cache, might be null
QPixmap *QwtPolarCanvas::paintCache()
{
    return d_data->cache;
}

//! Return the paint cache, might be null
const QPixmap *QwtPolarCanvas::paintCache() const
{
    return d_data->cache;
}

//! Invalidate the internal paint cache
void QwtPolarCanvas::invalidatePaintCache()
{
    if ( d_data->cache )
        *d_data->cache = QPixmap();
}

//! Paint event
void QwtPolarCanvas::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );

    if ( !contentsRect().contains( event->rect() ) )
    {
        painter.save();
        painter.setClipRegion( event->region() & frameRect() );
        drawFrame( &painter );
        painter.restore();
    }

    painter.setClipRegion( event->region() & contentsRect() );

    drawContents( &painter );
}

//! Resize event
void QwtPolarCanvas::resizeEvent( QResizeEvent *event )
{
    QFrame::resizeEvent( event );

    for ( int scaleId = 0; scaleId < QwtPolar::ScaleCount; scaleId++ )
        plot()->updateScale( scaleId );
}

//! Redraw the canvas
void QwtPolarCanvas::drawContents( QPainter *painter )
{
    if ( d_data->paintAttributes & PaintCached && d_data->cache
            && d_data->cache->size() == contentsRect().size() )
    {
        painter->drawPixmap( contentsRect().topLeft(), *d_data->cache );
    }
    else
    {
        QwtPolarPlot *plt = plot();
        if ( plt )
        {
            const bool doAutoReplot = plt->autoReplot();
            plt->setAutoReplot( false );
            drawCanvas( painter, QRectF( contentsRect() ) );
            plt->setAutoReplot( doAutoReplot );
        }
    }
}

/*!
  Draw the the canvas

  Paints all plot items to the canvasRect, using QwtPolarPlot::drawCanvas
  and updates the paint cache.

  \sa QwtPolarPlot::drawCanvas, setPaintAttributes(), testPaintAttributes()
*/
void QwtPolarCanvas::drawCanvas( QPainter *painter,
    const QRectF& canvasRect )
{
    if ( !canvasRect.isValid() )
        return;

    if ( testPaintAttribute( PaintCached ) && d_data->cache )
    {
        *d_data->cache = QPixmap( contentsRect().size() );

#ifdef Q_WS_X11
        if ( d_data->cache->x11Info().screen() != x11Info().screen() )
            d_data->cache->x11SetScreen( x11Info().screen() );
#endif

        d_data->cache->fill( this, d_data->cache->rect().topLeft() );

        QPainter cachePainter( d_data->cache );
        cachePainter.translate( -contentsRect().x(),
                                -contentsRect().y() );

        plot()->drawCanvas( &cachePainter, canvasRect );

        cachePainter.end();

        painter->drawPixmap( canvasRect.topLeft().toPoint(), *d_data->cache );
    }
    else
        plot()->drawCanvas( painter, canvasRect );
}
