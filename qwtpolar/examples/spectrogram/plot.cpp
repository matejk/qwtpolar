#include <qprinter.h>
#if QT_VERSION >= 0x040000
#include <qprintdialog.h>
#endif
#include <qwt_raster_data.h>
#include <qwt_color_map.h>
#include <qwt_polar_spectrogram.h>
#include "plot.h"

class SpectrogramData: public QwtRasterData
{
public:
    SpectrogramData():
        QwtRasterData(QwtDoubleRect(-1.5, -1.5, 3.0, 3.0))
    {
    }

    virtual QwtRasterData *copy() const
    {
        return new SpectrogramData();
    }

    virtual QwtDoubleInterval range() const
    {
        return QwtDoubleInterval(0.0, 10.0);
    }

    virtual double value(double x, double y) const
    {
        const double c = 0.842;

        const double v1 = x * x + (y-c) * (y+c);
        const double v2 = x * (y+c) + x * (y+c);

        return 1.0 / (v1 * v1 + v2 * v2);
    }
};

Plot::Plot(QWidget *parent):
    QwtPolarPlot(parent)
{
    d_spectrogram = new QwtPolarSpectrogram();

    QwtLinearColorMap colorMap(Qt::darkCyan, Qt::red);
    colorMap.addColorStop(0.1, Qt::cyan);
    colorMap.addColorStop(0.6, Qt::green);
    colorMap.addColorStop(0.95, Qt::yellow);

#if 0
    d_spectrogram->setColorMap(colorMap);
    d_spectrogram->setData(SpectrogramData());
#endif

    d_spectrogram->attach(this);

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

