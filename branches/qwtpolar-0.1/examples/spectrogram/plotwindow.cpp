#include <qwt_scale_engine.h>
#include <qwt_raster_data.h>
#include <qwt_polar_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qlayout.h>
#include "plot.h"
#include "plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent):
    QWidget(parent)
{
    d_plot = new Plot(this);

    d_colorScale = new QwtScaleWidget(this);
    d_colorScale->setAlignment(QwtScaleDraw::RightScale);
    d_colorScale->setColorBarEnabled(true);
    
    QwtText title("Intensity");
    QFont font = d_colorScale->font();
    font.setBold(true);
    title.setFont(font);
    d_colorScale->setTitle(title);
    
    const QwtDoubleInterval range = d_plot->spectrogram()->data().range();
    d_colorScale->setColorMap(range, d_plot->spectrogram()->colorMap());
    
    QwtLinearScaleEngine scaleEngine;
    d_colorScale->setScaleDiv(scaleEngine.transformation(),
        scaleEngine.divideScale(range.minValue(), range.maxValue(), 8, 5));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(d_plot, 10);
    layout->addWidget(d_colorScale, 10);
}
