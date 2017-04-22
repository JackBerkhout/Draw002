#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
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

//    // Create a timer
//    timer = new QTimer(this);
//    // Setup signal and slot
//    connect(timer, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
//    // Setup interval in msec
//    timer->start(1000);

    scene->setBackgroundBrush(Qt::black);

    //    connect(scene, SIGNAL(GraphicsScene::changedMousePosition(QPointF mousePoint)), this, SLOT(MainWindow::onChangedMousePosition(QPointF mousePoint)));

    connect(scene, &GraphicsScene::changedMousePosition, this, &MainWindow::onChangedMousePosition);
    connect(scene, &GraphicsScene::changedNumber, this, &MainWindow::onChangedNumber);

    ui->label->setText(QString("hello"));
    ui->graphicsView->setMouseTracking(true);

    scene->setNumber(0);

    QString fileName = QFileDialog::getOpenFileName(this,"Open Image File",QDir::currentPath());
    QImage image(fileName);
    QPixmap *pixmap = new QPixmap;
    pixmap->fromImage(image);

    float minR = 1.0;
    float maxR = 0.0;
    float minG = 1.0;
    float maxG = 0.0;
    float minB = 1.0;
    float maxB = 0.0;
    float minGray = 1.0;
    float maxGray = 0.0;
    QImage *myImage = new QImage;
    myImage->load(fileName);

    QString filename = "Data.txt";
//    QByteArray filename2 = filename.toLocal8Bit();
//    char *filename3 = filename2.data();
    char *filename3 = filename.toLocal8Bit().data();

    if (remove(filename3) != 0) {
        perror("Error deleting file");
    }
    int fileOpened = false;
    QFile file(filename);
    QTextStream stream(&file);
    if (file.open(QIODevice::ReadWrite)) {
        fileOpened = true;
    }

    // Get minimum and maximum values to enhance the contrast, if needed.
    for (int y = 0; y < myImage->height(); ++y) {
        for (int x = 0; x < myImage->width(); ++x) {
            QRgb pixel = myImage->pixel(x, y);
            int H, S, V;
            int R, G, B;
            QColor color(pixel);
            color.getHsv(&H,&S,&V);
            color.getRgb(&R,&G,&B);
            float Rf = (float)R / 255.0f;
            float Gf = (float)G / 255.0f;
            float Bf = (float)B / 255.0f;
            if (minR > Rf) {
                minR = Rf;
            }
            if (maxR < Rf) {
                maxR = Rf;
            }
            if (minG > Gf) {
                minG = Gf;
            }
            if (maxG < Gf) {
                maxG = Gf;
            }
            if (minB > Bf) {
                minB = Bf;
            }
            if (maxB < Bf) {
                maxB = Bf;
            }
            float Gray = rgb2gray(Rf, Gf, Bf);
            if (minGray > Gray) {
                minGray = Gray;
            }
            if (maxGray < Gray) {
                maxGray = Gray;
            }
        }
    }

    // Calculate the factor to enhance the contrast
    float Multiply = 1.0f / (maxGray - minGray);

    double BitmapXSize = (double) myImage->width() * 0.9;
    double BitmapYSize = (double) myImage->height() * 0.9;
    double Pitch = 1.0f;
    double BitmapXPostion;
    double BitmapYPostion;
    double ScaleX = BitmapXSize / (double)(myImage->width()-1);
    double ScaleY = BitmapYSize / (double)(myImage->height()-1);
    bool   FlipX = false;
    bool   FlipY = false;
    double Noise = 0.09f;   // Good value tested: 0.09 (0.06f - 0.12f)

    for (BitmapYPostion = 0; BitmapYPostion < BitmapYSize; BitmapYPostion += Pitch) {
        for (BitmapXPostion = 0; BitmapXPostion < BitmapXSize; BitmapXPostion += Pitch) {
            int H, S, V;
            int R, G, B;
            int RLT, GLT, BLT;
            int RRT, GRT, BRT;
            int RLB, GLB, BLB;
            int RRB, GRB, BRB;

            int IndexT = (int) (BitmapYPostion/ScaleY);
            int IndexB = (int) IndexT + 1;
            double WeightB = (BitmapYPostion/ScaleY) - floor(BitmapYPostion/ScaleY);
            double WeightT = 1.0 - WeightB;

            int IndexR = (int) (BitmapXPostion/ScaleX);
            int IndexL = (int) IndexR + 1;
            double WeightL = (BitmapXPostion/ScaleX) - floor(BitmapXPostion/ScaleX);
            double WeightR = 1.0 - WeightL;
            bool inRange = true;
            if ((IndexT < 0) || (IndexT >= myImage->height())) {
                inRange = false;
            }
            if ((IndexB < 0) || (IndexB >= myImage->height())) {
                inRange = false;
            }
            if ((IndexR < 0) || (IndexR >= myImage->width())) {
                inRange = false;
            }
            if ((IndexL < 0) || (IndexL >= myImage->width())) {
                inRange = false;
            }
            if (!inRange) {
                continue;
            }
            QRgb pixelLT = myImage->pixel(IndexL, IndexT);
            QRgb pixelRT = myImage->pixel(IndexR, IndexT);
            QRgb pixelLB = myImage->pixel(IndexL, IndexB);
            QRgb pixelRB = myImage->pixel(IndexR, IndexB);
            QColor colorLT(pixelLT);
            QColor colorRT(pixelRT);
            QColor colorLB(pixelLB);
            QColor colorRB(pixelRB);
            colorLT.getRgb(&RLT,&GLT,&BLT);
            colorRT.getRgb(&RRT,&GRT,&BRT);
            colorLB.getRgb(&RLB,&GLB,&BLB);
            colorRB.getRgb(&RRB,&GRB,&BRB);
            float GrayLT = rgb2gray((float)RLT / 255.0f, (float)GLT / 255.0f, (float)BLT / 255.0f);
            float GrayRT = rgb2gray((float)RRT / 255.0f, (float)GRT / 255.0f, (float)BRT / 255.0f);
            float GrayLB = rgb2gray((float)RLB / 255.0f, (float)GLB / 255.0f, (float)BLB / 255.0f);
            float GrayRB = rgb2gray((float)RRB / 255.0f, (float)GRB / 255.0f, (float)BRB / 255.0f);
            double GrayT = (GrayLT * WeightL + GrayRT * WeightR);
            double GrayB = (GrayLB * WeightL + GrayRB * WeightR);
            double GrayValue = (GrayT  * WeightT + GrayB  * WeightB);

            int x = (int) BitmapXPostion;
            int y = (int) BitmapYPostion;
            if ((x > myImage->width()) || (y > myImage->height())) {
                continue;
            }
            QRgb pixel = myImage->pixel(x, y);
            QColor color(pixel);
//            color.getHsv(&H,&S,&V);
//            color.getRgb(&R,&G,&B);
//            float Rf = (float)R / 255.0f;
//            float Gf = (float)G / 255.0f;
//            float Bf = (float)B / 255.0f;
//            float Gray = rgb2gray(Rf, Gf, Bf);

            float GrayNew = (GrayValue - minGray) * Multiply;
            double RandomValue = double(rand() % 1000) / 1000.0f - 0.5f;
            GrayNew += RandomValue * Noise;
            if (GrayNew > 1.0)
                GrayNew = 1.0;
            if (GrayNew < 0.0)
                GrayNew = 0.0;
            if (Dither (256, x, y) <= (short) (GrayNew*255) + 0.5)
            {
              // we have a punch!!
                color.setRgb(255, 255, 255);
                if (fileOpened) {
                    stream << "G00 X" << x << " Y" << y << endl;
                    stream << "M07"<< endl;
                    stream << "M08"<< endl;
                }
            }
            else {
                color.setRgb(0, 0, 0);
            }
            drawPixel(x, y, color);
        }
    }

//    int x, y;
//    x = 1200;
//    y = 700;
//    QRgb pixel = myImage->pixel(x, y);
//    int H, S, V;
//    int R, G, B;
//    QColor color(pixel);
//    color.getHsv(&H,&S,&V);
//    color.getRgb(&R,&G,&B);


//    ui->label->setText(QString("%1 %2 %3").arg(H).arg(S).arg(V));
    ui->label->setText(QString("Color (min-max) R(%1-%2) G(%3-%4) B(%5-%6) Gray(%7-%8) ").arg(minR).arg(maxR).arg(minG).arg(maxG).arg(minB).arg(maxB).arg(minGray).arg(maxGray));
//    ui->label->setText(QString("Color at (%1,%2): R=%3 G=%4 B=%5").arg(x).arg(y).arg(R).arg(G).arg(B));
//    remove(myImage); //to free the memory?
//    myImage = QImage();

    double RandomValue = double(rand() % 1000) / 10000.0f;
    ui->label->setText(QString("RandomValue: %1").arg(RandomValue));

////    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(*myImage);
////    ui->graphicsView->fitInView(item, Qt::KeepAspectRatio);

//    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
//    scene->addItem(item);


//    QPoint  local_pt = view->mapFromGlobal(e->globalPos());
//    QPointF img_coord_pt = view->mapToScene(local_pt);

//    QPointF mousePosition = event->pos();
    QPointF mousePosition;
    mousePosition.setX(0);
    mousePosition.setY(0);
//    QRgb rgbValue = pixmap().toImage().pixel(mousePosition.x(), mousePostion.y());
//    ui->label->setText(QString("%1").arg(myNumber));
//    unsigned char intensity = pixmap->. .pixel(x,y);


//    QFile file(filename);
//    if (file.open(QIODevice::ReadWrite)) {
//        QTextStream stream(&file);
//        stream << "Start" << endl;
//        for (int i = 0; i < 8; i++) {
//            int BitVal = (int)pow((float)2, (float)i);
//            stream << "i: " << i << ", Bitval: " << BitVal << endl;
//        }
//        stream << "End" << endl;
//    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

float MainWindow::rgb2gray(float r, float g, float b) {
    return r * 0.299f + g * 0.587f + b * 0.114f;
}

int MainWindow::Pxl(int A, int B)
{
  return (int)pow((float)A, (float)B);
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
//    QColor color;
//    color.setRgb(0, 0, 0);
//    QPen pen;
//    pen.setColor(color);
//    pen.setWidth(1);
//    QBrush brush;
//    brush.setColor(color);
//    brush.setStyle(Qt::SolidPattern);

//    scene->addRect(0, 0, width()+1000, height()+1000, pen, brush);

    scene->clear();
}

void MainWindow::drawPixel(int x, int y, QColor color)
{
    QPen pen;
    pen.setColor(color);
    pen.setWidth(1);
    scene->addLine(x, y, x, y, pen);
}

int MainWindow::Dither(int Resolution, int SizeX, int SizeY)
{
  SizeX = SizeX & Resolution -1;
  SizeY = SizeY & Resolution -1;
  int Value = 0;
  for (int Bit = 0; Bit <= 3; Bit++)
  {
    int BitVal = Pxl(2, Bit);
    int Bitindex = ((SizeY & BitVal) / BitVal) + (((SizeX & BitVal) / BitVal) * 2);
    if (Bitindex == 3)
      Bitindex = 1;
    else if (Bitindex == 1)
      Bitindex = 3;

    int Tmp = (64 / (short) Pxl(BitVal, 2));
    Value += Tmp * Bitindex;
  }
  return (Value);
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
    QColor color;
    color.setRgb(255, 255, 255);
    drawPixel(10, 10, color);
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

void MainWindow::MyTimerSlot()
{
    //    GraphicsScene *graphicsScene = new GraphicsScene();
    //    QPointF mousePoint = graphicsScene->getMousePoint();

    //    GraphicsScene graphicsScene; //Create instance
    //    QPointF mousePoint = scene->getMousePoint();

    //    int MouseX   = scene->getMouseX();
    //    int MouseY   = scene->getMouseY();
//    int myNumber = scene->getNumber();
//    Count++;
    qDebug() << "Timer...";
//    qDebug() << "Timer..." << QString("Count: %1").arg(Count);
    //    qDebug() << "Timer..." << QString("Mouse: %1, %2").arg(mousePoint.x()).arg(mousePoint.y());
    //    qDebug() << "Timer..." << QString("Mouse: %1, %2").arg(MouseX).arg(MouseY);
    //    qDebug() << "Timer..." << QString("Number: %1").arg(myNumber);
    //    statusBar()->showMessage(QString("x,y: %1, %2").arg(mousePoint.x()).arg(mousePoint.y()));
    //    ui->label->setText(QString("%1").arg(myNumber));
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
