#ifndef DRAWGRAPHICS_H
#define DRAWGRAPHICS_H

#include <QGraphicsView>

class DrawGraphics
{
public:
    DrawGraphics(QGraphicsView ** graphicsView);
    ~DrawGraphics();
    void setData(QVector<double> x, QVector<double> y);
    void setXAxisRange(double startVal, double endVal);
    void setYAxisRange(double startVal, double endVal);
    void setXAxisLabel(QString text);
    void setYAxisLabel(QString text);
    void setGraphTitle(QString text);
    void drawGraph();
private:
    QGraphicsView * m_graphicsView;
    QVector<double> m_x;
    QVector<double> m_y;
    double m_startXAxisRange;
    double m_endXAxisRange;
    double m_startYAxisRange;
    double m_endYAxisRange;
    QString m_xAxisLabel;
    QString m_yAxisLabel;
    QString m_title;
};

#endif // DRAWGRAPHICS_H
