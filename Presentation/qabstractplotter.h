#ifndef QABSTRACTPLOTTER_H
#define QABSTRACTPLOTTER_H

#include <QtWidgets/QWidget>
#include <QMap>
#include <QMenu>
#include <QPixmap>
#include <QVector>
#include <cmath>
#include <cstdio>
#include <string>
#include <QObject>
#include <QStylePainter>
#include <QtDBus/QtDBus>


/*
 *Clase para representar una Curva
 *Representa la tendencia de una variable en el tiempo
 */
 class Curve
{
    private:
        QVector<double>  data;
        QColor           color;
        QString          name;
    public:
        Curve(QString name, QColor color)
        {
            this->name = name;
            this->color = color;
        }
        Curve ()
        {
            color = QColor();
            name = "";
        }
        QVector<double> getData()
        {
            return data;
        }
        void setData(QVector<double> data)
        {
            this->data = data;
        }
        QString getName()
        {
            return name;
        }
        void setName(QString name)
        {
            this->name = name;
        }
        QColor getColor()
        {
            return color;
        }
        void setColor(QColor color)
        {
            this->color = color;
        }
};

/*
 *Clase para representar los datos de
 *configuracion del Plotter
 */
class PlotSettings
{
public:
    double minX;
    double maxX;
    int numXTicks;
    double minY;
    double maxY;
    int numYTicks;

    PlotSettings(double minValue, double maxValue, double numXTicks, double numYTicks)
    {
        this->minX = 0;
        this->maxX = numXTicks;
        this->numXTicks = numXTicks;
        this->minY = minValue;
        this->maxY = maxValue;
        this->numYTicks = numYTicks;
    }

    double spanX() const { return maxX - minX; }
    double spanY() const { return maxY - minY; }
};

class QAbstractPlotter : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int scale READ getScale WRITE setScale)
    Q_PROPERTY(int leftMargin READ getLeftMargin WRITE setLeftMargin)
    Q_PROPERTY(int rightMargin READ getRightMargin WRITE setRightMargin)
    Q_PROPERTY(int topMargin READ getTopMargin WRITE setTopMargin)
    Q_PROPERTY(int bottonMargin READ getBottonMargin WRITE setBottonMargin)
    Q_PROPERTY(double minimumValue READ getMinimumValue WRITE setMinimumValue)
    Q_PROPERTY(double maximumValue READ getMaximumValue WRITE setMaximumValue)


public:
    /**
     * Constructores y Destructores
     */
    explicit QAbstractPlotter(QWidget *parent=0);
    explicit QAbstractPlotter(int minValue, int maxValue,QWidget *parent=0);
    ~QAbstractPlotter();

    /**
     * Acciones sobre la curva
     */
    void     addCurve(QString name, int id, QColor color);
    void     removeCurve(int curveId);
    void     clear();
    void     setValueToCurve(int curveId,double value);
    void     setColorToCurve(int curveId,QColor color);
    bool     containCurve(quint32 curveId);


    /**
     * Propiedades
     */
    bool     setScale(int scale);
    int      getScale();

    void     setLeftMargin(int value);
    int      getLeftMargin();

    void     setRightMargin(int value);
    int      getRightMargin();

    void     setTopMargin(int value);
    int      getTopMargin();

    void     setBottonMargin(int value);
    int      getBottonMargin();

    void     setMinimumValue(double value);
    double   getMinimumValue();

    void     setMaximumValue(double value);
    double   getMaximumValue();

    QSize    minimumSizeHint() const;
    QSize    sizeHint() const;
    QSize    minimumSize() const;

protected:

    /**
     * Eventos
     */
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//  void mouseMoveEvent(QMouseEvent *event);
//  void keyPressEvent(QKeyEvent *event);


public slots:
    void readData(int deviceId, int varId, ushort quality, qulonglong timestamp, uchar /*readType*/, const QDBusVariant &value);
    void graphicCurve();

protected:
    /**
     * Auxiliar Functions
     */
    double absolute(double value);
    double myRound(double value);
    void setSettings(PlotSettings *settings);

    void refreshPixmap();

    void drawBackground(QPainter *painter);
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);
    void drawLejend(QPainter *painter);

    /**
     * Nucleo del Grafico
     */
    QMap<int,Curve> curveMap;
    QVector<int>    curveBag;
    QRect           curveArea;
    PlotSettings   *settings;

    /*
     *Actions
     */
    QAction *      addCurveAction;

    QMenu           popupMenu;
    QPixmap         pixmap;

    /**
     * Margenes
     */
    int leftMargin;
    int rightMargin;
    int topMargin;
    int bottonMargin;
    int scale;


    /**
     * Valores de Rango
     */
    double minimumValue;
    double maximumValue;

    int xMove;
    int yMove;
};

#endif // QABSTRACTPLOTTER_H
