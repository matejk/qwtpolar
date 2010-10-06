/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QwtPolar Widget Library
 * Copyright (C) 2008   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include <qpainter.h>
#include "qwt_global.h"
#include "qwt_painter.h"
#include "qwt_polar.h"
#include "qwt_scale_map.h"
#include "qwt_math.h"
#include "qwt_symbol.h"
#include "qwt_legend.h"
#include "qwt_legend_item.h"
#include "qwt_curve_fitter.h"
#include "qwt_clipper.h"
#include "qwt_polar_curve.h"

static int verifyRange(int size, int &i1, int &i2)
{
    if (size < 1)
        return 0;

    i1 = qwtLim(i1, 0, size-1);
    i2 = qwtLim(i2, 0, size-1);

    if ( i1 > i2 )
        qSwap(i1, i2);

    return (i2 - i1 + 1);
}

class QwtPolarCurve::PrivateData
{
public:
    PrivateData():
        style(QwtPolarCurve::Lines),
        curveFitter(NULL)
    {
        symbol = new QwtSymbol();
        pen = QPen(Qt::black);
    }

    ~PrivateData()
    {
        delete symbol;
        delete curveFitter;
    }

    QwtPolarCurve::CurveStyle style;
    const QwtSymbol *symbol;
    QPen pen;
    QwtCurveFitter *curveFitter;
};

//! Constructor
QwtPolarCurve::QwtPolarCurve():
    QwtPolarItem(QwtText())
{
    init();
}

/*!
  Constructor
  \param title title of the curve
*/
QwtPolarCurve::QwtPolarCurve(const QwtText &title):
    QwtPolarItem(title)
{
    init();
}

/*!
  Constructor
  \param title title of the curve
*/
QwtPolarCurve::QwtPolarCurve(const QString &title):
    QwtPolarItem(QwtText(title))
{
    init();
}

//! Destructor
QwtPolarCurve::~QwtPolarCurve()
{
    delete d_series;
    delete d_data;
}

//! Initialize data members
void QwtPolarCurve::init()
{
    d_data = new PrivateData;
    d_series = NULL;

    setItemAttribute(QwtPolarItem::AutoScale);
    setItemAttribute(QwtPolarItem::Legend);
    setZ(20.0);

    setRenderHint(RenderAntialiased, true);
}

//! \return QwtPolarCurve::Rtti_PolarCurve
int QwtPolarCurve::rtti() const
{
    return QwtPolarItem::Rtti_PolarCurve;
}

/*!
  Set the curve's drawing style

  \param style Curve style
  \sa CurveStyle, style()
*/
void QwtPolarCurve::setStyle(CurveStyle style)
{
    if ( style != d_data->style )
    {
        d_data->style = style;
        itemChanged();
    }
}

/*!
    \brief Return the current style
    \sa CurveStyle, setStyle()
*/  
QwtPolarCurve::CurveStyle QwtPolarCurve::style() const 
{ 
    return d_data->style; 
}

/*!
  \brief Assign a symbol
  \param symbol Symbol
  \sa symbol()
*/
void QwtPolarCurve::setSymbol(const QwtSymbol *symbol )
{
	if ( symbol != d_data->symbol )
	{
    	delete d_data->symbol;
    	d_data->symbol = symbol;
    	itemChanged();
	}
}

/*!
    \brief Return the current symbol
    \sa setSymbol()
*/
const QwtSymbol *QwtPolarCurve::symbol() const 
{ 
    return d_data->symbol; 
}

/*!
  \brief Assign a pen
  \param pen New pen
  \sa pen()
*/
void QwtPolarCurve::setPen(const QPen &pen)
{
    if ( pen != d_data->pen )
    {
        d_data->pen = pen;
        itemChanged();
    }
}

/*!
    \brief Return the pen used to draw the lines
    \sa setPen()
*/
const QPen& QwtPolarCurve::pen() const 
{ 
    return d_data->pen; 
}

/*!
  Initialize data with a pointer to QwtData.

  The x-values of the data object represent the azimuth,
  the y-value respresent the radius.

  \param data Data
  \sa QwtData::copy()
*/
void QwtPolarCurve::setData(QwtSeriesData<QPointF> *data)
{
    if ( d_series != data )
    {
        delete d_series;
        d_series = data;
        itemChanged();
    }
}

/*!
  \brief Insert a curve fitter

  A curve fitter interpolates the curve points. F.e QwtPolarFitter
  adds equidistant points so that the connection gets rounded instead
  of having straight lines. If curveFitter is NULL fitting is disabled.

  \sa curveFitter()
*/
void QwtPolarCurve::setCurveFitter(QwtCurveFitter *curveFitter)
{
    if ( curveFitter != d_data->curveFitter )
    {
        delete d_data->curveFitter;
        d_data->curveFitter = curveFitter;

        itemChanged();
    }
}

/*!
  \brief Return the curve fitter
  \sa setCurveFitter()
*/
QwtCurveFitter *QwtPolarCurve::curveFitter() const
{
    return d_data->curveFitter;
}

/*!
  Draw the curve

  \param painter Painter
  \param azimuthMap Maps azimuth values to values related to 0.0, M_2PI
  \param radialMap Maps radius values into painter coordinates.
  \param pole Position of the pole in painter coordinates
  \param radius Radius of the complete plot area in painter coordinates
  \param canvasRect Contents rect of the canvas in painter coordinates
*/
void QwtPolarCurve::draw(QPainter *painter,
    const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
    const QPointF &pole, double /*radius*/,
    const QRectF &) const
{
    draw(painter, azimuthMap, radialMap, pole, 0, -1);
}

/*!
  \brief Draw an interval of the curve
  \param painter Painter
  \param azimuthMap Maps azimuth values to values related to 0.0, M_2PI
  \param radialMap Maps radius values into painter coordinates.
  \param pole Position of the pole in painter coordinates
  \param from index of the first point to be painted
  \param to index of the last point to be painted. If to < 0 the
         curve will be painted to its last point.

  \sa drawCurve(), drawSymbols(),
*/
void QwtPolarCurve::draw(QPainter *painter,
    const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
    const QPointF &pole, int from, int to) const
{
    if ( !painter || dataSize() <= 0 )
        return;

    if (to < 0)
        to = dataSize() - 1;

    if ( verifyRange(dataSize(), from, to) > 0 )
    {
        painter->save();
        painter->setPen(d_data->pen);

        drawCurve(painter, d_data->style, 
            azimuthMap, radialMap, pole, from, to);
        painter->restore();

        if (d_data->symbol->style() != QwtSymbol::NoSymbol)
        {
            painter->save();
            drawSymbols(painter, *d_data->symbol, 
                azimuthMap, radialMap, pole, from, to);
            painter->restore();
        }
    }
}

/*!
  Draw the line part (without symbols) of a curve interval.

  \param painter Painter
  \param style Curve style, see QwtPolarCurve::CurveStyle
  \param azimuthMap Maps azimuth values to values related to 0.0, M_2PI
  \param radialMap Maps radius values into painter coordinates.
  \param pole Position of the pole in painter coordinates
  \param from index of the first point to be painted
  \param to index of the last point to be painted. 
  \sa draw(), drawLines()
*/
void QwtPolarCurve::drawCurve(QPainter *painter, int style, 
    const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
    const QPointF &pole, int from, int to) const
{
    switch (style)
    {
        case Lines:
            drawLines(painter, azimuthMap, radialMap, pole, from, to);
            break;
        case NoCurve:
        default:
            break;
    }
}

/*!
  Draw lines

  \param painter Painter
  \param azimuthMap Maps azimuth values to values related to 0.0, M_2PI
  \param radialMap Maps radius values into painter coordinates.
  \param pole Position of the pole in painter coordinates
  \param from index of the first point to be painted
  \param to index of the last point to be painted.
  \sa draw(), drawLines(), setCurveFitter()
*/
void QwtPolarCurve::drawLines(QPainter *painter,
    const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
    const QPointF &pole, int from, int to) const
{
    int size = to - from + 1;
    if ( size <= 0 )
        return;

    QPolygon polyline;
    if ( d_data->curveFitter )
    {
        QPolygonF points(size);
        for (int j = from; j <= to; j++)
            points[j - from] = QPointF(azimuth(j), radius(j));

        points = d_data->curveFitter->fitCurve(points);

        polyline.resize(points.size());
        for ( int i = 0; i < (int)points.size(); i++ )
        {
            const QwtPolarPoint point(points[i].x(), points[i].y());

            double r = radialMap.transform(point.radius());
            const double a = azimuthMap.transform(point.azimuth());
            polyline.setPoint(i, qwtPolar2Pos(pole, r, a).toPoint() );
        }
    }
    else
    {
        polyline.resize(size);

        for (int i = from; i <= to; i++)
        {
            const QwtPolarPoint point = sample(i);

            double r = radialMap.transform(point.radius());
            const double a = azimuthMap.transform(point.azimuth());
            polyline.setPoint(i - from, qwtPolar2Pos(pole, r, a).toPoint() );
        }
    }

    QRect clipRect = painter->window();
    clipRect.setRect(clipRect.x() - 1, clipRect.y() - 1, 
        clipRect.width() + 2, clipRect.height() + 2);

    polyline = QwtClipper::clipPolygon(clipRect, polyline);

    QwtPainter::drawPolyline(painter, polyline);
}

/*!
  Draw symbols

  \param painter Painter
  \param symbol Curve symbol
  \param azimuthMap Maps azimuth values to values related to 0.0, M_2PI
  \param radialMap Maps radius values into painter coordinates.
  \param pole Position of the pole in painter coordinates
  \param from index of the first point to be painted
  \param to index of the last point to be painted.
  \sa setSymbol(), draw(), drawCurve()
*/
void QwtPolarCurve::drawSymbols(QPainter *painter, const QwtSymbol &symbol, 
    const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
    const QPointF &pole, int from, int to) const
{
    painter->setBrush(symbol.brush());
    painter->setPen(symbol.pen());

    for (int i = from; i <= to; i++)
    {
        const QwtPolarPoint point = sample(i);
        const double r = radialMap.transform(point.radius());
        const double a = azimuthMap.transform(point.azimuth());

        const QPoint pos = qwtPolar2Pos(pole, r, a).toPoint();

        symbol.drawSymbol(painter, pos);
    }
}

/*!
  Return the size of the data arrays
  \sa setData()
*/
int QwtPolarCurve::dataSize() const
{
    return d_series->size();
}

//!  Update the widget that represents the curve on the legend
void QwtPolarCurve::updateLegend(QwtLegend *legend) const
{
    if ( !legend )
        return;
        
    QwtPolarItem::updateLegend(legend);
    
    QWidget *widget = legend->find(this);
    if ( !widget || !widget->inherits("QwtLegendItem") )
        return;
        
    QwtLegendItem *legendItem = (QwtLegendItem *)widget;
    
    const bool doUpdate = legendItem->updatesEnabled();
    legendItem->setUpdatesEnabled(false);
    
#if 0
    const int policy = legend->displayPolicy();
    if (policy == QwtLegend::FixedIdentifier)
    {
        int mode = legend->identifierMode();

        if (mode & QwtLegendItem::ShowLine)
            legendItem->setCurvePen(pen());

        if (mode & QwtLegendItem::ShowSymbol)
            legendItem->setSymbol(symbol());

        if (mode & QwtLegendItem::ShowText)
            legendItem->setText(title());
        else
            legendItem->setText(QwtText());

        legendItem->setIdentifierMode(mode);
    }
    else if (policy == QwtLegend::AutoIdentifier)
    {
        int mode = 0;

        if (QwtPolarCurve::NoCurve != style())
        {
            legendItem->setCurvePen(pen());
            mode |= QwtLegendItem::ShowLine;
        }
        if (QwtSymbol::NoSymbol != symbol().style())
        {
            legendItem->setSymbol(symbol());
            mode |= QwtLegendItem::ShowSymbol;
        }
        if ( !title().isEmpty() )
        {
            legendItem->setText(title());
            mode |= QwtLegendItem::ShowText;
        }
        else
        {
            legendItem->setText(QwtText());
        }
        legendItem->setIdentifierMode(mode);
    }
#endif

    legendItem->setUpdatesEnabled(doUpdate);
    legendItem->update();
}

/*!
   Interval, that is necessary to display the item
   This interval can be useful for operations like clipping or autoscaling

   \param scaleId Scale index
   \return bounding interval

   \sa QwtData::boundingRect()
*/
QwtInterval QwtPolarCurve::boundingInterval(int scaleId) const
{
    const QRectF boundingRect = d_series->boundingRect();
    if ( scaleId == QwtPolar::ScaleAzimuth )
        return QwtInterval(boundingRect.left(), boundingRect.right());
    else  if ( scaleId == QwtPolar::ScaleRadius )
        return QwtInterval(boundingRect.top(), boundingRect.bottom());

    return QwtInterval();
}