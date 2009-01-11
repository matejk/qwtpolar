#include <qwt_polar_plot.h>

class QwtPolarGrid;

class Plot: public QwtPolarPlot
{
    Q_OBJECT

public:
    Plot(QWidget * = NULL);

public slots:
    void printPlot();
    void showGrid(bool);

private:
    QwtPolarGrid *d_grid;
};
