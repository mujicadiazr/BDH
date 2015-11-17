#ifndef HISTORICPLOT_H
#define HISTORICPLOT_H

#include <vector>

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


#include <../Domain/variablepoint.h>

class Zoomer;

class HistoricPlot : public QwtPlot
{
    Q_OBJECT

public:
    HistoricPlot(QWidget * parent = NULL);
    virtual ~HistoricPlot();

    void addCurve(quint32 idVariable, std::vector<VariablePoint> data);

signals:

public slots:


private:
    QwtPlotGrid *m_grid;
    QwtDateScaleDraw *m_scaleDraw;
    QwtDateScaleEngine *m_scaleEngine;
    QwtLegend *m_legend;
    Zoomer* m_zoomer;
    QwtPlotPanner *m_panner;
};

#endif // HISTORICPLOT_H
