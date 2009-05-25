#include <qwt_polar_plot.h>

class QwtPolarGrid;
class QwtPolarSpectrogram;

class Plot: public QwtPolarPlot
{
    Q_OBJECT

public:
    Plot(QWidget * = NULL);

    const QwtPolarSpectrogram *spectrogram() const;

public slots:
    void printPlot();
    void showGrid(bool);

private:
    QwtPolarGrid *d_grid;
    QwtPolarSpectrogram *d_spectrogram;
};
