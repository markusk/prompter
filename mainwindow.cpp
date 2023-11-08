#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    createImage("Ein einfacher Text\nDer auch mal länger sein kann.", "", "TestBild.png", Qt::green);

    ui->setupUi(this);

    // display image/pixmap in GUI
    ui->labelImage->setStyleSheet("border-image:url(:/2.png);");
    ui->labelImage->setPixmap(pix);
}

MainWindow::~MainWindow()
{
    delete ui;
}

