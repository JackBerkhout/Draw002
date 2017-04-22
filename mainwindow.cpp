#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qApp->installEventFilter(this);

    Count = 0;
    // A way to close the app, clicking actionQuit, without using a seperate function
    //    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    int Width = 3200;
    int Height = 2400;
    this->resize(Width, Height);
    scene = new GraphicsScene;
    ui->graphicsView->setScene(scene);
    int sceneWidth = this->width()-20;
    int sceneHeight = this->height()-160;
    ui->graphicsView->resize(sceneWidth, sceneHeight);
    ui->graphicsView->setSceneRect(0, 0, sceneWidth, sceneHeight);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

    flipVertical = false;
    if (flipVertical) {
        ui->graphicsView->scale(1, -1); // Coordinate 0,0 at Bottom-Left
    }

    statusBar()->showMessage(QString("Size: %1, %2 ").arg(Width).arg(Height));

    scene->setBackgroundBrush(Qt::black);

    connect(scene, &GraphicsScene::changedMousePosition, this, &MainWindow::onChangedMousePosition);
    connect(scene, &GraphicsScene::changedNumber, this, &MainWindow::onChangedNumber);

    ui->label->setText(QString("hello"));
    ui->graphicsView->setMouseTracking(true);

    scene->setNumber(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj)
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()));
    }
    return false;
}

void MainWindow::eraseScreen(void)
{
    scene->clear();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event)
    int sceneWidth = this->width()-20;
    int sceneHeight = this->height()-160;
    ui->graphicsView->resize(sceneWidth, sceneHeight);
    ui->graphicsView->setSceneRect(0, 0, sceneWidth-1, sceneHeight-1);
    //rubberband->setRect(ui->graphicsView->sceneRect());

}

void MainWindow::on_toolButtonDraw_clicked()
{
    scene->drawLine(100, 100, 1500, 500);
}

void MainWindow::on_toolButtonErase_clicked()
{
    eraseScreen();
}

void MainWindow::on_actionQuit_triggered()
{
    // A way to close the app, clicking actionQuit, using this function
    // Some cleanup is possible here
    close();
}

void MainWindow::onChangedMousePosition(QPointF mousePoint)
{
    //    statusBar()->showMessage(QString("x: %1 y: %2").arg(mousePoint.x()).arg(mousePoint.y()));
    statusBar()->showMessage(QString("x,y: %1, %2").arg(mousePoint.x()).arg(mousePoint.y()));
}

void MainWindow::onChangedNumber(int Number)
{
    //    statusBar()->showMessage(QString("x: %1 y: %2").arg(mousePoint.x()).arg(mousePoint.y()));
    ui->label->setText(QString("N: %1").arg(Number));
}
