#include "mainloopthread.h"

MainLoopThread::MainLoopThread(QLabel* label)
{
    this->label = label;
    this->image.create(MW_H, MW_W, CV_8UC3);
    this->isStop = 0;
    this->mouseX = MW_W/2;
    this->mouseY = MW_H/2;
    mouseImageDefault.load(sourcePath+"mouseDefault.png", 1.0);
    mouseImage = &mouseImageDefault;
    currentScene = &scene1;
    scene1.load(sourcePath+"welcome.png");
    MyButton* playButton = new MyButton();
    playButton->setName("playButton");
    playButton->setPostion(400, 510);
    playButton->loadPicture(sourcePath+"playButton.png", sourcePath+"playButton_push.png");
    connect(playButton, &MyButton::myRelease, this, &MainLoopThread::playButtonRelease);
    scene1.addButton(playButton);
    MyButton* exitButton = new MyButton();
    exitButton->setName("exitButton");
    exitButton->setPostion(400, 650);
    exitButton->loadPicture(sourcePath+"exitButton.png", sourcePath+"exitButton_push.png");
    connect(exitButton, &MyButton::myRelease, this, &MainLoopThread::exitButtonRelease);
    scene1.addButton(exitButton);
    scene2.load(sourcePath+"playScene.png");
    scene2.addButton(exitButton);
    CooldownButton* peashooterButton = new CooldownButton();
    peashooterButton->setName("PeaShooterButton");
    peashooterButton->loadPicture(sourcePath+"PeaShooter.png", sourcePath+"PeaShooter.png");

}

void MainLoopThread::run(){
    while(1){
        mainLoop();
        // std::cout <<"GG" << std::endl;
        if(isStop){
            break;
        }
        MYTOOL::waitMSec(20);
    }
    //isDone();
}

void MainLoopThread::mainLoop(){
    // clear frame
    cv::rectangle(image, cv::Rect(0, 0, image.cols, image.rows), cv::Scalar(255, 255, 255), -1);


    // update
    currentScene->update();

    // draw
    currentScene->draw(image);

    // debug
    cv::putText(image, "HELLO WORLD!", cv::Point(0, image.rows-1), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
    // draw mouse info
    std::stringstream ss;
    ss <<"x: " << mouseX <<" y: " << mouseY;
    //std::cout << ss.str() << std::endl;
    cv::putText(image, ss.str(), cv::Point(0, image.rows-1-25), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));

    // draw mouse
    //cv::circle(image, cv::Point(mouseX, mouseY), 10, cv::Scalar(255, 0, 0), -1);
    mouseImage->getPicture().copyTo(image(cv::Rect(std::min(mouseX, image.cols-mouseImage->width()), std::min(mouseY, image.rows-mouseImage->height()), mouseImage->width(), mouseImage->height())), mouseImage->getMask());

    QImage qimage = MYTOOL::mat2QImage(image);
    label->setPixmap(QPixmap::fromImage(qimage));
}

void MainLoopThread::stopLoop(){
    //std::cout << "stop loop" << std::endl;
    isStop = 1;
}

void MainLoopThread::myMouseMoveSlot(QMouseEvent *e){
    mouseX = e->pos().x();
    mouseY = e->pos().y();
}

void MainLoopThread::myMousePushSlot(QMouseEvent *e){
    currentScene->push(e->pos().x(), e->pos().y());
}

void MainLoopThread::myMouseReleaseSlot(QMouseEvent *e){
    currentScene->release();
}

void MainLoopThread::playButtonRelease(){
    std::cout << "change scene" << std::endl;
    currentScene = &scene2;
}

void MainLoopThread::exitButtonRelease(){
    std::cout << "exit" << std::endl;
    myExit();
}
