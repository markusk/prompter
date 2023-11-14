#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // create scroll timer
    timer = new QTimer(this);

    // scroll one pixel on timeout
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

    // update prompter text live, when text field changes
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(updatePrompterImage()) );
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

    // use font site (if changed on slider, @sa on_verticalSliderFontSize_valueChanged slot)
    textFont.setPixelSize(fontSize);
    painter.setPen(textPen);
    painter.setFont(textFont);

    painter.drawText(QRect(0, 0, width, height), textDirection|Qt::AlignVCenter, ui->textEdit->toPlainText());

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
    //qDebug("pushButton clicked");

    if (timer->isActive())
    {        
        timer->stop();
        ui->pushButtonTest->setText("Scroll");
    }
    else
    {
        // update image
        updatePrompterImage();

        timer->start(scrollSpeed);
        ui->pushButtonTest->setText("Stop");
    }
}


void MainWindow::on_pushButtonReset_clicked()
{
    scrollValueY = 0;

    // update image
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

    // update image
    updatePrompterImage();
}


void MainWindow::on_radioButtonLeft_clicked()
{
    // change text alignment
    if (ui->radioButtonLeft->isChecked())
        textDirection = Qt::AlignLeft;

    // update image
    updatePrompterImage();
}


void MainWindow::on_radioButtonCentered_clicked()
{
    // change text alignment
    if (ui->radioButtonCentered->isChecked())
        textDirection = Qt::AlignHCenter;

    // update image
    updatePrompterImage();
}


void MainWindow::on_radioButtonRight_clicked()
{
    // change text alignment
    if (ui->radioButtonRight->isChecked())
        textDirection = Qt::AlignRight;

    // update image
    updatePrompterImage();
}
