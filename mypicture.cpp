#include "mypicture.h"

MyPicture::MyPicture()
{

}

void MyPicture::load(const std::string path, double scale){
    this->scale = scale;
    cv::Mat pTemp = cv::imread(path);
    cv::resize(pTemp, p, cv::Size((double)pTemp.cols*scale, (double)pTemp.rows*scale), cv::INTER_NEAREST);
    //cv::Mat mTemp = cv::imread(path, cv::IMREAD_GRAYSCALE);
    //cv::resize(mTemp, m, cv::Size((double)mTemp.cols*scale, (double)mTemp.rows*scale), cv::INTER_NEAREST);
    cv::cvtColor(p, m, cv::COLOR_BGR2GRAY);
    this->path = path;
}

cv::Mat MyPicture::getPicture(){
    return p;
}

cv::Mat MyPicture::getMask(){
    return m;
}

int MyPicture::width(){
    return p.cols;
}

int MyPicture::height(){
    return p.rows;
}

void MyPicture::draw(int x, int y, cv::Mat &image){
    int w = width();
    int h = height();
    p.copyTo(image(cv::Rect(std::min(x, image.cols-w), std::min(y, image.rows-h), w, h)), m);
}

std::string  MyPicture::getPath(){
    return path;
}
