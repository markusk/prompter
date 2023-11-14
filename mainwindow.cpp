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
//    static int i=0;

//    i++;
//    qDebug() << i <<". paintEvent called.";


    // paint now
    QPainter painter;

    // paiting into openFLWidget in my form
    painter.begin(ui->openGLWidget);
    painter.setRenderHint(QPainter::Antialiasing);
/*

    painter.translate(100, 100);

    //painter.save();
    //painter.rotate(180);


    painter.setPen(textPen);
    painter.setFont(textFont);
    painter.drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Qt"));
    //painter.restore();

    // not needed any longer! this->paint(&painter, event, elapsed);
    painter.end();
*/
    QImage image(QSize(width,height),QImage::Format_RGB32);

//    QPainter painter(&image);

/*
    painter.setBrush(QBrush(bgColor));
    painter.fillRect(QRectF(0,0,width,height),Qt::darkGreen);
    // qDebug() << (width-w-offset)/2 << "\t" << (height-h-offset)/2 << "\t" << w << "\t" << h;
    QRect aRect = QRect( (width-w)/2, (height-h)/2, w, h );
    QRect aRectOffset = QRect( (width-w+offset)/2, (height-h+offset)/2, w-(offset/2), h-(offset/2) );
    painter.fillRect(QRect(aRect),Qt::white);
*/
    painter.setPen(QPen(Qt::black));
    painter.setFont(QFont( "Courier", 20) );

    // add text to image
    painter.drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("hello world"));

    // mirror image horicontally
//    image = image.mirrored(true, false);

    // draw image into openGL widget
    painter.drawImage( QPointF(1.0, 0.0), image);

    painter.end();
}


void MainWindow::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}


bool MainWindow::createImage(QString text, QColor aColor)
{
    imagePrompterText = QImage(QSize(width, height),QImage::Format_RGB32);
    // QImage image(QSize(width, height),QImage::Format_RGB32);
    //image = new QImage(width, height, QImage::Format_RGB32);

    QPainter painter(&imagePrompterText);
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
    imagePrompterText = imagePrompterText.mirrored(true, false);

    // assign image to Pixmap 'pix'
    pix = QPixmap(width, height);
    pix.fromImage(imagePrompterText);

    // display image/pixmap in QLabel
    //ui->labelImage->setStyleSheet("border-image:url(:/2.png);");
    ui->labelImage->setPixmap(pix);

/*
    // save image to disk
    QDir aDir = QDir(path);
    if ( aDir.mkpath(path) )
        return image.save(path + "/" + imageName);
    else
        return image.save(imageName);
*/
}


void MainWindow::on_pushButton_clicked()
{
    //qDebug("pushButton clicked");
    createImage("Hello world!", Qt::white);

    // start scroll timer
    // timer->start(50);

}
