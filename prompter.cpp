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
    fontSizePrompter = ui->verticalSliderFontSize->value();

    // set default fonts
    textEditFont = QFont("Arial", fontSizeTextEdit);
    prompterFont = QFont("Arial", fontSizePrompter);

    // set GUI to values from here
    ui->fontComboBoxTextEdit->setCurrentFont(textEditFont);
    ui->fontComboBoxPrompter->setCurrentFont(textEditFont);
    ui->spinBoxFontSizeTextEdit->setValue(fontSizeTextEdit);
    ui->spinBoxFontSizePrompter->setValue(fontSizePrompter);

    // set fonts in editor and promnpter
    ui->textEdit->setFont(textEditFont);
    ui->openGLWidget->setFont(prompterFont);

    // get prompter widget size for the QImage
    width = ui->openGLWidget->width();
    height = ui->openGLWidget->height();

    // add example text to text Edit
    ui->textEdit->setText("Hello world...");

    // get current screen resolution
    QScreen *screen = qApp->screens().at(0);
    qDebug() << screen->geometry() << screen->physicalSize() << screen->physicalDotsPerInch();
    //height = screen->geometry().height();
    //width  = screen->geometry().width();

    // store original position of the prompter widget (for later "full screen" mode)
    prompterWidgetGeometryX = ui->openGLWidget->x();
    prompterWidgetGeometryY = ui->openGLWidget->y();
    prompterWidgetGeometryHeight = ui->openGLWidget->height();
    prompterWidgetGeometryWidth = ui->openGLWidget->width();

    // create prompter image (with the max. size of the screen if we go full-screen)
//    imagePrompterText = QImage(QSize(width, height),QImage::Format_RGB32);

    // create prompter text
    updatePrompterImage();

    // update prompter text live, when text field changes
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(updatePrompterImage()) );

    // update chosen font
    connect(ui->fontComboBoxTextEdit, SIGNAL(currentFontChanged(QFont)), this, SLOT(onFontComboBoxTextEditChanged(QFont)));
    connect(ui->fontComboBoxPrompter, SIGNAL(currentFontChanged(QFont)), this, SLOT(onFontComboBoxPrompterChanged(QFont)));

    //
    connect(ui->fontComboBoxTextEdit,     SIGNAL(currentFontChanged(const QFont&)), this, SLOT(updateTextEditFont()));
    connect(ui->spinBoxFontSizeTextEdit,  SIGNAL(valueChanged(int)),                this, SLOT(updateTextEditFont()));
}


Prompter::~Prompter()
{
    delete ui;
}


void Prompter::paintEvent(QPaintEvent *event)
{
    /// added for fullscreen test, does not help at all
    // updating QImage with content etc...
    //updatePrompterImage();

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


void Prompter::updatePrompterImage()
{
    // get current widget size
    width  = ui->openGLWidget->width();
    height = ui->openGLWidget->height();

    imagePrompterText = QImage(QSize(width, height),QImage::Format_RGB32);

    QPainter painter(&imagePrompterText);

    //painter.begin(ui->openGLWidget);

    // erase area inside the rectangle    
    painter.eraseRect(0,0, width, height);
    painter.fillRect(0,0,width, height, backgroundColor);

    // use font size (if changed on slider, @sa on_verticalSliderFontSize_valueChanged slot)
    prompterFont.setPixelSize(fontSizePrompter);
    painter.setPen(textPen);
    painter.setFont(prompterFont);

    if (ui->checkBoxWordWrap->isChecked())
        painter.drawText(QRect(0, 0, width, height), textDirection|Qt::AlignVCenter|Qt::TextWordWrap, ui->textEdit->toPlainText());
    else
        painter.drawText(QRect(0, 0, width, height), textDirection|Qt::AlignVCenter,                  ui->textEdit->toPlainText());

//    painter.end();

    // mirror image horizontally
    if (ui->checkBoxMirror->isChecked())
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


void Prompter::on_pushButtonExit_clicked()
{
    QApplication::exit();
}


void Prompter::on_verticalSliderScrollSpeed_valueChanged()
{
    scrollSpeed = ui->verticalSliderScrollSpeed->value();
    ui->labelScrollSpeed->setNum(scrollSpeed);
    timer->setInterval(scrollSpeed);
}


void Prompter::on_verticalSliderFontSize_valueChanged()
{
    fontSizePrompter = ui->verticalSliderFontSize->value();
    ui->labelFontSize->setNum(fontSizePrompter);

    // update image
    updatePrompterImage();
}


void Prompter::on_spinBoxFontSizeTextEdit_valueChanged(int size)
{
}

void Prompter::updateTextEditFont()
{
    fontSizeTextEdit = ui->spinBoxFontSizeTextEdit->value();

    QString fontFamily = ui->fontComboBoxTextEdit->currentFont().family();

    // Create a new font with the selected size and family
    QFont newFont(fontFamily, fontSizeTextEdit);

    // Set the font for the text edit
    ui->textEdit->setFont(newFont);
}


void Prompter::on_spinBoxFontSizePrompter_valueChanged(int size)
{
    fontSizePrompter = size;
    prompterFont = QFont("Arial", fontSizePrompter);

    // set fonts in editor and promnpter
    ui->openGLWidget->setFont(prompterFont);

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


void Prompter::on_checkBoxMirror_stateChanged()
{
    // update image
    updatePrompterImage();
}


void Prompter::on_pushButtonFullScreen_clicked()
{
    //qDebug("Test button clicked");
    static bool fullScreen = false;


    // toggle
    fullScreen = !fullScreen;

    if (fullScreen)
    {
        /*
        // mark widget as window to make it full-screen
        ui->openGLWidget->setWindowFlag(Qt::Window);
        // show it again! @sa https://doc.qt.io/qt-5/qwidget.html#windowFlags-prop
        ui->openGLWidget->show();
        ui->openGLWidget->showFullScreen();
        */

        // start scrolling
        emit on_pushButtonScroll_clicked();

        ui->textEdit->hide();
        ui->labelLink->hide();
        ui->groupBoxAlignment->hide();
        ui->groupBoxWarpMirror->hide();
        ui->verticalSliderFontSize->hide();
        ui->verticalSliderScrollSpeed->hide();
        ui->fontComboBoxTextEdit->hide();
        ui->fontComboBoxPrompter->hide();

        // enter fullscreen for main window
        QMainWindow::showFullScreen();

        // set openGLWidget to full main window size (not full-Screen)
        ui->openGLWidget->setGeometry(0, 0, ui->centralwidget->width(), ui->centralwidget->height());
    }
    else
    {
        /*
        ui->openGLWidget->setWindowFlag(Qt::Window, false);
        ui->openGLWidget->show();
        */

        // return from full screen
        QMainWindow::showNormal();

        // stop scrolling
        emit on_pushButtonScroll_clicked();

        // restore openGLWidget to size of pgm start
        ui->openGLWidget->setGeometry(prompterWidgetGeometryX, prompterWidgetGeometryY, prompterWidgetGeometryWidth , prompterWidgetGeometryHeight);

        ui->fontComboBoxPrompter->show();
        ui->fontComboBoxTextEdit->show();
        ui->verticalSliderScrollSpeed->show();
        ui->verticalSliderFontSize->show();
        ui->groupBoxWarpMirror->show();
        ui->groupBoxAlignment->show();
        ui->labelLink->show();
        ui->textEdit->show();
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


void Prompter::onFontComboBoxTextEditChanged(const QFont& font)
{
    // Do something with the selected font, for example, update a label
    ui->textEdit->setFont(font);
}


void Prompter::onFontComboBoxPrompterChanged(const QFont& font)
{
    //ui->textEdit->setFont(font);
}
