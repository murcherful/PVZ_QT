#include "myscene.h"

MyScene::MyScene()
{
    isPush = 0;
    currentButton = NULL;
}

MyScene::~MyScene(){
/*
    for(int i = 0; i < buttons.size(); ++i){
        delete buttons[i];
    }
*/
}

void MyScene::load(const std::string path){
    background.load(path, 1.0);
}

void MyScene::update(){
    for(int i = 0; i < buttons.size(); ++i){
        buttons[i]->update();
    }
}

void MyScene::draw(cv::Mat &image){
    background.getPicture().copyTo(image(cv::Rect(0, 0, background.width(), background.height())), background.getMask());
    for(int i = 0; i < buttons.size(); ++i){
        buttons[i]->draw(image);
    }
}

void MyScene::push(int x, int y){
    for(int i = 0; i < buttons.size(); ++i){
        if(buttons[i]->isIn(x, y)){
            currentButton = buttons[i];
            buttons[i]->push();
            isPush = 1;
            break;
        }
    }
}

void MyScene::release(){
    if(isPush){
        currentButton->release();
        isPush = 0;
        currentButton = NULL;
    }
}

void MyScene::addButton(MyButton* button){
    buttons.push_back(button);
}

PlayScene::PlayScene(){
    shopX = 100;
    shopY = 20;
    shopN = 8;
}

void PlayScene::addCooldownButton(CooldownButton *cooldownButton){
    int n = cooldownButtons.size();
    cooldownButton->setPostion(shopX+n*100, shopY);
    if(n >= shopN){
        cooldownButton->setDeactive();
    }
    buttons.push_back(cooldownButton);
    cooldownButtons.push_back(cooldownButton);
}
