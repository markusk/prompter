#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButtonTest_clicked();
    void on_pushButtonReset_clicked();
    void on_verticalSliderScrollSpeed_valueChanged();
    void on_verticalSliderFontSize_valueChanged();

    void scrollPrompter();


private:
    Ui::MainWindow *ui;

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

    // timer for scroll animation
    QTimer *timer;

    //
    bool updatePrompterImage();
};
#endif // MAINWINDOW_H
