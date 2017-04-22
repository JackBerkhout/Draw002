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

    bool eventFilter(QObject *obj, QEvent *event);

    void eraseScreen(void);

    QPointF mousePoint;

    int Count;

//    GraphicsScene graphicsScene;

public slots:
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
