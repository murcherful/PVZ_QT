#ifndef MYTOOL_H
#define MYTOOL_H

#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <QImage>
#include <QSize>
#include <QCoreApplication>
#include <QTime>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>


#define MW_H        800
#define MW_W        1200

#define PFS         20

#define GRID_X_N    11
#define GRID_Y_N    5
#define GRID_X      50
#define GRID_Y      150
#define GRID_X_S    0       // the space between two grid in x direction
#define GRID_Y_S    0       // the space between two grid in y direction
#define SOURCE_PATH std::string("/home/murcherful/Qt/Project/PVZ/picture/")
namespace MYTOOL {
    QImage mat2QImage(cv::Mat m);
    QImage myResiezQImage(QImage img, QSize size);
    void waitSec(int sec);
    void waitMSec(int ms);
}

#endif // MYTOOL_H
