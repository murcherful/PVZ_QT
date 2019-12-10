#include "mainloopthread.h"

int gridHeight = -1;
int gridWidth = -1;


MainLoopThread::MainLoopThread(QLabel* label)
{
    this->label = label;
    this->image.create(MW_H, MW_W, CV_8UC3);
    this->isStop = 0;
    this->mouseX = MW_W/2;
    this->mouseY = MW_H/2;
    mouseImageDefault.load(SOURCE_PATH+"mouseDefault.png", 1.0);
    isMouseCenter = 0;
    mouseImage = &mouseImageDefault;
    // set scene
    currentScene = &scene1;
    // load scene1 (welcome scene)
    // and add buttons
    scene1.load(SOURCE_PATH+"welcome.png");

    playButton = new MyButton();

    playButton->setName("playButton");
    playButton->setPosition(355, 510);
    playButton->loadPicture(SOURCE_PATH+"playButton.png", SOURCE_PATH+"playButton_push.png");
    connect(playButton, &MyButton::myRelease, this, &MainLoopThread::playButtonRelease);
    //std::cout << "[Debug]: add button: " << playButton->name << std::endl;
    scene1.addButton(playButton);

    exitButton = new MyButton();
    exitButton->setName("exitButton");
    exitButton->setPosition(695, 510);
    exitButton->loadPicture(SOURCE_PATH+"exitButton.png", SOURCE_PATH+"exitButton_push.png");
    connect(exitButton, &MyButton::myRelease, this, &MainLoopThread::exitButtonRelease);
    scene1.addButton(exitButton);

    // load scene2 (play scene)
    // add buttons
    // and add objects    
    scene2.load(SOURCE_PATH+"playScene.png");
    exitButtonSmall = new MyButton();
    exitButtonSmall->setName("exitButtonSmall");
    exitButtonSmall->setPosition(1150, 10);
    exitButtonSmall->loadPicture(SOURCE_PATH+"exitButtonSmall.png", SOURCE_PATH+"exitButtonSmall_push.png");
    connect(exitButtonSmall, &MyButton::myRelease, this, &MainLoopThread::exitButtonRelease);
    scene2.addButton(exitButtonSmall);

    // add buttons of shop
    shopPlants.push_back(new SunFlower());
    shopPlants.push_back(new PeaShooter());

    for(int i = 0; i < shopPlants.size(); ++i){
        CooldownButton* cooldownButton = new CooldownButton();
        cooldownButton->setName(shopPlants[i]->getName()+"Button");
        cooldownButton->loadPicture(shopPlants[i]->getpicture()->getPath(), shopPlants[i]->getpicture()->getPath());
        cooldownButton->setCooldownTime(shopPlants[i]->getCooldownTime());
        cooldownButton->addInfo(shopPlants[i]->getName());
        connect(cooldownButton, &CooldownButton::cooldownButtonFirstPush, this, &MainLoopThread::cooldownButtonPush);
        connect(cooldownButton, &CooldownButton::cooldownButtonMyRelease, this, &MainLoopThread::cooldownButtonRelease);
        scene2.addCooldownButton(cooldownButton);
    }
    /*
    peaShooterButton = new CooldownButton();
    peaShooterButton->setName("PeaShooterButton");
    peaShooterButton->loadPicture(SOURCE_PATH+"PeaShooter.png", SOURCE_PATH+"PeaShooter.png");
    peaShooterButton->setCooldownTime(PFS*10);
    peaShooterButton->addInfo("PeaShooter");
    connect(peaShooterButton, &CooldownButton::firstPush, this, &MainLoopThread::peaShooterButtonPush);
    connect(peaShooterButton, &CooldownButton::myRelease, this, &MainLoopThread::peaShooterButtonRelease);
    scene2.addCooldownButton(peaShooterButton);
    */
    // add grids
    for(int i = 0 ;i < GRID_Y_N; ++i){
        for(int j = 0; j < GRID_X_N; ++j){
            Grid* grid = new Grid();
            grid->loadPicture(SOURCE_PATH+"grid.png");
            grid->loadBlackPicture(SOURCE_PATH+"grid_black.png");
            grid->setPosition(GRID_X+j*grid->getW(), GRID_Y+i*grid->getH());
            if(gridHeight == -1){
                gridHeight = grid->getH();
                gridWidth = grid->getW();
            }
            scene2.addGrids(grid);
        }
    }

    // add suns

    for(int i = 0 ;i < GRID_Y_N; ++i){
        for(int j = 0; j < GRID_X_N; ++j){
            Sun* sun = new Sun();
            sun->loadPicture(SOURCE_PATH+"sun.png", SOURCE_PATH+"sun.png");
            sun->setPosition(GRID_X+j*gridWidth, GRID_Y+i*gridHeight);
            //std::cout << "[Debug] " << sun->x << " " << sun->y << std::endl;
            scene2.addSun(sun);
        }
    }

    // test add charactor
    NormalZombie* nz0 = new NormalZombie();
    nz0->setPosition(GRID_X+gridWidth*10, GRID_Y+gridHeight*1);
    scene2.addZombie(nz0);

    SunFlower* sf0 = new SunFlower();
    //int sfX = GRID_X+gridWidth*0+gridWidth/2-sf0->getW()/2;
    //int sfY = GRID_Y+gridHeight*0+gridHeight/2-sf0->getH()/2;
    //std::cout << "[Debug]: " << sfX << " " << sfY << std::endl;
    //sf0->setPosition(sfX, sfY);
    //int sX = (sf0->getX()-GRID_X)/gridWidth;
    //int sY = (sf0->getY()-GRID_Y)/gridHeight;
    connect(sf0, &SunFlower::genSun, scene2.getSun(0, 0), &Sun::addSun);
    scene2.addPlant(sf0, 0, 0);

    PeaShooter* ps0 = new PeaShooter();
    //int psX = GRID_X+gridWidth*0+gridWidth/2-ps0->getW()/2;
    //int psY = GRID_Y+gridHeight*1+gridHeight/2-ps0->getH()/2;
    //ps0->setPosition(psX, psY);
    connect(ps0, &PeaShooter::genBullet, this, &MainLoopThread::AddPeaBullet);
    scene2.addPlant(ps0, 0, 1);

}

void MainLoopThread::run(){
    while(1){
        mainLoop();
        // std::cout <<"GG" << std::endl;
        if(isStop){
            break;
        }
        MYTOOL::waitMSec(1000/PFS);
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
    int tMouseX;
    int tMouseY;
    if(isMouseCenter){
        tMouseX = mouseX-mouseImage->width()/2;
        tMouseY = mouseY-mouseImage->height()/2;
        mouseImage->getPicture().copyTo(image(cv::Rect(std::min(tMouseX, image.cols-mouseImage->width()), std::min(tMouseY, image.rows-mouseImage->height()), mouseImage->width(), mouseImage->height())), mouseImage->getMask());
    }
    else{
        tMouseX = mouseX;
        tMouseY = mouseY;
        mouseImage->getPicture().copyTo(image(cv::Rect(std::min(tMouseX, image.cols-mouseImage->width()), std::min(tMouseY, image.rows-mouseImage->height()), mouseImage->width(), mouseImage->height())), mouseImage->getMask());
    }

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
/*
void MainLoopThread::peaShooterButtonPush(){
    mouseImage = peaShooterButton->getPicture();
    isMouseCenter = 1;
}

void MainLoopThread::peaShooterButtonRelease(){
    mouseImage = &mouseImageDefault;
    isMouseCenter = 0;
}
*/
void MainLoopThread::cooldownButtonPush(MyPicture *p){
    mouseImage = p;
    isMouseCenter = 0;
}

void MainLoopThread::cooldownButtonRelease(int cost){
    mouseImage = &mouseImageDefault;
    isMouseCenter = 0;
}

void MainLoopThread::AddPeaBullet(int x, int y){
    PeaBullet* pb = new PeaBullet();
    pb->setPosition(x, y);
    scene2.addBullet(pb);
}
