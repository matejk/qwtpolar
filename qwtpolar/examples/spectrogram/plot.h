#include <qwt_polar_plot.h>
#include <qwt_polar_spectrogram.h>

class Plot: public QwtPolarPlot
{
    Q_OBJECT

public:
    Plot(QWidget * = NULL);

public slots:
    void printPlot();

private:
    QwtPolarSpectrogram *d_spectrogram;
};
