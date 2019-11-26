#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QObject>

#include "mypicture.h"
#include <string>
#include <cmath>
#include <string>

class MyButton : public QObject
{
    Q_OBJECT
public:
    explicit MyButton(QObject *parent = nullptr);

    int x, y, w, h;
    int valible;
    int active;
    int isPush;
    std::string name;

    MyPicture pictureNormal;
    MyPicture picturePush;

    void setName(const std::string name);
    void setPostion(int x, int y);
    void loadPicture(const std::string pathNormal, const std::string pathPush);
    void update();
    void draw(cv::Mat &image);
    void push();
    void release();
    bool isIn(int x, int y);
    void setActive();
    void setDeactive();

signals:
    void pushed();
    void firstPush();
    void myRelease();
public slots:
};

class CooldownButton: public MyButton{
public:
    int coolDownTime;
    int coolDonwCount;
    std::vector<std::string> infos;

    CooldownButton();
    void addInfo(std::string info);
    void setCooldownTime(int t);
    void update();
    void draw(cv::Mat &image);
    void release();
};

#endif // MYBUTTON_H
