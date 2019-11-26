#ifndef MYBUTTON2_H
#define MYBUTTON2_H

#include <QObject>

#include "mypicture.h"
#include <string>
#include <cmath>
#include <string>

class MyButton2 : public QObject
{
    Q_OBJECT
public:
    explicit MyButton2(QObject *parent = nullptr);

    int x, y, w, h;
    int valible;
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

signals:
    void pushed(std::string name);
public slots:
};

#endif // MYBUTTON2_H
