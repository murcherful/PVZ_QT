#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainLoopThread = new MainLoopThread(ui->label);
    mainLoopThread->start();
    connect(this, &MainWindow::myMouseMoveSignal, mainLoopThread, &MainLoopThread::myMouseMoveSlot);
    connect(this, &MainWindow::myMousePushSignal, mainLoopThread, &MainLoopThread::myMousePushSlot);
    connect(this, &MainWindow::myMouseReleaseSignal, mainLoopThread, &MainLoopThread::myMouseReleaseSlot);
    connect(mainLoopThread, &MainLoopThread::myExit, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    //std::cout <<"quit" << std::endl;
    mainLoopThread->stopLoop();
    mainLoopThread->quit();
    mainLoopThread->wait();
    delete ui;
    delete mainLoopThread;
}

void MainWindow::mouseMoveEvent(QMouseEvent *e){
    myMouseMoveSignal(e);
}

void MainWindow::mousePressEvent(QMouseEvent *e){
    myMousePushSignal(e);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e){
    myMouseReleaseSignal(e);
}

