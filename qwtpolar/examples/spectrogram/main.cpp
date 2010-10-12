#include <qapplication.h>
#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include "plotwindow.h"
#include "plot.h"

class MainWindow: public QMainWindow
{
public:
    MainWindow( QWidget * = NULL );

private:
    PlotWindow *d_plotWindow;
};

MainWindow::MainWindow( QWidget *parent ):
    QMainWindow( parent )
{
    d_plotWindow = new PlotWindow( this );
    setCentralWidget( d_plotWindow );

    QToolBar *toolBar = new QToolBar( this );

    QToolButton *btnExport = new QToolButton( toolBar );
    QToolButton *btnGrid = new QToolButton( toolBar );

    btnExport->setText( "Export" );
    btnExport->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( btnExport );

    btnGrid->setText( "Grid" );
    btnGrid->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    btnGrid->setCheckable( true );
    btnGrid->setChecked( true );
    toolBar->addWidget( btnGrid );

    addToolBar( toolBar );

    connect( btnExport, SIGNAL( clicked() ),
             d_plotWindow->plot(), SLOT( exportDocument() ) );
    connect( btnGrid, SIGNAL( toggled( bool ) ),
             d_plotWindow->plot(), SLOT( showGrid( bool ) ) );
}

int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    MainWindow mainWindow;
    mainWindow.resize( 700, 600 );
    mainWindow.show();

    return a.exec();
}
