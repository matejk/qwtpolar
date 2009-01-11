#include <qapplication.h>
#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include "plot.h"

class MainWindow: public QMainWindow
{
public:
    MainWindow(QWidget * = NULL);

private:
     Plot *d_plot;
};

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    d_plot = new Plot(this);

    setCentralWidget(d_plot);

    QToolBar *toolBar = new QToolBar(this);

    QToolButton *btnPrint = new QToolButton(toolBar);
    QToolButton *btnGrid = new QToolButton(toolBar);

#if QT_VERSION >= 0x04000
    btnPrint->setText("Print");
    btnPrint->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addWidget(btnPrint);

    btnGrid->setText("Grid");
    btnGrid->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnGrid->setCheckable(true);
    btnGrid->setChecked(true);
    toolBar->addWidget(btnGrid);
#else
    btnPrint->setTextLabel("Print");
    btnPrint->setUsesTextLabel(true);

    btnGrid->setTextLabel("Print");
    btnGrid->setUsesTextLabel(true);
    btnGrid->setToggleButton(true);
    btnGrid->setChecked(true);
#endif

    addToolBar(toolBar);

    connect(btnPrint, SIGNAL(clicked()), d_plot, SLOT(printPlot()) );
    connect(btnGrid, SIGNAL(toggled(bool)), d_plot, SLOT(showGrid(bool)) );
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    MainWindow mainWindow;
#if QT_VERSION < 0x040000
    a.setMainWidget(&mainWindow);
#endif

    mainWindow.resize(800,600);
    mainWindow.show();

    return a.exec(); 
}
