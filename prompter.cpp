#include "prompter.h"
#include "./ui_prompter.h"


Prompter::Prompter(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Prompter)
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
    height = ui->openGLWidget->height();

    // add example text to text Edit
    ui->textEdit->setText("Hello world...");

    // get current screen resolution
    QScreen *screen = qApp->screens().at(0);
    qDebug() << screen->geometry() << screen->physicalSize() << screen->physicalDotsPerInch();
    height = screen->geometry().height();
    width  = screen->geometry().width();

    // create prompter image (with the max. size of the screen if we go full-screen)
//    imagePrompterText = QImage(QSize(width, height),QImage::Format_RGB32);

    // create prompter text
    updatePrompterImage();

    // update prompter text live, when text field changes
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(updatePrompterImage()) );
}


Prompter::~Prompter()
{
    delete ui;
}


void Prompter::paintEvent(QPaintEvent *event)
{
    // show image in openGL widget
    QPainter painter;

    // paiting into openFLWidget in my form
    painter.begin(ui->openGLWidget);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw image into openGL widget
    painter.drawImage( QPoint(1, scrollValueY), imagePrompterText);

    painter.end();

    //qDebug() << "PaintEvent!  OpenGL widget size is " << ui->openGLWidget->width() << "x" << ui->openGLWidget->height();
}


bool Prompter::updatePrompterImage()
{
    imagePrompterText = QImage(QSize(width, height),QImage::Format_RGB32);

    QPainter painter(&imagePrompterText);

    // get prompter widget size for the QImage
    height = ui->openGLWidget->height();

    //painter.begin(ui->openGLWidget);

    // erase area inside the rectangle    
    painter.eraseRect(0,0, width, height);
    painter.fillRect(0,0,width, height, backgroundColor);

    // use font site (if changed on slider, @sa on_verticalSliderFontSize_valueChanged slot)
    textFont.setPixelSize(fontSize);
    painter.setPen(textPen);
    painter.setFont(textFont);

    if (ui->checkBoxWordWrap->isChecked())
        painter.drawText(QRect(0, 0, width, height), textDirection|Qt::AlignVCenter|Qt::TextWordWrap, ui->textEdit->toPlainText());
    else
        painter.drawText(QRect(0, 0, width, height), textDirection|Qt::AlignVCenter, ui->textEdit->toPlainText());

    painter.end();

    // mirror image horicontally
    imagePrompterText.mirror(true, false);

    // update GUI
    update();

    //qDebug() << "updatePrompterImage update called...";
}


void Prompter::scrollPrompter()
{
    scrollValueY--;
    update();
}


void Prompter::on_pushButtonScroll_clicked()
{
    if (timer->isActive())
    {        
        timer->stop();
        ui->pushButtonScroll->setText("Scroll");
    }
    else
    {
        // update image
        updatePrompterImage();

        timer->start(scrollSpeed);
        ui->pushButtonScroll->setText("Stop");
    }
}


void Prompter::on_pushButtonReset_clicked()
{
    scrollValueY = 0;

    // update image
    updatePrompterImage();
}


void Prompter::on_verticalSliderScrollSpeed_valueChanged()
{
    scrollSpeed = ui->verticalSliderScrollSpeed->value();
    ui->labelScrollSpeed->setNum(scrollSpeed);
    timer->setInterval(scrollSpeed);
}


void Prompter::on_verticalSliderFontSize_valueChanged()
{
    fontSize = ui->verticalSliderFontSize->value();
    ui->labelFontSize->setNum(fontSize);

    // update image
    updatePrompterImage();
}


void Prompter::on_radioButtonLeft_clicked()
{
    // change text alignment
    if (ui->radioButtonLeft->isChecked())
        textDirection = Qt::AlignLeft;

    // update image
    updatePrompterImage();
}


void Prompter::on_radioButtonCentered_clicked()
{
    // change text alignment
    if (ui->radioButtonCentered->isChecked())
        textDirection = Qt::AlignHCenter;

    // update image
    updatePrompterImage();
}


void Prompter::on_radioButtonRight_clicked()
{
    // change text alignment
    if (ui->radioButtonRight->isChecked())
        textDirection = Qt::AlignRight;

    // update image
    updatePrompterImage();
}


void Prompter::on_checkBoxWordWrap_stateChanged()
{
    // update image
    updatePrompterImage();
}


void Prompter::on_pushButtonTest_clicked()
{
    //qDebug("Test button clicked");
    static bool fullScreen = false;


    // toggle
    fullScreen = !fullScreen;

    if (fullScreen)
    {
        // mark widget as window to make it full-screen
        ui->openGLWidget->setWindowFlag(Qt::Window);
        // show it again! @sa https://doc.qt.io/qt-5/qwidget.html#windowFlags-prop
        ui->openGLWidget->show();
        ui->openGLWidget->showFullScreen();
    }
    else
    {
        ui->openGLWidget->setWindowFlag(Qt::Window, false);
        ui->openGLWidget->show();
    }

    updatePrompterImage();

/*
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("prompter");
    msgBox.setInformativeText("https://github.com/markusk/prompter");
    msgBox.exec();
 */
}
