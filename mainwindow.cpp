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

    elapsed = 0;

    scrollValueY = 0;

    textDirection = Qt::AlignCenter;

    // start GUI
    ui->setupUi(this);

    // get scroll speed from slider value
    scrollSpeed = ui->verticalSliderScrollSpeed->value();

    // get font size from slider value
    fontSize = ui->verticalSliderFontSize->value();
    textFont.setPixelSize(fontSize);

    // create prompter text
    createImage("Hello world!", Qt::white);
}


MainWindow::~MainWindow()
{
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
    painter.drawImage( QPoint(1, scrollValueY), imagePrompterText);

    painter.end();
}


void MainWindow::animate()
{
    scrollValueY--;
    update();
}


bool MainWindow::createImage(QString text, QColor color)
{
    // create image
    imagePrompterText = QImage(QSize(width, height),QImage::Format_RGB32);

    QPainter painter(&imagePrompterText);
    //painter = new QPainter(image);

    painter.setPen(textPen);
    painter.setFont(textFont);

    painter.drawText(QRect(0, 0, width, height), textDirection, text);

    painter.end();

    // mirror image horicontally
    imagePrompterText = imagePrompterText.mirrored(true, false);
}


void MainWindow::on_pushButtonTest_clicked()
{
    qDebug("pushButton clicked");

    if (timer->isActive())
    {
        timer->stop();
        ui->pushButtonTest->setText("Scroll");
    }
    else
    {
        timer->start(scrollSpeed);
        ui->pushButtonTest->setText("Stop");
    }
}


void MainWindow::on_pushButtonReset_clicked()
{
    // change text alignment
    if (ui->radioButtonLeft->isChecked())
        textDirection = Qt::AlignLeft;
    else
        if (ui->radioButtonCentered->isChecked())
            textDirection = Qt::AlignCenter;
        else
            textDirection = Qt::AlignRight;

    scrollValueY = 0;

    // recreate image
//    createImage("Hello world!", Qt::white);

    // update GUI
    update();
}


void MainWindow::on_verticalSliderScrollSpeed_valueChanged()
{
    scrollSpeed = ui->verticalSliderScrollSpeed->value();
    ui->labelScrollSpeed->setNum(scrollSpeed);
    timer->setInterval(scrollSpeed);
}


void MainWindow::on_verticalSliderFontSize_valueChanged()
{
    fontSize = ui->verticalSliderFontSize->value();
    ui->labelFontSize->setNum(fontSize);
}
