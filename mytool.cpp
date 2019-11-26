#include "mytool.h"

QImage MYTOOL::mat2QImage(cv::Mat m){
    cv::Mat tempImage;
    cv::cvtColor(m, tempImage, cv::COLOR_BGR2RGB);
    return QImage((const unsigned char *)(tempImage.data), tempImage.cols, tempImage.rows, tempImage.cols*tempImage.channels(), QImage::Format_RGB888).copy();
}

QImage MYTOOL::myResiezQImage(QImage img, QSize size){


    double imgWHRatio = (double)img.size().width()/(double)img.size().height();
    double sizeWHRatio = (double)size.width()/(double)size.height();

    QSize newSize;
    if(imgWHRatio > sizeWHRatio){
        newSize = QSize(size.width(), (int)(size.width()/imgWHRatio));
    }
    else{
        newSize = QSize((int)(size.height()*imgWHRatio), size.height());
    }
    return img.scaled(newSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void MYTOOL::waitSec(int s)
{
    QTime t;
    t.start();
    while(t.elapsed()<1000*s)
        QCoreApplication::processEvents();
}

void MYTOOL::waitMSec(int ms)
{
    QTime t;
    t.start();
    while(t.elapsed()< ms)
        QCoreApplication::processEvents();
}
