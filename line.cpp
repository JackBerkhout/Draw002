#include "mainwindow.h"
#include "graphicsscene.h"
#include <QDebug>

void GraphicsScene::setLineP1(int x, int y)
{
    RubberBand.X1n = x;
    RubberBand.Y1n = y;
    RubberBand.Drawn = false;
}

void GraphicsScene::setLineP2(int x, int y)
{
    RubberBand.X2n = x;
    RubberBand.Y2n = y;
//    RubberBand.Drawn = true;
    QColor color;
    color.setRgb(255, 255, 255);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::RoundCap);
    if (RubberBand.Drawn) {
        color.setRgb(0, 0, 0);
        pen.setColor(color);
        this->addLine(RubberBand.X1o, RubberBand.Y1o, RubberBand.X2o, RubberBand.Y2o, pen);
        RubberBand.Drawn = false;
    }

    color.setRgb(255, 255, 255);
    pen.setColor(color);

//    QPainter painter;
////    QRectF rect;
////    rect.setX(0);
////    rect.setY(0);
////    rect.setWidth(1000);
////    rect.setHeight(1000);
////    this->drawBackground(painter, rect);

//    painter.begin(this);
//    QPointF p1, p2;
//    p1.setX(100);
//    p1.setY(100);
//    p2.setX(200);
//    p2.setY(200);
//    painter.setPen(QPen(Qt::white, 12, Qt::DashLine, Qt::RoundCap));
//    painter.drawLine(p1, p2);
//    painter.end();

    this->addLine(RubberBand.X1n, RubberBand.Y1n, RubberBand.X2n, RubberBand.Y2n, pen);
    RubberBand.X1o = RubberBand.X1n;
    RubberBand.Y1o = RubberBand.Y1n;
    RubberBand.X2o = RubberBand.X2n;
    RubberBand.Y2o = RubberBand.Y2n;
    RubberBand.Drawn = true;
}

void GraphicsScene::drawLine(int x1, int y1, int x2, int y2)
{
    // Just draw something by clicking a button

    qDebug() << "line2";                 // This debug message is shown

    QColor color;
    color.setRgb(128, 0, 255);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(20);
    pen.setCapStyle(Qt::RoundCap);

    this->addLine(x1, y1, x2, y2, pen);
}
