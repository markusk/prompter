#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QDir>
#include <QObject> // for connecting signals and slots


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    int width = 1024;
    int height = 768;
    int offset = 25;
    int w = 400;
    int h = 200;

    QPixmap pix;


    bool createImage(QString text="Leer", QString path="./", QString imageName="TextImage.png", QColor aColor=Qt::red)
    {
        QImage image(QSize(width,height),QImage::Format_RGB32);

        QPainter painter(&image);
        painter.setBrush(QBrush(aColor));
        painter.fillRect(QRectF(0,0,width,height),Qt::darkGreen);
        // qDebug() << (width-w-offset)/2 << "\t" << (height-h-offset)/2 << "\t" << w << "\t" << h;
        QRect aRect = QRect( (width-w)/2, (height-h)/2, w, h );
        QRect aRectOffset = QRect( (width-w+offset)/2, (height-h+offset)/2, w-(offset/2), h-(offset/2) );
        painter.fillRect(QRect(aRect),Qt::white);
        painter.setPen(QPen(Qt::black));
        painter.setFont(QFont( "Courier", 20) );

        // add text to image
        painter.drawText(QRect(aRectOffset),text);

        // mirror image horicontally
        image = image.mirrored(true, false);

        // assign image to Pixmap 'pix'
        pix = QPixmap(width, height);
        pix.fromImage(image);

        // save image to disk
        QDir aDir = QDir(path);
        if ( aDir.mkpath(path) )
            return image.save(path + "/" + imageName);
        else
            return image.save(imageName);
    }
};
#endif // MAINWINDOW_H
