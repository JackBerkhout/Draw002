#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "graphicsscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    float rgb2gray(float r, float g, float c);
    int Pxl(int A, int B);

    bool eventFilter(QObject *obj, QEvent *event);

    void eraseScreen(void);
    void drawPixel(int x, int y, QColor color);
    int Dither(int Resolution, int SizeX, int SizeY);

    QTimer *timer;
    QPointF mousePoint;

    int Count;

//    GraphicsScene graphicsScene;

public slots:
    void MyTimerSlot();
    void onChangedMousePosition(QPointF mousePoint);
    void onChangedNumber(int Number);


private slots:
    void resizeEvent(QResizeEvent* event);

    void on_toolButtonDraw_clicked();

    void on_toolButtonErase_clicked();

    void on_actionQuit_triggered();

private:
    Ui::MainWindow *ui;
    GraphicsScene *scene;

    int timerId;
    bool flipVertical;

protected:

};

#endif // MAINWINDOW_H
