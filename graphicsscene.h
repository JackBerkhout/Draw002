#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QList>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

    QPointF getMousePoint(void);
    int getMouseX(void);
    int getMouseY(void);
    int getNumber(void);
    void setNumber(int num);

    QPointF mousePoint;
    int MouseX, MouseY;
    int myNumber;

signals:
    void changedMousePosition(QPointF mousePoint);
    void changedNumber(int myNumber);

public slots:

private:
    QList <QPointF> mousePoints;

//    int Number;

};

#endif // GRAPHICSSCENE_H
