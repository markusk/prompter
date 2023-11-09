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

    //
    bool createImage(QString text, QString path, QString imageName, QColor aColor);
};
#endif // MAINWINDOW_H
