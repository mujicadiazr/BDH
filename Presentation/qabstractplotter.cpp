#include "qabstractplotter.h"

#include <QtGui>
#include <QDebug>
#include <iostream>
#include <QInputDialog>
#include <QAction>
#include <QMessageBox>

using namespace std;

//////////////////////////////////////////////////////////////////
//Plotter Code                                                  //
//////////////////////////////////////////////////////////////////

QAbstractPlotter::QAbstractPlotter(int minValue, int maxValue,QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Shadow);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);

    scale = 20;
    leftMargin = 80;
    rightMargin = 120;
    topMargin = 30;
    bottonMargin = 30;

    xMove = -1;
    yMove = -1;

    curveArea = QRect(leftMargin,topMargin,width()-(leftMargin + rightMargin),height()-(topMargin + bottonMargin));
    setSettings(new PlotSettings(minValue,maxValue,curveArea.width() / scale,curveArea.height() / scale));

    addCurveAction = new QAction("&Add curve",this);
    addCurveAction->setIcon(QIcon(":/icons/add.png"));
    addCurveAction->setShortcut(QKeySequence("Ctrl+A"));
    QObject::connect(addCurveAction,SIGNAL(triggered()),this,SLOT(graphicCurve()));

    popupMenu.addAction(addCurveAction);

}

QAbstractPlotter::QAbstractPlotter(QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Shadow);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);

    scale = 20;
    leftMargin = 80;
    rightMargin = 120;
    topMargin = 30;
    bottonMargin = 30;

    xMove = -1;
    yMove = -1;

    curveArea = QRect(leftMargin,topMargin,width()-(leftMargin + rightMargin),height()-(topMargin + bottonMargin));
    setSettings(new PlotSettings(0,1,curveArea.width() / scale,curveArea.height() / scale));


    addCurveAction = new QAction("&Add curve",this);
    addCurveAction->setIcon(QIcon(":/icons/add.png"));
    addCurveAction->setShortcut(QKeySequence("Ctrl+A"));
    QObject::connect(addCurveAction,SIGNAL(triggered()),this,SLOT(graphicCurve()));

    popupMenu.addAction(addCurveAction);
}


QAbstractPlotter::~QAbstractPlotter()
{
    delete settings;
}

void QAbstractPlotter::setSettings(PlotSettings *settings)
{
    this->settings = settings;
}

double QAbstractPlotter::absolute(double value){
    return (value > -value) ? value : -value;
}

int QAbstractPlotter::getScale()
{
    return scale;
}

void QAbstractPlotter::setLeftMargin(int value)
{
    if (value > 0 && value < (width() - curveArea.width() - rightMargin))
        leftMargin = value;
}

int  QAbstractPlotter::getLeftMargin()
{
    return leftMargin;
}

void QAbstractPlotter::setRightMargin(int value)
{
    if (value > 0 && value < (width() - curveArea.width() - leftMargin))
        rightMargin = value;
}

int QAbstractPlotter::getRightMargin()
{
    return rightMargin;
}

void QAbstractPlotter::setTopMargin(int value)
{
    if (value > 0 && value < (height() - curveArea.height() - bottonMargin))
        topMargin = value;
}

int QAbstractPlotter::getTopMargin()
{
    return topMargin;
}

void QAbstractPlotter::setBottonMargin(int value)
{
    if (value > 0 && value < (height() - curveArea.height() - topMargin))
        bottonMargin = value;
}

int QAbstractPlotter::getBottonMargin()
{
    return bottonMargin;
}

void QAbstractPlotter::setMinimumValue(double value)
{
    minimumValue = value;
}

double QAbstractPlotter::getMinimumValue()
{
    return minimumValue;
}

void     QAbstractPlotter::setMaximumValue(double value)
{
    maximumValue = value;
}

double   QAbstractPlotter::getMaximumValue()
{
    return maximumValue;
}

void QAbstractPlotter::removeCurve(int curveId)
{
    curveMap.remove(curveId);
    refreshPixmap();
}

void QAbstractPlotter::clear()
{
    curveMap.clear();
    refreshPixmap();
}

QSize QAbstractPlotter::minimumSizeHint() const
{
    return QSize(6 * topMargin, 4 * topMargin);
}

QSize QAbstractPlotter::minimumSize() const
{
    return QSize(400,200);
}

QSize QAbstractPlotter::sizeHint() const
{
    return QSize(40 * topMargin,20 * topMargin);
}

void QAbstractPlotter::paintEvent(QPaintEvent * /*event*/)
{
    QStylePainter painter(this);
    painter.fillRect(rect(),QColor(0,0,0));
    painter.drawPixmap(0, 0, pixmap);
}

void QAbstractPlotter::resizeEvent(QResizeEvent */*event*/)
{
    curveArea = QRect(leftMargin,topMargin,width()-(leftMargin + rightMargin),height()-(topMargin + bottonMargin));
    settings->numYTicks = curveArea.height() / scale;
    settings->numXTicks = curveArea.width() / scale;
    refreshPixmap();
}

void QAbstractPlotter::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0){
        if (setScale(scale + 5)){
            settings->numYTicks = curveArea.height() / scale;
            settings->numXTicks = curveArea.width() / scale;
            refreshPixmap();
        }
    }
    else
        if (setScale(scale - 5)){
            settings->numYTicks = curveArea.height() / scale;
            settings->numXTicks = curveArea.width() / scale;
            refreshPixmap();
        }
}

void QAbstractPlotter::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && curveArea.contains(event->pos())) { //Si hago touch/click dentro del area de las curvas
        yMove = event->y();                                                      //con el click primario
    } else if (event->button() == Qt::RightButton) { //Si hago click derecho fuera de las curvas
        popupMenu.popup(QCursor::pos());
    }
}

void QAbstractPlotter::mouseReleaseEvent(QMouseEvent *event)
{
    if (yMove != -1) {
        if (event->y() < yMove) {
            if (setScale(scale - 5)){
                settings->numYTicks = curveArea.height() / scale;
                settings->numXTicks = curveArea.width() / scale;
                refreshPixmap();
            }
        }
        else if (event->y() > yMove) {
            if (setScale(scale + 5)){
                settings->numYTicks = curveArea.height() / scale;
                settings->numXTicks = curveArea.width() / scale;
                refreshPixmap();
            }
        }
        yMove = -1;
    }

}

void QAbstractPlotter::readData(int /*deviceId*/, int varId, ushort /*quality*/, qulonglong /*timestamp*/, uchar, const QDBusVariant &value)
{
    cerr << "ENTREEE!!!!!!!!!!"<<endl;

    double val = value.variant().toDouble();
    qDebug() << "Recibi la senal!!!\n" << "El valor es : " << val << "\n";

    if (!curveMap.contains(varId)) { //Si es una curva que no se esta graficando
        if (!curveBag.contains(varId))//Si todavia no esta en el saco entonces la meto en el saco
            curveBag.push_back(varId);
    } else { //Si no, es una que esta siendo graficada, entonces grafico su valor

        if (val < getMinimumValue())
            setMinimumValue(val);

        if (val > getMaximumValue())
            setMaximumValue(val);

        refreshPixmap(); //Refresco por si modifique los limites

        setValueToCurve(varId,val);
    }
}

bool QAbstractPlotter::setScale(int scale)
{
    int m = (curveArea.width() < curveArea.height()) ? curveArea.width() : curveArea.height();
    if((scale < m / 2) && (scale >= 25)){
        this->scale = scale;
        return true;
    }
    return false;
}

double QAbstractPlotter::myRound(double value)
{
    QString format = "%.3lf";
    char cad[100]="";
    double val=0;
    sprintf(cad,format.toStdString().c_str(),value);
    sscanf(cad,"%lf",&val);
    return val;
}

void QAbstractPlotter::refreshPixmap()
{
    pixmap = QPixmap(size());
    QPainter painter(&pixmap);
    painter.initFrom(this);
    drawGrid(&painter);
    drawLejend(&painter);
    drawCurves(&painter);
    update();
}

void QAbstractPlotter::drawGrid(QPainter *painter)
{
    if (!curveArea.isValid())
        return;
    QPainterPath a;

    painter->setPen(QColor(0,0,0));
    painter->setBackground(QColor(0,0,0));

    painter->setBrush(QColor(43,47,63));
    painter->drawRect(rect());
    painter->setBrush(QColor(0,0,0));
    painter->drawRect(curveArea);
    painter->setBrush(QColor(255,255,255));

    QPen quiteDark = palette().dark().color().light();
    QPen light = palette().light().color();
    QColor lines(200,200,200);

    light.setWidth(2);
    for (int i = 0; i <= settings->numXTicks; ++i)
    {
        int x = curveArea.left() + (i * (curveArea.width() - 1) / settings->numXTicks);
        painter->setPen(QPen(QBrush(lines), 1 ,Qt::DotLine));
        painter->drawLine(x, curveArea.top(), x, curveArea.bottom());

        painter->setPen(QPen(QBrush(lines), 1 ,Qt::DotLine));
        painter->drawLine(x, curveArea.bottom(), x, curveArea.bottom() + 5);
    }


    for (int j = 0; j <= settings->numYTicks; ++j)
    {
        int y = curveArea.bottom() - (j * (curveArea.height()) / settings->numYTicks);
        double num = myRound(settings->minY + (j * settings->spanY() / settings->numYTicks));

        painter->setPen(QPen(QBrush(lines), 1 ,Qt::DotLine));
        painter->drawLine(curveArea.left(), y, curveArea.right(), y);

//        painter->setPen(QPen(QBrush(lines), 1 ,Qt::SolidLine));
//        painter->drawLine(curveArea.left() - 5, y, curveArea.left(), y);

        painter->setPen(QColor(255,255,255));
        painter->setFont(QFont("Arial", 8,QFont::Bold));
        painter->drawText(0, y - 10, leftMargin - 10, 20,Qt::AlignRight | Qt::AlignVCenter,QString().setNum(num));
    }
}

void QAbstractPlotter::drawCurves(QPainter *painter)
{
    if (!curveArea.isValid())
        return;
    painter->setClipRect(curveArea.adjusted(+1, +1, -1, -1));
    QMapIterator<int, Curve> i(curveMap);

    while (i.hasNext()) {
        i.next();
        Curve data = i.value();
        QPolygonF polyline(data.getData().count());
        for (int j = 0; j < data.getData().count(); ++j) {

            double dx = j;
            double dy = data.getData()[j] - settings->minY;

            double x = curveArea.left()   + (dx * curveArea.width() / settings->numXTicks);
            double y = curveArea.bottom() - (dy * (curveArea.height()) / settings->spanY());

            polyline[j] = QPointF(x, y);
        }

        painter->setPen(QPen(data.getColor(),2));
        painter->drawPolyline(polyline);
    }
}

void QAbstractPlotter::addCurve(QString name, int id, QColor color)
{
    curveMap[id] = Curve(name,color);
    refreshPixmap();
}

void QAbstractPlotter::setValueToCurve(int curveId, double value)
{
    if(!curveMap.contains(curveId))
        return;

    if(value < settings->minY)
        settings->minY = value;
    else if(value > settings->maxY)
        settings->maxY = value;

    QVector<double> temp = curveMap[curveId].getData();
    temp.push_back(value);

    if(temp.size() > settings->numXTicks + 1) //Aqui es donde hago el efecto ventana deslizante,
        temp.pop_front();                     //o sea aqui es donde saco de la cabeza de la cola

    curveMap[curveId].setData(temp);
    refreshPixmap();
}

void QAbstractPlotter::setColorToCurve(int curveId, QColor color)
{
    if(!curveMap.contains(curveId))
        return;
    curveMap[curveId].setColor(color);
    refreshPixmap();
}

bool QAbstractPlotter::containCurve(quint32 curveId)
{
    return curveMap.contains(curveId);
}

void QAbstractPlotter::graphicCurve()
{
    QStringList items;

    foreach (int item, curveBag) {
        items.append(QString().setNum(item));
    }
    bool ret = false;

    QString curve = QInputDialog::getItem(this,"Add curve","Select from all curves ID",items,0,false,&ret);
    if (ret) {
        addCurve(QString("id=")+curve,curve.toInt(),QColor(rand()%255,rand()%255,rand()%255));
    }
}

void QAbstractPlotter::drawLejend(QPainter *painter)
{
    QFont font("Arial", 10, QFont::Bold,true);
    QMapIterator<int, Curve> itt(curveMap);
    int i=0;

    while (itt.hasNext()) {
        itt.next();
        Curve data = itt.value();

        painter->setPen(data.getColor());
        painter->setBrush(QBrush(data.getColor(),Qt::SolidPattern));
        painter->drawRect(width() - rightMargin + 10 , topMargin + 20*i,10,10);
        painter->setPen(palette().light().color());
        painter->setFont(font);
        painter->drawText(width() - rightMargin + 30 , topMargin + 20*i+10,data.getName());
        ++i;
    }

}

void QAbstractPlotter::drawBackground(QPainter *painter)
{
    painter->setPen(Qt::black);
    painter->drawRect(0,0,width(),height());
}
