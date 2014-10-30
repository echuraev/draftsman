#include "drawgraphics.h"

DrawGraphics::DrawGraphics(QGraphicsView ** graphicsView)
{
    m_graphicsView = *graphicsView;
}

DrawGraphics::~DrawGraphics()
{}

void DrawGraphics::setData(QVector<double> x, QVector<double> y)
{
    m_x = x;
    m_y = y;
}

void DrawGraphics::setXAxisRange(double startVal, double endVal)
{
    m_startXAxisRange = startVal;
    m_endXAxisRange = endVal;
}

void DrawGraphics::setYAxisRange(double startVal, double endVal)
{
    m_startYAxisRange = startVal;
    m_endYAxisRange = endVal;
}

void DrawGraphics::setXAxisLabel(QString text)
{m_xAxisLabel = text;}

void DrawGraphics::setYAxisLabel(QString text)
{m_yAxisLabel = text;}

void DrawGraphics::setGraphTitle(QString text)
{m_title = text;}

void DrawGraphics::drawGraph()
{
    QGraphicsScene *scene = new QGraphicsScene(m_graphicsView);
    QPen pen(Qt::black);
    scene->addLine(0,90,180,90,pen);//x
    scene->addLine(90,0,90,180,pen);//y
    m_graphicsView->setScene(scene);
}
