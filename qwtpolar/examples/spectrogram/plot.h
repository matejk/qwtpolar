#include <qwt_polar_plot.h>

class Plot: public QwtPolarPlot
{
    Q_OBJECT

public:
    Plot(QWidget * = NULL);

public slots:
    void printPlot();
};
