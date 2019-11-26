#include "mybutton.h"

MyButton::MyButton(QObject *parent) : QObject(parent)
{
    valible = 1;
    isPush = 0;
    name = "";
    active = 1;
}

void MyButton::setName(const std::string name){
    this->name = name;
}

void MyButton::setPostion(int x, int y){
    this->x = x;
    this->y = y;
}

void MyButton::loadPicture(const std::string pathNormal, const std::string pathPush){
    pictureNormal.load(pathNormal, 1.0);
    picturePush.load(pathPush, 1.0);
    w = pictureNormal.width();
    h = pictureNormal.height();
}

void MyButton::update(){
    if(isPush){
        pushed();
    }
}

void MyButton::draw(cv::Mat &image){
    if(!active){
        return;
    }
    if(isPush){
        picturePush.getPicture().copyTo(image(cv::Rect(std::min(x, image.cols-w), std::min(y, image.rows-h), w, h)), picturePush.getMask());
    }
    else{
        pictureNormal.getPicture().copyTo(image(cv::Rect(std::min(x, image.cols-w), std::min(y, image.rows-h), w, h)), pictureNormal.getMask());
    }
}

void MyButton::push(){
    isPush = 1;
    firstPush();
}

void MyButton::release(){
    isPush = 0;
    myRelease();
}

bool MyButton::isIn(int x, int y){
    if(active && valible && this->x < x && this->x + this->w > x && this->y < y && this->y + this->h > y){
        return 1;
    }
    else{
        return 0;
    }
}

void MyButton::setActive(){
    active = 1;
}

void MyButton::setDeactive(){
    active = 0;
}

CooldownButton::CooldownButton(){
    coolDownTime = 0;
    coolDonwCount = 0;
}

void CooldownButton::addInfo(std::string info){
    infos.push_back(info);
}

void CooldownButton::setCooldownTime(int t){
    coolDownTime = t;
}

void CooldownButton::update(){
    if(coolDonwCount>0){
        coolDonwCount--;
    }
    else if(!valible && coolDonwCount == 0){
        valible = 1;
    }
    MyButton::update();
}

void CooldownButton::draw(cv::Mat &image){
    MyButton::draw(image);
    for(int i = 0; i < infos.size(); ++i){
        cv::putText(image, infos[i], cv::Point(x, y+h+(i+1)*20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
    }
}

void CooldownButton::release(){
    coolDonwCount = coolDownTime;
    valible = 0;
    MyButton::release();
}


