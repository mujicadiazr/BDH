#include "historicplot.h"

class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer( QWidget *canvas ):
        QwtPlotZoomer( canvas )
    {
        setRubberBandPen( QColor( Qt::darkGreen ) );
        setTrackerMode( QwtPlotPicker::AlwaysOn );
    }

protected:
    virtual QwtText trackerTextF( const QPointF &pos ) const
    {
        const QDateTime dt = QwtDate::toDateTime( pos.x() );

        QString s;
        s += QwtDate::toString( QwtDate::toDateTime( pos.x() ),
            "yyyy MMM dd\nhh:mm:ss ", QwtDate::FirstThursday );

        QwtText text( s );
        text.setColor( Qt::white );

        QColor c = rubberBandPen().color();
        text.setBorderPen( QPen( c ) );
        text.setBorderRadius( 6 );
        c.setAlpha( 170 );
        text.setBackgroundBrush( c );

        return text;
    }
};

HistoricPlot::HistoricPlot(QWidget *parent)
:QwtPlot(parent)
{
    //Fondo del canvas negro
    canvas()->setPalette(Qt::black);

    //Auto repintar las curvas
    setAutoReplot(true);

    setTitle("Historian Variable Plot");

    // Grid
    m_grid = new QwtPlotGrid;
    m_grid->enableXMin( true );
    m_grid->setMajorPen( Qt::gray, 0, Qt::DotLine );
    m_grid->setMinorPen( Qt::darkGray, 0, Qt::DotLine );
    m_grid->attach( this );

    //Scale
    m_scaleDraw = new QwtDateScaleDraw( Qt::UTC );

    m_scaleDraw->setDateFormat( QwtDate::Millisecond, "hh:mm:ss:zzz\nddd dd MMM yyyy" );
    m_scaleDraw->setDateFormat( QwtDate::Second, "hh:mm:ss\nddd dd MMM yyyy" );
    m_scaleDraw->setDateFormat( QwtDate::Minute, "hh:mm\nddd dd MMM yyyy" );
    m_scaleDraw->setDateFormat( QwtDate::Hour, "hh:mm\nddd dd MMM yyyy" );
    m_scaleDraw->setDateFormat( QwtDate::Day, "ddd dd MMM yyyy" );
    m_scaleDraw->setDateFormat( QwtDate::Week, "Www yyyy" );
    m_scaleDraw->setDateFormat( QwtDate::Month, "MMM yyyy" );
    m_scaleDraw->setDateFormat( QwtDate::Year, "yyyy");


    m_scaleEngine = new QwtDateScaleEngine( Qt::UTC );

    //Axis
    setAxisTitle( QwtPlot::xBottom, QString( "Time" ) );
    setAxisScaleDraw( QwtPlot::xBottom, m_scaleDraw );
    setAxisScaleEngine( QwtPlot::xBottom, m_scaleEngine );
    setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );

    setAxisTitle( QwtPlot::yLeft, QString( "Value" ) );

    //Legend
    m_legend = new QwtLegend;
    insertLegend(m_legend);

    //Zoomer
    m_zoomer = new Zoomer( canvas() );
    m_zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier );
    m_zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
        Qt::RightButton );

    //Panner
    m_panner = new QwtPlotPanner( canvas() );
    m_panner->setMouseButton( Qt::MidButton );

    resize(800,600);
}

HistoricPlot::~HistoricPlot()
{
    delete m_grid;
    delete m_legend;
    delete m_panner;
    delete m_scaleDraw;
    delete m_scaleEngine;
    delete m_zoomer;
}

void HistoricPlot::addCurve(quint32 idVariable, std::vector<VariablePoint> data)
{
    m_zoomer->setZoomBase(true);

    QwtPlotCurve *c = new QwtPlotCurve(QString("id : %1").arg(idVariable));
    c->setPen( QColor(qrand() % 255,qrand() % 255,qrand() % 255), 2 ),
    c->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 5, 5 ) );
    c->setSymbol( symbol );

    QVector<VariablePoint> d2 = QVector<VariablePoint>::fromStdVector(data);
    QVector<QPointF> cleanData;

    for (int i = 0; i < data.size(); ++i) {
        VariablePoint vp = d2.at(i);
        qDebug() << QString("ID: %1, VALUE: %2").arg(vp.idVariable()).arg(vp.value().toDouble());

        if (vp.value().type() == QVariant::Double)
            cleanData.append(QPointF(QwtDate::toDouble(QDateTime::fromMSecsSinceEpoch(vp.timeStamp())),
                                 vp.value().toDouble()));
        else
            cleanData.append(QPointF(QwtDate::toDouble(QDateTime::fromMSecsSinceEpoch(vp.timeStamp())),
                                 (double)vp.value().toLongLong()));
    }

    c->setSamples(cleanData);
    c->attach(this);
}
