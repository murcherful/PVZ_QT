#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QObject>

#include "mypicture.h"
#include "myobject.h"
#include "mytool.h"
#include <string>
#include <cmath>
#include <string>

class MyButton : public MyObject
{
    Q_OBJECT
public:
    MyButton();

    //int x, y, w, h;
    int valible;
    int active;
    int isPush;
    //std::string name;

    MyPicture pictureNormal;
    MyPicture picturePush;

    //virtual void setPostion(int x, int y);
    virtual void loadPicture(const std::string pathNormal, const std::string pathPush);
    void update();
    void draw(cv::Mat &image);
    virtual void push();
    virtual void release();
    virtual bool isIn(int x, int y);
    virtual void setActive();
    virtual void setDeactive();
    virtual MyPicture* getPicture();

signals:
    void pushed();
    void firstPush();
    void myRelease();
public slots:
};

class CooldownButton: public MyButton{
    Q_OBJECT
public:
    int coolDownTime;
    int coolDownCount;
    int cost;
    int hasEnoughSun;
    std::vector<std::string> infos;
    std::string plantName;

    CooldownButton();
    void addInfo(std::string info);
    void setCooldownTime(int t);
    void setCost(int cost);
    void setPlantName(std::string plantName);
    void update();
    void draw(cv::Mat &image);
    void push();
    void release();
    void cooldown();
signals:
    void cooldownButtonFirstPush(MyPicture* p);
    void cooldownButtonMyRelease(std::string name, CooldownButton* b);

public slots:
    void checkSun(int sun);
};

#define GEN_SUN_PRE (PFS*500)

class Sun: public MyButton{
    Q_OBJECT
public:

    int sunNumber;

    Sun();
    void update();
    void draw(cv::Mat &image);
    void release();

signals:
    void getSun(int n);

public slots:
    void addSun(int n);
    void removeSun();
};

#endif // MYBUTTON_H
