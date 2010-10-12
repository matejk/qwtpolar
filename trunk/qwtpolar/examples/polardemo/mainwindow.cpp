#include <qregexp.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qprinter.h>
#include <qfiledialog.h>
#include <qlayout.h>
#ifdef QT_SVG_LIB
#include <qsvggenerator.h>
#endif
#include <qprintdialog.h>
#include <qfileinfo.h>
#include <qwt_polar_panner.h>
#include <qwt_polar_magnifier.h>
#include "mainwindow.h"
#include "plot.h"
#include "settingseditor.h"
#include "pixmaps.h"

MainWindow::MainWindow( QWidget *parent ):
    QMainWindow( parent )
{
    QWidget *w = new QWidget( this );

    d_plot = new Plot( w );

    d_panner = new QwtPolarPanner( d_plot->canvas() );
    d_panner->setEnabled( false );

    d_zoomer = new QwtPolarMagnifier( d_plot->canvas() );
    d_zoomer->setEnabled( false );

    d_settingsEditor = new SettingsEditor( w );

    d_settingsEditor->showSettings( d_plot->settings() );
    connect( d_settingsEditor, SIGNAL( edited( const PlotSettings& ) ),
             d_plot, SLOT( applySettings( const PlotSettings& ) ) );

    QHBoxLayout *layout = new QHBoxLayout( w );
    layout->addWidget( d_settingsEditor, 0 );
    layout->addWidget( d_plot, 10 );

    setCentralWidget( w );

    QToolBar *toolBar = new QToolBar( this );

    QToolButton *btnZoom = new QToolButton( toolBar );

    const QString zoomHelp =
        "Use the wheel to zoom in/out.\n"
        "When the plot is zoomed in,\n"
        "use the left mouse button to move it.";

    btnZoom->setText( "Zoom" );
    btnZoom->setIcon( QIcon( zoom_xpm ) );
    btnZoom->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    btnZoom->setToolTip( zoomHelp );
    btnZoom->setCheckable( true );
    toolBar->addWidget( btnZoom );
    connect( btnZoom, SIGNAL( toggled( bool ) ), SLOT( enableZoomMode( bool ) ) );

    QToolButton *btnPrint = new QToolButton( toolBar );
    btnPrint->setText( "Print" );
    btnPrint->setIcon( QIcon( print_xpm ) );
    btnPrint->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( btnPrint );
    connect( btnPrint, SIGNAL( clicked() ), SLOT( print() ) );

#ifdef QT_SVG_LIB
    QToolButton *btnSVG = new QToolButton( toolBar );
    btnSVG->setText( "SVG" );
    btnSVG->setIcon( QIcon( print_xpm ) );
    btnSVG->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( btnSVG );

    connect( btnSVG, SIGNAL( clicked() ), SLOT( exportSVG() ) );
#endif

    addToolBar( toolBar );
}

void MainWindow::print()
{
    QPrinter printer( QPrinter::HighResolution );
    printer.setOutputFileName( "/tmp/polardemo.pdf" );

    QString docName = d_plot->title().text();
    if ( !docName.isEmpty() )
    {
        docName.replace ( QRegExp ( QString::fromLatin1 ( "\n" ) ), tr ( " -- " ) );
        printer.setDocName ( docName );
    }

    printer.setCreator( "Polardemo example" );
    printer.setOrientation( QPrinter::Landscape );

    QPrintDialog dialog( &printer );
    if ( dialog.exec() )
        d_plot->renderTo( printer );
}

void MainWindow::exportSVG()
{
    QString fileName = "polardemo.svg";

#ifdef QT_SVG_LIB
#ifndef QT_NO_FILEDIALOG
    fileName = QFileDialog::getSaveFileName(
                   this, "Export File Name", QString(),
                   "SVG Documents (*.svg)" );
#endif
    if ( !fileName.isEmpty() )
    {
        QSvgGenerator generator;
        generator.setFileName( fileName );
        generator.setSize( QSize( 800, 600 ) );

        d_plot->renderTo( generator );
    }
#endif
}

void MainWindow::enableZoomMode( bool on )
{
    d_panner->setEnabled( on );
    d_zoomer->setEnabled( on );
}
