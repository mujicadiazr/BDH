#ifndef REALTIMEPLOT_H
#define REALTIMEPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_date_scale_draw.h>
#include <qwt_date_scale_engine.h>
#include <qwt_legend.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_legend_label.h>
#include <qwt_date.h>
#include <qwt_text.h>
#include <qwt_symbol.h>

class RealtimePlot : public QwtPlot
{
public:
    RealtimePlot(QWidget * parent = NULL);
    virtual ~RealtimePlot();
};

#endif // REALTIMEPLOT_H
