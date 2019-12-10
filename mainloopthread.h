#ifndef MAINLOOPTHREAD_H
#define MAINLOOPTHREAD_H

#include <QThread>
#include <QLabel>
#include <opencv2/opencv.hpp>
#include "mytool.h"
#include <QMouseEvent>
#include "mypicture.h"
#include <cmath>
#include "myscene.h"
#include <string>
#include "myobject.h"


class MainLoopThread : public QThread
{
    Q_OBJECT
public:
    MainLoopThread(QLabel* label);

    void stopLoop();

protected:
    void run();

signals:
    void isDone();

private:
    QLabel* label;
    cv::Mat image;
    bool isStop;

    int mouseX;
    int mouseY;
    MyPicture *mouseImage;
    MyPicture mouseImageDefault;
    bool isMouseCenter;

    MyScene *currentScene;
    MyScene scene1;
    PlayScene scene2;

    std::vector<Plant*> shopPlants;

    MyButton* playButton;
    MyButton* exitButton;
    MyButton* exitButtonSmall;
    //CooldownButton* peaShooterButton;
    //CooldownButton* sunFloewerButton;

    void mainLoop();

signals:
    void myExit();

public slots:
    void myMouseMoveSlot(QMouseEvent *e);
    void myMousePushSlot(QMouseEvent *e);
    void myMouseReleaseSlot(QMouseEvent *e);

    void playButtonRelease();

    void exitButtonRelease();

    //void peaShooterButtonPush();

    //void peaShooterButtonRelease();

    void cooldownButtonPush(MyPicture* p);

    void cooldownButtonRelease(int cost);

    void AddPeaBullet(int x, int y);

};

#endif // MAINLOOPTHREAD_H
