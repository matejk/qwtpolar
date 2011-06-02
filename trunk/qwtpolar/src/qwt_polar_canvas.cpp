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
        backingStore( NULL )
    {
    }

    ~PrivateData()
    {
        delete backingStore;
    }

    QwtPolarCanvas::PaintAttributes paintAttributes;
    QPixmap *backingStore;
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

    setPaintAttribute( BackingStore, true );
}

//! Destructor
QwtPolarCanvas::~QwtPolarCanvas()
{
    delete d_data;
}

//! \return Parent plot widget
QwtPolarPlot *QwtPolarCanvas::plot()
{
    QWidget *w = parentWidget();
    if ( w && w->inherits( "QwtPolarPlot" ) )
        return ( QwtPolarPlot * )w;

    return NULL;
}

//! \return Parent plot widget
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
        case BackingStore:
        {
            if ( on )
            {
                if ( d_data->backingStore == NULL )
                    d_data->backingStore = new QPixmap();

                if ( isVisible() )
                {
                    const QRect cr = contentsRect();
                    *d_data->backingStore = QPixmap::grabWidget( this, cr );
                }
            }
            else
            {
                delete d_data->backingStore;
                d_data->backingStore = NULL;
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

//! \return Backing store, might be null
const QPixmap *QwtPolarCanvas::backingStore() const
{
    return d_data->backingStore;
}

//! Invalidate the internal backing store
void QwtPolarCanvas::invalidateBackingStore()
{
    if ( d_data->backingStore )
        *d_data->backingStore = QPixmap();
}

/*!
  Paint event
  \param event Paint event
*/
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

/*!
  Resize event
  \param event Resize event
*/
void QwtPolarCanvas::resizeEvent( QResizeEvent *event )
{
    QFrame::resizeEvent( event );

    for ( int scaleId = 0; scaleId < QwtPolar::ScaleCount; scaleId++ )
        plot()->updateScale( scaleId );
}

//! Redraw the canvas
void QwtPolarCanvas::drawContents( QPainter *painter )
{
    if ( ( d_data->paintAttributes & BackingStore ) && d_data->backingStore
            && d_data->backingStore->size() == contentsRect().size() )
    {
        painter->drawPixmap( contentsRect().topLeft(), *d_data->backingStore );
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
  and updates the backing store.

  \sa QwtPolarPlot::drawCanvas, setPaintAttributes(), testPaintAttributes()
*/
void QwtPolarCanvas::drawCanvas( QPainter *painter,
    const QRectF& canvasRect )
{
    if ( !canvasRect.isValid() )
        return;

    if ( testPaintAttribute( BackingStore ) && d_data->backingStore )
    {
        *d_data->backingStore = QPixmap( contentsRect().size() );

#ifdef Q_WS_X11
        if ( d_data->backingStore->x11Info().screen() != x11Info().screen() )
            d_data->backingStore->x11SetScreen( x11Info().screen() );
#endif

        d_data->backingStore->fill( this, d_data->backingStore->rect().topLeft() );

        QPainter bsPainter( d_data->backingStore );
        bsPainter.translate( -contentsRect().x(), -contentsRect().y() );

        plot()->drawCanvas( &bsPainter, canvasRect );

        bsPainter.end();

        painter->drawPixmap( canvasRect.topLeft().toPoint(), 
            *d_data->backingStore );
    }
    else
        plot()->drawCanvas( painter, canvasRect );
}

/*!
    Translate a point from plot into widget coordinates

    \return Point in widget coordinates
    \sa transform()
*/
QwtPointPolar QwtPolarCanvas::invTransform( const QPoint &pos ) const
{
    const QwtPolarPlot *pl = plot();

    const QwtScaleMap azimuthMap = pl->scaleMap( QwtPolar::Azimuth );
    const QwtScaleMap radialMap = pl->scaleMap( QwtPolar::Radius );

    double dx = pos.x() - pl->plotRect().center().x();
    double dy = -( pos.y() - pl->plotRect().center().y() );

    const QwtPointPolar polarPos = QwtPointPolar( QPoint( dx, dy ) ).normalized();

    const double azimuth = azimuthMap.invTransform( polarPos.azimuth() );
    const double radius = radialMap.invTransform( polarPos.radius() );

    return QwtPointPolar(azimuth, radius );
}

/*!
    Translate a point from plot into widget coordinates

    \return Point in widget coordinates
    \sa transform()
*/
QPoint QwtPolarCanvas::transform( const QwtPointPolar &polarPos ) const
{
    const QwtPolarPlot *pl = plot();

    const QwtScaleMap azimuthMap = pl->scaleMap( QwtPolar::Azimuth );
    const QwtScaleMap radialMap = pl->scaleMap( QwtPolar::Radius );

    const double radius = radialMap.transform( polarPos.radius() );
    const double azimuth = azimuthMap.transform( polarPos.azimuth() );

    const QPointF pos = qwtPolar2Pos(
        pl->plotRect().center(), radius, azimuth );

    return pos.toPoint();
}
