#include "line.h"

Line::Line():
    GraphicsScene(parent)
{
}

void Line::drawLine(int x1, int y1, int x2, int y2)
{
    // Just draw something by clicking a button

    qDebug() << "line";                 // This debug message is shown

    QColor color;
    color.setRgb(128, 0, 255);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(20);
    pen.setCapStyle(Qt::RoundCap);

    this->addLine(x1, y1, x2, y2, pen); // Didn't draw the line on the scene
}
