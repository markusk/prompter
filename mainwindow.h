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
    void on_pushButton_clicked();
    void animate();


private:
    Ui::MainWindow *ui;

    int width = 300;
    int height = 200;
    int offset = 25;

    //QImage *image;
    //QPainter *painter;
    QPixmap pix;
    // new for OpenGL drawing
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;

    int elapsed;


    QImage imagePrompterText;

    // timer for scroll animation
    QTimer *timer;

    //
    bool createImage(QString text, QColor aColor);
};
#endif // MAINWINDOW_H
