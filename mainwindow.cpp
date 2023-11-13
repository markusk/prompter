#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // create scroll timer
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(animate()) );
    //QObject::connect(scrollTimer, &QTimer::timeout, formWidget, scrollPrompterText);

    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    background = QBrush(QColor(64, 32, 64));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);

    elapsed = 0;


    // start GUI
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    //delete pix;
    //delete painter;
    //delete image;
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    static int i=0;

    i++;
    qDebug() << i <<". paintEvent called.";


    // paint now
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);


    painter.fillRect(event->rect(), background);
    painter.translate(100, 100);
    //! [1]

    //! [2]
    painter.save();
    painter.setBrush(circleBrush);
    painter.setPen(circlePen);
    painter.rotate(elapsed * 0.030);

    qreal r = elapsed / 1000.0;
    int n = 30;
    for (int i = 0; i < n; ++i) {
        painter.rotate(30);
        qreal factor = (i + r) / n;
        qreal radius = 0 + 120.0 * factor;
        qreal circleRadius = 1 + factor * 20;
        painter.drawEllipse(QRectF(radius, -circleRadius,
                                    circleRadius * 2, circleRadius * 2));
    }
    painter.restore();
    //! [2]

    //! [3]
    painter.setPen(textPen);
    painter.setFont(textFont);
    painter.drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Qt"));


    // not needed any longer! this->paint(&painter, event, elapsed);
    painter.end();
}


/*
void MainWindow::paint(QPainter *painter, QPaintEvent *event, int elapsed)
{
    static int i=0;

    i++;
    qDebug() << i <<". paint called.";

    painter->fillRect(event->rect(), background);
    painter->translate(100, 100);
    //! [1]

    //! [2]
    painter->save();
    painter->setBrush(circleBrush);
    painter->setPen(circlePen);
    painter->rotate(elapsed * 0.030);

    qreal r = elapsed / 1000.0;
    int n = 30;
    for (int i = 0; i < n; ++i) {
        painter->rotate(30);
        qreal factor = (i + r) / n;
        qreal radius = 0 + 120.0 * factor;
        qreal circleRadius = 1 + factor * 20;
        painter->drawEllipse(QRectF(radius, -circleRadius,
                                    circleRadius * 2, circleRadius * 2));
    }
    painter->restore();
    //! [2]

    //! [3]
    painter->setPen(textPen);
    painter->setFont(textFont);
    painter->drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Qt"));
}
*/


void MainWindow::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
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

    // start scroll timer
    // timer->start(50);

}
