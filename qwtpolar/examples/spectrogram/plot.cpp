#include <qprinter.h>
#include <qprintdialog.h>
#include <qpen.h>
#include <qwt_raster_data.h>
#include <qwt_polar_panner.h>
#include <qwt_polar_magnifier.h>
#include <qwt_polar_grid.h>
#include <qwt_polar_spectrogram.h>
#include "plot.h"

// Pointless synthetic data, showing something nice

class SpectrogramData: public QwtRasterData
{
public:
	SpectrogramData()
	{
		setInterval(Qt::ZAxis, QwtInterval(0.0, 10.0));
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
    d_spectrogram->setData(new SpectrogramData());
    d_spectrogram->attach(this);

    d_spectrogram->setZ(1.0);
    d_grid->setZ(2.0);

    new QwtPolarPanner(canvas());
    new QwtPolarMagnifier(canvas());
}

QwtPolarSpectrogram *Plot::spectrogram() 
{
    return d_spectrogram;
}

void Plot::printPlot()
{
    QPrinter printer;
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFileName("/tmp/spectrogram.pdf");
    QPrintDialog dialog(&printer);
    if ( dialog.exec() )
        renderTo(printer);
}

void Plot::showGrid(bool on)
{
    d_grid->setVisible(on);
    replot();
}
