#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "mytool.h"
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <sstream>
#include "mainloopthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    MainLoopThread* mainLoopThread;

    void stopThread();

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

signals:
    void myMouseMoveSignal(QMouseEvent *e);
    void myMousePushSignal(QMouseEvent *e);
    void myMouseReleaseSignal(QMouseEvent *e);

public slots:

};

#endif // MAINWINDOW_H
