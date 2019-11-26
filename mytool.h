#ifndef MYTOOL_H
#define MYTOOL_H

#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <QImage>
#include <QSize>
#include <QCoreApplication>
#include <QTime>

#define MW_H 800
#define MW_W 1200

namespace MYTOOL {
    QImage mat2QImage(cv::Mat m);
    QImage myResiezQImage(QImage img, QSize size);
    void waitSec(int sec);
    void waitMSec(int ms);
}

#endif // MYTOOL_H
