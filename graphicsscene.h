#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QList>
#include <QPainter>

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

    void myLine(int x1, int y1, int x2, int y2);

    QPointF mousePoint;
    int MouseX, MouseY;
    int myNumber;

    // line.cpp
    void setLineP1(int x, int y);
    void setLineP2(int x, int y);

    void drawLine(int x1, int y1, int x2, int y2);

signals:
    void changedMousePosition(QPointF mousePoint);
    void changedNumber(int myNumber);

public slots:

private:
    QList <QPointF> mousePoints;

    struct rubberband {
        double X1n, Y1n, X2n, Y2n, X3n, Y3n;
        double X1o, Y1o, X2o, Y2o, X3o, Y3o;
        bool Drawn;
    } RubberBand;
    int CoordinateNumber;

//    object Objects[1024];

    int NoOfObjects;

};

#endif // GRAPHICSSCENE_H
