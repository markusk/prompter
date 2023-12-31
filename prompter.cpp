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

    // set default fonts for GUI
    textEditFont = QFont("Arial", fontSizeTextEdit);

    // set GUI to values from here
    ui->fontComboBoxTextEdit->setCurrentFont(textEditFont);
    ui->fontComboBoxPrompter->setCurrentFont(prompterFont);

    ui->spinBoxFontSizeTextEdit->setValue(fontSizeTextEdit);
    ui->spinBoxFontSizePrompter->setValue(fontSizePrompter);

    // set fonts in editor and promnpter
    ui->textEdit->setFont(textEditFont);

    // get prompter widget size for the QImage
    width = ui->openGLWidget->width();
    height = ui->openGLWidget->height();

    // add example text to text Edit
    ui->textEdit->setText("A long time ago in a galaxy far, far away....");

    // store original position of the prompter widget (for later "full screen" mode)
    prompterWidgetGeometryX = ui->openGLWidget->x();
    prompterWidgetGeometryY = ui->openGLWidget->y();
    prompterWidgetGeometryHeight = ui->openGLWidget->height();
    prompterWidgetGeometryWidth = ui->openGLWidget->width();

    // create prompter text with given fonts and sizes NOW
    updatePrompterImage();

    // update prompter text live, when text field changes
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(updatePrompterImage()) );

    // update promoter font
    connect(ui->fontComboBoxPrompter, SIGNAL(currentFontChanged(QFont)), this, SLOT(updatePrompterImage()));

    // update textEdit field to chosen font type and size
    connect(ui->fontComboBoxTextEdit,     SIGNAL(currentFontChanged(const QFont&)), this, SLOT(updateTextEditFont()));
    connect(ui->spinBoxFontSizeTextEdit,  SIGNAL(valueChanged(int)),                this, SLOT(updateTextEditFont()));
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
}


void Prompter::updatePrompterImage()
{
    // get current widget size
    width  = ui->openGLWidget->width();
    height = ui->openGLWidget->height();

    imagePrompterText = QImage(QSize(width, height),QImage::Format_RGB32);

    QPainter painter(&imagePrompterText);

    // erase area inside the rectangle    
    painter.eraseRect(0,0, width, height);
    painter.fillRect(0,0,width, height, backgroundColor);

    // get font from combo box
    QString fontFamily = ui->fontComboBoxPrompter->currentFont().family();

    // Create a new font with the selected size and family
    QFont newFont(fontFamily, ui->spinBoxFontSizePrompter->value());

    // set local var
    prompterFont = newFont;

    // change painter font
    painter.setPen(textPen);
    painter.setFont(newFont);

    if (ui->checkBoxWordWrap->isChecked())
        painter.drawText(QRect(0, 0, width, height), textDirection|Qt::AlignVCenter|Qt::TextWordWrap, ui->textEdit->toPlainText());
    else
        painter.drawText(QRect(0, 0, width, height), textDirection|Qt::AlignVCenter,                  ui->textEdit->toPlainText());

    // mirror image horizontally
    if (ui->checkBoxMirror->isChecked())
        imagePrompterText.mirror(true, false);

    // update GUI
    update();
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

        timer->start(ui->dialScrollSpeed->value());
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


void Prompter::on_dialScrollSpeed_valueChanged(int value)
{
    ui->labelScrollSpeed->setNum(value);
    timer->setInterval(value);
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


void Prompter::updatePrompterFont()
{
    // Set the font
    updatePrompterImage();
}


void Prompter::on_spinBoxFontSizePrompter_valueChanged()
{
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
        // start scrolling
        on_pushButtonScroll_clicked();

        ui->textEdit->hide();
        ui->labelLink->hide();
        ui->groupBoxAlignment->hide();
        ui->groupBoxWarpMirror->hide();
        ui->groupBoxScrollSpeed->hide();
        ui->fontComboBoxTextEdit->hide();
        ui->fontComboBoxPrompter->hide();
        ui->spinBoxFontSizeTextEdit->hide();
        ui->spinBoxFontSizePrompter->hide();
        ui->pushButtonScroll->hide();
        ui->pushButtonReset->hide();
        ui->pushButtonFullScreen->hide();
        ui->pushButtonExit->hide();
        ui->labelText->hide();
        ui->labelPreview->hide();

        // Set the cursor to a blank cursor (hide it)
        setCursor(Qt::BlankCursor);

        // enter fullscreen for main window
        QMainWindow::showFullScreen();

        // set openGLWidget to full main window size (not full-Screen)
        ui->openGLWidget->setGeometry(0, 0, ui->centralwidget->width(), ui->centralwidget->height());
    }
    else
    {
        // return from full screen
        QMainWindow::showNormal();

        // Set the cursor to a normal cursor (show it)
        unsetCursor();

        // stop scrolling
        on_pushButtonScroll_clicked();

        // restore openGLWidget to size of pgm start
        ui->openGLWidget->setGeometry(prompterWidgetGeometryX, prompterWidgetGeometryY, prompterWidgetGeometryWidth , prompterWidgetGeometryHeight);

        ui->labelPreview->show();
        ui->labelText->show();
        ui->pushButtonExit->show();
        ui->pushButtonFullScreen->show();
        ui->pushButtonReset->show();
        ui->pushButtonScroll->show();
        ui->spinBoxFontSizePrompter->show();
        ui->spinBoxFontSizeTextEdit->show();
        ui->fontComboBoxPrompter->show();
        ui->fontComboBoxTextEdit->show();
        ui->groupBoxScrollSpeed->show();
        ui->groupBoxWarpMirror->show();
        ui->groupBoxAlignment->show();
        ui->labelLink->show();
        ui->textEdit->show();
    }

    updatePrompterImage();
}


void Prompter::onFontComboBoxTextEditChanged(const QFont& font)
{
    // Do something with the selected font, for example, update a label
    ui->textEdit->setFont(font);
}


void Prompter::keyPressEvent(QKeyEvent *event)
{
    // CMD+X (Win: ctrl+x)
//    if (event->key() == Qt::Key_X && event->modifiers() == Qt::ControlModifier)
//    {
//        QApplication::exit(0);
//    }
//    else
//    {
        if (event->key() == Qt::Key_Escape)
        {
            // do the same as if the "full screen" button would be pushed again
            on_pushButtonFullScreen_clicked();
        }
        else
        {
            // Call the base class implementation for other key events
            QMainWindow::keyPressEvent(event);
        }
//    }
}
