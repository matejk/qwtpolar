#include <qfiledialog.h>
#include <qimagewriter.h>
#include <qpen.h>
#include <qwt_raster_data.h>
#include <qwt_polar_panner.h>
#include <qwt_polar_magnifier.h>
#include <qwt_polar_grid.h>
#include <qwt_polar_spectrogram.h>
#include <qwt_polar_renderer.h>
#include <qwt_polar_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_div.h>
#include <qwt_round_scale_draw.h>
#include "plot.h"

class MyPicker: public QwtPolarPicker
{
public:
    MyPicker(QwtPolarCanvas *canvas):
        QwtPolarPicker(canvas)
    {
        setStateMachine(new QwtPickerDragPointMachine());
        setRubberBand(QwtPicker::NoRubberBand);
        setTrackerMode(ActiveOnly);
        setAngleUnit( QwtPolar::Gradians );
    }

    virtual QwtText trackerTextPolar(const QwtPointPolar &pos) const
    {
        QColor bg(Qt::white);
        bg.setAlpha(200);

        QwtText text = QwtPolarPicker::trackerTextPolar(pos);
        text.setBackgroundBrush( QBrush( bg ));
        return text;
    }
};

// Pointless synthetic data, showing something nice

class SpectrogramData: public QwtRasterData
{
public:
    SpectrogramData()
    {
        setInterval( Qt::ZAxis, QwtInterval( 0.0, 10.0 ) );
    }

    virtual double value( double azimuth, double radius ) const
    {
        const double c = 0.842;
        const double x = radius / 10.0 * 3.0 - 1.5;
        const double y = azimuth / 360.0 * 3.0 - 1.5;

        const double v1 = x * x + ( y - c ) * ( y + c );
        const double v2 = 2 * x * ( y + c );

        const double v = 1.0 / ( v1 * v1 + v2 * v2 );
        return v;
    }
};

class AzimuthScaleDraw: public QwtRoundScaleDraw
{
public:
    virtual QwtText label( double value ) const
    {
        if ( qFuzzyCompare( value, 360.0 ) )
            value = 0.0;
            
        return QLocale().toString( value );
    }
};

Plot::Plot( QWidget *parent ):
    QwtPolarPlot( parent )
{
    (void) new MyPicker( canvas() );

    setAutoReplot( false );
    setPlotBackground( Qt::darkBlue );


    // scales
    setScale( QwtPolar::Azimuth, 0.0, 360.0, 45.0 );
    setScaleMaxMinor( QwtPolar::Azimuth, 2 );

    setScale( QwtPolar::Radius, 0.0, 10.0 );
    setScaleMaxMinor( QwtPolar::Radius, 2 );

    // grids
    d_grid = new QwtPolarGrid();
    d_grid->setPen( QPen( Qt::white ) );
    for ( int scaleId = 0; scaleId < QwtPolar::ScaleCount; scaleId++ )
    {
        d_grid->showGrid( scaleId );
        d_grid->showMinorGrid( scaleId );

        QPen minorPen( Qt::gray );
        d_grid->setMinorGridPen( scaleId, minorPen );
    }
    d_grid->setAxisPen( QwtPolar::AxisAzimuth, QPen( Qt::black ) );
    d_grid->setAzimuthScaleDraw( new AzimuthScaleDraw() );
    d_grid->showAxis( QwtPolar::AxisAzimuth, true );
    d_grid->showAxis( QwtPolar::AxisLeft, false );
    d_grid->showAxis( QwtPolar::AxisRight, true );
    d_grid->showAxis( QwtPolar::AxisTop, false );
    d_grid->showAxis( QwtPolar::AxisBottom, false );
    d_grid->showGrid( QwtPolar::Azimuth, true );
    d_grid->showGrid( QwtPolar::Radius, true );
    d_grid->attach( this );

    // spectrogram

    d_spectrogram = new QwtPolarSpectrogram();
    d_spectrogram->setPaintAttribute( 
        QwtPolarSpectrogram::ApproximatedAtan, true );
    d_spectrogram->setRenderThreadCount( 0 );
    d_spectrogram->setData( new SpectrogramData() );
    d_spectrogram->attach( this );

    d_spectrogram->setZ( 1.0 );
    d_grid->setZ( 2.0 );

    new QwtPolarPanner( canvas() );
    new QwtPolarMagnifier( canvas() );
}

QwtPolarSpectrogram *Plot::spectrogram()
{
    return d_spectrogram;
}

void Plot::rotate()
{
    const double interval = 15.0; // degrees

    double origin = azimuthOrigin() / M_PI * 180.0;
    origin = qRound( origin / interval ) * interval + interval;

    setAzimuthOrigin( origin / 180.0 * M_PI );
    replot();
}

void Plot::mirror()
{
    setScale( QwtPolar::Azimuth,
        scaleDiv( QwtPolar::Azimuth )->upperBound(),
        scaleDiv( QwtPolar::Azimuth )->lowerBound(),
        45.0
    );
    replot();
}

void Plot::exportDocument()
{
    QString fileName = "spectrogram.pdf";

#ifndef QT_NO_FILEDIALOG
    const QList<QByteArray> imageFormats =
        QImageWriter::supportedImageFormats();

    QStringList filter;
    filter += "PDF Documents (*.pdf)";
#ifndef QWT_NO_POLAR_SVG
    filter += "SVG Documents (*.svg)";
#endif
    filter += "Postscript Documents (*.ps)";

    if ( imageFormats.size() > 0 )
    {
        QString imageFilter("Images (");
        for ( int i = 0; i < imageFormats.size(); i++ )
        {
            if ( i > 0 )
                imageFilter += " ";
            imageFilter += "*.";
            imageFilter += imageFormats[i];
        }
        imageFilter += ")";

        filter += imageFilter;
    }

    fileName = QFileDialog::getSaveFileName(
        this, "Export File Name", fileName,
        filter.join(";;"), NULL, QFileDialog::DontConfirmOverwrite);
#endif
    if ( !fileName.isEmpty() )
    {
        QwtPolarRenderer renderer;
        renderer.renderDocument(this, fileName, QSizeF(300, 200), 85);
    }
}

void Plot::showGrid( bool on )
{
    d_grid->setVisible( on );
    replot();
}
