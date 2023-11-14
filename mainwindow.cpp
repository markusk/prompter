#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // create scroll timer
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(scrollPrompter()) );

    scrollValueY = 0;

    textDirection = Qt::AlignCenter;
    textColor = Qt::white;
    textPen = QPen(textColor);
    backgroundColor = Qt::black;

    // start GUI
    ui->setupUi(this);

    // get scroll speed from slider value
    scrollSpeed = ui->verticalSliderScrollSpeed->value();

    // get font size from slider value
    fontSize = ui->verticalSliderFontSize->value();
    textFont.setPixelSize(fontSize);

    // get prompter widget size for the QImage
    width = ui->openGLWidget->width();
    height= ui->openGLWidget->height();

    // add example text to text Edit
    ui->textEdit->setText("Hello world...");

    // create prompter image
    imagePrompterText = QImage(QSize(width, height),QImage::Format_RGB32);

    // create prompter text
    updatePrompterImage();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    // show image in openGL widget
    QPainter painter;

    // paiting into openFLWidget in my form
    painter.begin(ui->openGLWidget);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw image into openGL widget
    painter.drawImage( QPoint(1, scrollValueY), imagePrompterText);

    painter.end();
}


bool MainWindow::updatePrompterImage()
{
    // re-create prompter image <<< does create glitch <<<
    // imagePrompterText = QImage(QSize(width, height),QImage::Format_RGB32);

    QPainter painter(&imagePrompterText);

    // erase area inside the rectangle
    painter.eraseRect(0,0, width, height);
    painter.fillRect(0,0,width, height, backgroundColor);

    painter.setPen(textPen);
    painter.setFont(textFont);

    painter.drawText(QRect(0, 0, width, height), textDirection, ui->textEdit->toPlainText());

    painter.end();

    // mirror image horicontally
    imagePrompterText = imagePrompterText.mirrored(true, false);

    // update GUI
    update();
}


void MainWindow::scrollPrompter()
{
    scrollValueY--;
    update();
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
    updatePrompterImage();
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
