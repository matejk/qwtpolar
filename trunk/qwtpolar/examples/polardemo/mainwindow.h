#include <qmainwindow.h>

class Plot;
class SettingsEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = NULL);

private slots:
    void print();
    void exportSVG();

private:
    Plot *d_plot;
    SettingsEditor *d_settingsEditor;
};
