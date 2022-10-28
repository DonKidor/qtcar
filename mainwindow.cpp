#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <MyScene.h>
#include <mycar.h>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(size());
    scene = new MyScene(QRectF(0,0,800,800));
    for(int i=0; i<scene->carCount ; ++i) {
        MyCar *car = new MyCar();
        scene->addCar(car);
    }


    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(QRect(-400,-400,800,800));

    /*
    QObject::connect(scene, SIGNAL(pressW(bool)), car, SLOT(forward(bool)));
    QObject::connect(scene, SIGNAL(pressA(bool)),
                     car, SLOT(turnLeft(bool)));
    QObject::connect(scene, SIGNAL(pressD(bool)),
                     car, SLOT(turnRight(bool)));
    QObject::connect(scene, SIGNAL(pressS(bool)),
                     car, SLOT(brake(bool)));
                     */
    QObject::connect(ui->wallButton, SIGNAL(clicked()),
                     scene, SLOT(wallSelected()));
    QObject::connect(ui->cancelButton , SIGNAL(clicked()),
                     scene, SLOT(cancelSelected()));
    QObject::connect(ui->flagButton , SIGNAL(clicked()),
                     scene, SLOT(fitnessSelected()));
    QObject::connect(ui->calibrButton, SIGNAL(clicked()),
                     scene, SLOT(calibrSelected()));
    QObject::connect(ui->startButton, SIGNAL(clicked()),
                     scene, SLOT(genStart()));

    QObject::connect(scene, SIGNAL(genChanged(int)), ui->label, SLOT(setNum(int)));
}


MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

