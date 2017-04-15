#include "mainwindow.h"
#include "graphicsscene.h"
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QPainter>
#include <QPainterPath>
#include "qmath.h"

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    this->setBackgroundBrush(Qt::black);
}

void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    mousePoint = mouseEvent->scenePos();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    mousePoint = mouseEvent->scenePos();
    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos() << "X" << mousePoint.x() << "Y" << mousePoint.y();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    mousePoint = mouseEvent->scenePos();

    mousePoints.append(mouseEvent->scenePos());

    if(mousePoints.size() == 2)
    {

        QColor color;
        color.setRgb(128, 0, 255);
        QPen pen;
        pen.setColor(color);
        pen.setWidth(20);
        pen.setCapStyle(Qt::RoundCap);
        scene->addLine(mousePoints.at(0).x(), mousePoints.at(0).y(), mousePoints.at(1).x(), mousePoints.at(1).y(), pen);


        // use math to define the circle
        QLineF lineBC(mousePoints.at(1), mousePoints.at(2));
        QLineF lineAC(mousePoints.at(0), mousePoints.at(2));
        QLineF lineBA(mousePoints.at(1), mousePoints.at(0));
        qreal rad = qAbs(lineBC.length()/(2*qSin(qDegreesToRadians(lineAC.angleTo(lineBA)))));

        QLineF bisectorBC(lineBC.pointAt(0.5), lineBC.p2());
        bisectorBC.setAngle(lineBC.normalVector().angle());

        QLineF bisectorBA(lineBA.pointAt(0.5), lineBA.p2());
        bisectorBA.setAngle(lineBA.normalVector().angle());

        QPointF center;
        bisectorBA.intersect(bisectorBC, &center);

        qDebug() << rad << center;

        bool drawCircle = true;

        QGraphicsEllipseItem * ellipse = new QGraphicsEllipseItem(center.x() - rad, center.y() - rad, rad*2, rad*2);

        QColor color;
        QPen pen;
        QBrush brush;

        color.setRgb(255, 0, 255);
        pen.setColor(color);
        brush.setColor(color);
        brush.setStyle(Qt::SolidPattern);
        pen.setWidth(2);
        ellipse->setPen(pen);
        this->addEllipse(center.x() - rad, center.y() - rad, rad*2, rad*2,pen, brush);
        this->addEllipse(0,0,100,100,pen, brush);

        if(drawCircle)
            this->addItem(ellipse);


        // add arc
        // this->addItem(path);
        QPainterPath path;
        QLineF lineOA(center, mousePoints.at(0));
        QLineF lineOC(center, mousePoints.at(2));
        path.arcMoveTo(ellipse->boundingRect(),lineOA.angle());
        path.arcTo(ellipse->boundingRect(), lineOA.angle(), lineOC.angle() - lineOA.angle());
        QGraphicsPathItem * pathItem = new QGraphicsPathItem(path);
        pathItem->setPen(QPen(Qt::red,10));
        this->addItem(pathItem);

        if(!drawCircle)
            delete ellipse;
        mousePoints.clear();
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

QPointF GraphicsScene::getMousePoint(void)
{
    return mousePoint;
}
