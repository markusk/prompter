#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    //delete pix;
    //delete painter;
    //delete image;
    delete ui;
}


void MainWindow::paint(QPainter *painter, QPaintEvent *event, int elapsed)
{

}


bool MainWindow::createImage(QString text, QString path, QString imageName, QColor aColor)
{
    QImage image(QSize(width,height),QImage::Format_RGB32);
    //image = new QImage(width, height, QImage::Format_RGB32);

    QPainter painter(&image);
    //painter = new QPainter(image);

    painter.setBrush(QBrush(aColor));
    painter.fillRect(QRectF(0,0,width,height),Qt::darkGreen);
    // qDebug() << (width-w-offset)/2 << "\t" << (height-h-offset)/2 << "\t" << w << "\t" << h;
    QRect aRect = QRect( (width-w)/2, (height-h)/2, w, h );
    QRect aRectOffset = QRect( (width-w+offset)/2, (height-h+offset)/2, w-(offset/2), h-(offset/2) );
    painter.fillRect(QRect(aRect),Qt::white);
    painter.setPen(QPen(Qt::black));
    painter.setFont(QFont( "Courier", 20) );

    // add text to image
    painter.drawText(QRect(aRectOffset),text);

    // mirror image horicontally
    image = image.mirrored(true, false);

    // assign image to Pixmap 'pix'
    pix = QPixmap(width, height);
    pix.fromImage(image);

    // display image/pixmap in QLabel
    //ui->labelImage->setStyleSheet("border-image:url(:/2.png);");
    ui->labelImage->setPixmap(pix);

    // save image to disk
    QDir aDir = QDir(path);
    if ( aDir.mkpath(path) )
        return image.save(path + "/" + imageName);
    else
        return image.save(imageName);
}


void MainWindow::on_pushButton_clicked()
{
    //qDebug("pushButton clicked");
    createImage("Ein einfacher Text\nDer auch mal länger sein kann.", "./", "TestBild.png", Qt::green);
}
