#include <qregexp.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qprinter.h>
#include <qfiledialog.h>
#include <qlayout.h>
#if QT_VERSION >= 0x040000
#ifdef QT_SVG_LIB
#include <qsvggenerator.h>
#endif
#include <qprintdialog.h>
#include <qfileinfo.h>
#endif
#include "mainwindow.h"
#include "plot.h"
#include "settingseditor.h"
#include "pixmaps.h"

MainWindow::MainWindow(QWidget *parent): 
    QMainWindow(parent)
{
    QWidget *w = new QWidget(this);

    d_plot = new Plot(w);
    d_settingsEditor = new SettingsEditor(w);

    d_settingsEditor->showSettings(d_plot->settings());
    connect(d_settingsEditor, SIGNAL(edited(const PlotSettings&)),
        d_plot, SLOT(applySettings(const PlotSettings&)) );

    QHBoxLayout *layout = new QHBoxLayout(w);
    layout->addWidget(d_settingsEditor, 0);
    layout->addWidget(d_plot, 10);

    setCentralWidget(w);

    QToolBar *toolBar = new QToolBar(this);

    QToolButton *btnPrint = new QToolButton(toolBar);
#if QT_VERSION >= 0x040000
    btnPrint->setText("Print");
    btnPrint->setIcon(QIcon(print_xpm));
    btnPrint->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addWidget(btnPrint);
#else
    btnPrint->setTextLabel("Print");
    btnPrint->setPixmap(print_xpm);
    btnPrint->setUsesTextLabel(true);
#endif

    connect(btnPrint, SIGNAL(clicked()), SLOT(print()));

#if QT_VERSION >= 0x040300
#ifdef QT_SVG_LIB
    QToolButton *btnSVG = new QToolButton(toolBar);
    btnSVG->setText("SVG");
    btnSVG->setIcon(QIcon(print_xpm));
    btnSVG->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addWidget(btnSVG);

    connect(btnSVG, SIGNAL(clicked()), SLOT(exportSVG()));
#endif
#endif

    addToolBar(toolBar);
}

void MainWindow::print()
{
#if 0
    QPrinter printer;
#else
    QPrinter printer(QPrinter::HighResolution);
#if QT_VERSION < 0x040000
    printer.setOutputToFile(true);
    printer.setOutputFileName("/tmp/polardemo.ps");
    printer.setColorMode(QPrinter::Color);
#else
    printer.setOutputFileName("/tmp/polardemo.pdf");
#endif
#endif

    QString docName = d_plot->title().text();
    if ( !docName.isEmpty() )
    {
        docName.replace (QRegExp (QString::fromLatin1 ("\n")), tr (" -- "));
        printer.setDocName (docName);
    }

    printer.setCreator("Polardemo example");
    printer.setOrientation(QPrinter::Landscape);

#if QT_VERSION >= 0x040000
    QPrintDialog dialog(&printer);
    if ( dialog.exec() )
    {
#else
    if (printer.setup())
    {
#endif
        d_plot->renderTo(printer);
    }
}

void MainWindow::exportSVG()
{
    QString fileName = "polardemo.svg";

#if QT_VERSION >= 0x040300
#ifdef QT_SVG_LIB
#ifndef QT_NO_FILEDIALOG
    fileName = QFileDialog::getSaveFileName(
        this, "Export File Name", QString(),
        "SVG Documents (*.svg)");
#endif
    if ( !fileName.isEmpty() )
    {
        QSvgGenerator generator;
        generator.setFileName(fileName);
        generator.setSize(QSize(800, 600));

        d_plot->renderTo(generator);
    }
#endif
#endif
}
