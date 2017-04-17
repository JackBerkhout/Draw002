#ifndef LINE_H
#define LINE_H

#include <QMainWindow>
#include <QObject>
#include <QDebug>
#include "graphicsscene.h"

class Line: public GraphicsScene
{
public:
    Line();
    void drawLine(int x1, int y1, int x2, int y2);
};

#endif // LINE_H
