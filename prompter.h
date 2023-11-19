#ifndef PROMPTER_H
#define PROMPTER_H

#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QDir>
#include <QObject> // for connecting signals and slots
#include <QTimer>  // for scroll animation

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

#include <QScreen> // for getting resolution
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class Prompter; }
QT_END_NAMESPACE

class Prompter : public QMainWindow
{
    Q_OBJECT

public:
    Prompter(QWidget *parent = nullptr);
    ~Prompter();
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButtonScroll_clicked();
    void on_pushButtonReset_clicked();
    void on_pushButtonExit_clicked();
    void on_verticalSliderScrollSpeed_valueChanged();
    void on_verticalSliderFontSize_valueChanged();
    void on_radioButtonLeft_clicked();
    void on_radioButtonCentered_clicked();
    void on_radioButtonRight_clicked();
    void on_checkBoxWordWrap_stateChanged();
    void on_checkBoxMirror_stateChanged();
    void on_pushButtonTest_clicked();

    // let the prompter text scroll
    void scrollPrompter();

    // update openGL widget with mirrored prompter text
    void updatePrompterImage();


private:
    Ui::Prompter *ui;

    int width;
    int height;

    // new for OpenGL drawing
    QFont textFont;
    QPen textPen;


    QImage imagePrompterText;
    int scrollValueY;
    int scrollSpeed;
    int textDirection;
    int fontSize;
    QColor textColor;
    QColor backgroundColor;

    // timer for scroll animation
    QTimer *timer;
};
#endif // PROMPTER_H
