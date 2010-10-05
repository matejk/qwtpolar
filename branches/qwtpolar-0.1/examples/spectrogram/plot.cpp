#include <qprinter.h>
#if QT_VERSION >= 0x040000
#include <qprintdialog.h>
#endif
#include <qpen.h>
#include <qwt_raster_data.h>
#include <qwt_color_map.h>
#include <qwt_polar_panner.h>
#include <qwt_polar_magnifier.h>
#include <qwt_polar_grid.h>
#include <qwt_polar_spectrogram.h>
#include "plot.h"

// Pointless synthetic data, showing something nice

class SpectrogramData: public QwtRasterData
{
public:
    virtual QwtRasterData *copy() const
    {
        return new SpectrogramData();
    }

    virtual QwtDoubleInterval range() const
    {
        return QwtDoubleInterval(0.0, 10.0);
    }

    virtual double value(double azimuth, double radius) const
    {
        const double c = 0.842;
        const double x = radius / 10.0 * 3.0 - 1.5;
        const double y = azimuth / 360.0 * 3.0 - 1.5;

        const double v1 = x * x + (y-c) * (y+c);
        const double v2 = 2 * x * (y+c);

        const double v = 1.0 / (v1 * v1 + v2 * v2);
        return v;
    }
};

Plot::Plot(QWidget *parent):
    QwtPolarPlot(parent)
{
    setAutoReplot(false);
    setPlotBackground(Qt::darkBlue);

    // scales 
    setScale(QwtPolar::Azimuth, 0.0, 360.0, 45.0 );
    setScaleMaxMinor(QwtPolar::Azimuth, 2);

    setScale(QwtPolar::Radius, 0.0, 10.0);
    setScaleMaxMinor(QwtPolar::Radius, 2);
    
    // grids
    d_grid = new QwtPolarGrid();
    d_grid->setPen(QPen(Qt::white));
    for ( int scaleId = 0; scaleId < QwtPolar::ScaleCount; scaleId++ )
    {
        d_grid->showGrid(scaleId);
        d_grid->showMinorGrid(scaleId);

        QPen minorPen(Qt::gray);
        d_grid->setMinorGridPen(scaleId, minorPen);
    }
    d_grid->setAxisPen(QwtPolar::AxisAzimuth, QPen(Qt::black));
    d_grid->showAxis(QwtPolar::AxisAzimuth, true);
    d_grid->showAxis(QwtPolar::AxisLeft, false);
    d_grid->showAxis(QwtPolar::AxisRight, true);
    d_grid->showAxis(QwtPolar::AxisTop, false);
    d_grid->showAxis(QwtPolar::AxisBottom, false);
    d_grid->showGrid(QwtPolar::Azimuth, true);
    d_grid->showGrid(QwtPolar::Radius, true);
    d_grid->attach(this);

    // spectrogram

    d_spectrogram = new QwtPolarSpectrogram();

    QwtLinearColorMap colorMap(Qt::darkBlue, Qt::yellow);
    colorMap.addColorStop(0.05, Qt::blue);
    colorMap.addColorStop(0.3, Qt::cyan);
    colorMap.addColorStop(0.6, Qt::green);
    colorMap.addColorStop(0.98, Qt::red);

    d_spectrogram->setColorMap(colorMap);
    d_spectrogram->setData(SpectrogramData());

    d_spectrogram->attach(this);

    d_spectrogram->setZ(1.0);
    d_grid->setZ(2.0);

    new QwtPolarPanner(canvas());
    new QwtPolarMagnifier(canvas());
}

const QwtPolarSpectrogram *Plot::spectrogram() const
{
    return d_spectrogram;
}

void Plot::printPlot()
{
    QPrinter printer;
    printer.setOrientation(QPrinter::Landscape);
#if QT_VERSION < 0x040000
    printer.setColorMode(QPrinter::Color);
    printer.setOutputFileName("/tmp/spectrogram.ps");
    if (printer.setup())
#else
    printer.setOutputFileName("/tmp/spectrogram.pdf");
    QPrintDialog dialog(&printer);
    if ( dialog.exec() )
#endif
    {
        renderTo(printer);
    }
}

void Plot::showGrid(bool on)
{
    d_grid->setVisible(on);
    replot();
}
