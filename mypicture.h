#ifndef MYPICTURE_H
#define MYPICTURE_H

#include <opencv2/opencv.hpp>
#include <string>

class MyPicture
{
public:
    double scale;
    cv::Mat p;
    cv::Mat m;
    std::string path;

    MyPicture();

    void load(const std::string path, double scale);
    cv::Mat getPicture();
    cv::Mat getMask();
    int width();
    int height();
    void draw(int x, int y, cv::Mat &image);
    std::string getPath();

private:

};

#endif // MYPICTURE_H

