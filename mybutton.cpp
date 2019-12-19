#include "mybutton.h"

MyButton::MyButton()
{
    //std::cout << "[Debug]: GG" << std::endl;
    valible = 1;
    isPush = 0;
    name = "";
    active = 1;
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
        picturePush.draw(x, y, image);
        //picturePush.getPicture().copyTo(image(cv::Rect(std::min(x, image.cols-w), std::min(y, image.rows-h), w, h)), picturePush.getMask());
    }
    else{
        pictureNormal.draw(x, y, image);
        //pictureNormal.getPicture().copyTo(image(cv::Rect(std::min(x, image.cols-w), std::min(y, image.rows-h), w, h)), pictureNormal.getMask());
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

MyPicture* MyButton::getPicture(){
    return &pictureNormal;
}

void MyButton::setDeactive(){
    active = 0;
}

CooldownButton::CooldownButton(){
    coolDownTime = 0;
    coolDownCount = 0;
    cost = 0;
    hasEnoughSun = 0;
}

void CooldownButton::addInfo(std::string info){
    infos.push_back(info);
}

void CooldownButton::setCooldownTime(int t){
    coolDownTime = t;
}

void CooldownButton::setCost(int cost){
    this->cost = cost;
}

void CooldownButton::setPlantName(std::string plantName){
    this->plantName = plantName;
}

void CooldownButton::update(){
    if(coolDownCount>0){
        coolDownCount--;
        valible = 0;
    }
    else if(!valible && coolDownCount == 0 && hasEnoughSun){
        valible = 1;
    }
    else{
        valible = 0;
    }
    MyButton::update();
}

void CooldownButton::draw(cv::Mat &image){
    if(!active){
        return;
    }
    if(!hasEnoughSun){
        cv::rectangle(image, cv::Rect(x, y, w, h), cv::Scalar(150, 150, 150), -1);
    }
    else{
        cv::rectangle(image, cv::Rect(x, y, w, h), cv::Scalar(255, 255, 255), -1);
        cv::rectangle(image, cv::Rect(x, y, w, h*((double(coolDownCount)/coolDownTime))), cv::Scalar(255, 191, 0), -1);
    }
    //cv::rectangle(image, cv::Rect(x, y, w, h*((double(coolDownCount)/coolDownTime))), cv::Scalar(0, 191, 255), -1);
    //cv::rectangle(image, cv::Rect(x, y, w, h*((double(coolDownTime-coolDownCount)/coolDownTime))), cv::Scalar(255, 255, 255), -1);
    cv::rectangle(image, cv::Rect(x, y, w, h), cv::Scalar(30, 105, 139), 3);
    MyButton::draw(image);
    //std::cout << "dd" << std::endl;
    for(int i = 0; i < infos.size(); ++i){
        cv::putText(image, infos[i], cv::Point(x, y+h+(i+1)*20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 2);
    }
}

void CooldownButton::push(){
    MyButton::push();
    cooldownButtonFirstPush(&pictureNormal);
}

void CooldownButton::cooldown(){
    coolDownCount = coolDownTime;
    valible = 0;
}

void CooldownButton::release(){
    MyButton::release();
    cooldownButtonMyRelease(plantName, this);
}

void CooldownButton::checkSun(int sun){
    if(sun >= cost){
        hasEnoughSun = 1;
        valible = 1;
    }
    else{
        hasEnoughSun = 0;
        valible = 0;
    }
}

void CooldownButton::reSet(){
    coolDownCount = 0;
}

int CooldownButton::getCost(){
    return cost;
}

Sun::Sun(){
    sunNumber = 0;
}

void Sun::update(){
    int randNumber = rand()%GEN_SUN_PRE;
    if(randNumber == 0){
        sunNumber ++;
    }
    MyButton::update();
}

void Sun::draw(cv::Mat &image){
    if(sunNumber > 0){
        pictureNormal.draw(x, y, image);
        std::stringstream ss;
        ss << sunNumber;
        cv::putText(image, ss.str(), cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 2);
    }
}

void Sun::addSun(int n){
    sunNumber += n;
}

void Sun::removeSun(){
    int temp = sunNumber;
    sunNumber = 0;
    getSun(temp);
}

void Sun::release(){
    removeSun();
    MyButton::release();
}

void Sun::clearSun(){
    sunNumber = 0;
}

