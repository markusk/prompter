#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createImage("Ein einfacher Text\nDer auch mal länger sein kann.", "", "TestBild.png", Qt::green);
}

MainWindow::~MainWindow()
{
    delete ui;
}

