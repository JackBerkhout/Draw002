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
//    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::RoundCap);
    // Group all selected items together
//    QList<QGraphicsItem *> x86List;
//    QGraphicsItemGroup *x86Group;
//    x86Group = this->createItemGroup(x86List);
//    QGraphicsItemGroup *group = this->createItemGroup(this->selectedItems());
//    group
//    x86Group
    if (RubberBand.Drawn) {
        color.setRgb(0, 0, 0);
        pen.setColor(color);
        this->addLine(RubberBand.X1o, RubberBand.Y1o, RubberBand.X2o, RubberBand.Y2o, pen); // Didn't draw the line on the scene
        RubberBand.Drawn = false;
    }

    color.setRgb(255, 255, 255);
    pen.setColor(color);
    this->addLine(RubberBand.X1n, RubberBand.Y1n, RubberBand.X2n, RubberBand.Y2n, pen); // Didn't draw the line on the scene
    RubberBand.X1o = RubberBand.X1n;
    RubberBand.Y1o = RubberBand.Y1n;
    RubberBand.X2o = RubberBand.X2n;
    RubberBand.Y2o = RubberBand.Y2n;
    RubberBand.Drawn = true;

    // Destroy the group, and delete the group item
//    this->destroyItemGroup(group);
//    x86Group->hide();

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

    this->addLine(x1, y1, x2, y2, pen); // Didn't draw the line on the scene
}
