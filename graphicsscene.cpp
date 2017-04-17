#include "mainwindow.h"
#include "graphicsscene.h"
#include <QDebug>

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    this->setBackgroundBrush(Qt::black);
    myNumber = 0;
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

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    mousePoint = mouseEvent->scenePos();
    MouseX = mouseEvent->scenePos().x();
    MouseY = mouseEvent->scenePos().y();
    mousePoints.append(mouseEvent->scenePos());

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
