#ifndef MYPICTURE_H
#define MYPICTURE_H

#include <opencv2/opencv.hpp>
#include <string>

class MyPicture
{
public:
    MyPicture();

    void load(const std::string path, double scale);
    cv::Mat getPicture();
    cv::Mat getMask();
    int width();
    int height();

private:
    double scale;
    cv::Mat p;
    cv::Mat m;
};

#endif // MYPICTURE_H
