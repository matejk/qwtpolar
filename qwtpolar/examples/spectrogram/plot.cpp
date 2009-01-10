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
#if 0
	SpectrogramData():
		QwtRasterData(QwtDoubleRect(0.0, 0.0, 360.0, 8.0))
	{
	}
#endif

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
        const double v2 = x * (y+c) + x * (y+c);

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
#if 1
    setScale(QwtPolar::Azimuth, 0.0, 360.0, 360.0 / 8.0 );
#endif

    setScaleMaxMinor(QwtPolar::Azimuth, 2);
    setScale(QwtPolar::Radius, 0.0, 10.0);
    
    // grids
    QwtPolarGrid *grid = new QwtPolarGrid();
    grid->setPen(QPen(Qt::white));
    for ( int scaleId = 0; scaleId < QwtPolar::ScaleCount; scaleId++ )
    {
        grid->showGrid(scaleId);
        grid->showMinorGrid(scaleId);

        QPen minorPen(Qt::gray);
#if 0
        minorPen.setStyle(Qt::DotLine);
#endif
        grid->setMinorGridPen(scaleId, minorPen);
    }
    grid->setAxisPen(QwtPolar::AxisAzimuth, QPen(Qt::black));
    grid->showAxis(QwtPolar::AxisAzimuth, true);
    grid->showAxis(QwtPolar::AxisLeft, false);
    grid->showAxis(QwtPolar::AxisRight, true);
    grid->showAxis(QwtPolar::AxisTop, false);
    grid->showAxis(QwtPolar::AxisBottom, false);
    grid->showGrid(QwtPolar::Azimuth, true);
    grid->showGrid(QwtPolar::Radius, true);
#if 1
    grid->attach(this);
#endif

	// spectrogram

    QwtPolarSpectrogram *spectrogram = new QwtPolarSpectrogram();

    QwtLinearColorMap colorMap(Qt::darkBlue, Qt::yellow);
    colorMap.addColorStop(0.05, Qt::blue);
    colorMap.addColorStop(0.3, Qt::cyan);
    colorMap.addColorStop(0.6, Qt::green);
    colorMap.addColorStop(0.98, Qt::red);

    spectrogram->setColorMap(colorMap);
    spectrogram->setData(SpectrogramData());

    spectrogram->attach(this);

	spectrogram->setZ(1.0);
	grid->setZ(2.0);

    new QwtPolarPanner(canvas());
    new QwtPolarMagnifier(canvas());

    replot();
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

