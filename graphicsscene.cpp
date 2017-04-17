#include "mainwindow.h"
#include "graphicsscene.h"
#include <QDebug>
#include "line.h"

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    this->setBackgroundBrush(Qt::black);
    myNumber = 0;
    CoordinateNumber = 0;
//    this-> ->setMouseTracking(true);
}

void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
//    mousePoint = mouseEvent->scenePos();
//    MouseX = mousePoint.x();
//    MouseY = mousePoint.y();
    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    mousePoint = mouseEvent->scenePos();
    MouseX = mouseEvent->scenePos().x();
    MouseY = mouseEvent->scenePos().y();

    emit changedMousePosition(mousePoint);

    if (CoordinateNumber == 1) {
        setLineP2(MouseX, MouseY);
    }

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    mousePoint = mouseEvent->scenePos();
    MouseX = mouseEvent->scenePos().x();
    MouseY = mouseEvent->scenePos().y();
    mousePoints.append(mouseEvent->scenePos());

    CoordinateNumber++;
    if (CoordinateNumber == 1) {
        setLineP1(MouseX, MouseY);
    }

    MainWindow *mainWindow = new MainWindow();
    mainWindow->Count++;

    if(mousePoints.size() == 2)
    {
        myNumber++;

        emit changedNumber(myNumber);

        QColor color;
        color.setRgb(128, 0, 255);
        QPen pen;
        pen.setColor(color);
        pen.setWidth(20);
        pen.setCapStyle(Qt::RoundCap);
        this->addLine(mousePoints.at(0).x(), mousePoints.at(0).y(), mousePoints.at(1).x(), mousePoints.at(1).y(), pen);

        mousePoints.clear();
        CoordinateNumber = 0;
        RubberBand.Drawn = false;
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    mousePoint = mouseEvent->scenePos();
    MouseX = mouseEvent->scenePos().x();
    MouseY = mouseEvent->scenePos().y();
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

QPointF GraphicsScene::getMousePoint(void)
{
    return mousePoint;
}

int GraphicsScene::getMouseX(void)
{
    MouseX = mousePoint.x();
    return mousePoint.x();
}

int GraphicsScene::getMouseY(void)
{
    MouseY = mousePoint.y();
    return mousePoint.y();
}

void GraphicsScene::setNumber(int num)
{
    myNumber = num;
}

int GraphicsScene::getNumber(void)
{
    return myNumber;
}

void GraphicsScene::myLine(int x1, int y1, int x2, int y2)
{
//    drawLine(x1, y1, x2, y2);
    QPoint point1, point2;
    point1.setX(x1);
    point1.setY(y1);
    point2.setX(x2);
    point2.setY(y2);

//    QPainter painter(this);
//    painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
//    painter.drawLine(point1, point2);
}
