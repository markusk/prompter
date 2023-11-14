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

    // create prompter text
    createImage("Hello world!", Qt::white);


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
    /*
    // paint now
    QPainter painter;

    // paiting into openFLWidget in my form
    painter.begin(ui->openGLWidget);
    painter.setRenderHint(QPainter::Antialiasing);


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



    // show image in openGL widget
    QPainter painter;

    // paiting into openFLWidget in my form
    painter.begin(ui->openGLWidget);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw image into openGL widget
    painter.drawImage( QPointF(1.0, 0.0), imagePrompterText);

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

    painter.setPen(textPen);
    painter.setFont(textFont);

    // add text to image
    //painter.drawText(QPointF(1.0, 0.0), text);
    painter.drawText(QRect(0, 0, 100, 100), Qt::AlignCenter, QStringLiteral("hello"));

    painter.end();

    // mirror image horicontally
//    imagePrompterText = imagePrompterText.mirrored(true, false);

    /*
    // assign image to Pixmap 'pix'
        pix = QPixmap(width, height);
    pix.fromImage(imagePrompterText);

    // display image/pixmap in QLabel
    //ui->labelImage->setStyleSheet("border-image:url(:/2.png);");
    ui->labelImage->setPixmap(pix);
*/
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
    // paint now
}
