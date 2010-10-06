#include <qwt_polar_plot.h>

class QwtPolarGrid;
class QwtPolarSpectrogram;

class Plot: public QwtPolarPlot
{
    Q_OBJECT

public:
    Plot(QWidget * = NULL);
    QwtPolarSpectrogram *spectrogram();

public Q_SLOTS:
    void printPlot();
    void showGrid(bool);

private:
    QwtPolarGrid *d_grid;
    QwtPolarSpectrogram *d_spectrogram;
};
