#include "mybutton2.h"

MyButton2::MyButton2(QObject *parent) : QObject(parent)
{
    valible = 1;
    isPush = 0;
    name = "";
}

void MyButton2::setName(const std::string name){
    this->name = name;
}

void MyButton2::setPostion(int x, int y){
    this->x = x;
    this->y = y;
}

void MyButton2::loadPicture(const std::string pathNormal, const std::string pathPush){
    pictureNormal.load(pathNormal, 1.0);
    picturePush.load(pathPush, 1.0);
    w = pictureNormal.width();
    h = pictureNormal.height();
}

void MyButton2::update(){
    if(isPush){
        pushed(name);
    }
}

void MyButton2::draw(cv::Mat &image){
    if(isPush){
        picturePush.getPicture().copyTo(image(cv::Rect(std::min(x, image.cols-w), std::min(y, image.rows-h), w, h)), picturePush.getMask());
    }
    else{
        pictureNormal.getPicture().copyTo(image(cv::Rect(std::min(x, image.cols-w), std::min(y, image.rows-h), w, h)), pictureNormal.getMask());
    }
}

void MyButton2::push(){
    isPush = 1;
}

void MyButton2::release(){
    isPush = 0;
}

bool MyButton2::isIn(int x, int y){
    if(this->x < x && this->x + this->w > x && this->y < y && this->y + this->w > y){
        return 1;
    }
    else{
        return 0;
    }
}
