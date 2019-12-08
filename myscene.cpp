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
    background.draw(0, 0, image);
    //background.getPicture().copyTo(image(cv::Rect(0, 0, background.width(), background.height())), background.getMask());
    //std::cout << "[Debug]: here" << std::endl;
    for(int i = 0; i < buttons.size(); ++i){
        //std::cout << "[Debug]: button: " << buttons[i]->name << std::endl;
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
    cooldownButton->setPosition(shopX+n*100, shopY);
    if(n >= shopN){
        cooldownButton->setDeactive();
    }
    buttons.push_back(cooldownButton);
    cooldownButtons.push_back(cooldownButton);
}

void PlayScene::addSun(Sun *sun){
    buttons.push_back(sun);
    suns.push_back(sun);
}

void PlayScene::addGrids(Grid *grid){
    grids.push_back(grid);
}

void PlayScene::update(){
    MyScene::update();
    clear();
    for(int i = 0; i < plants.size(); ++i){
        for(int j = 0; j < zombies.size(); ++j){
            plants[i]->interactive(zombies[j]);
            zombies[j]->interactive(plants[i]);
        }
    }
    for(int i = 0; i < bullets.size(); ++i){
        for(int j = 0; j < zombies.size(); ++j){
            bullets[i]->interactive(zombies[j]);
        }
    }
    for(int i = 0; i < charactors.size(); ++i){
        charactors[i]->update();
    }
    for(int i = 0; i < grids.size(); ++i){
        grids[i]->update();
    }
}

void PlayScene::draw(cv::Mat &image){
    background.draw(0, 0, image);
    for(int i = 0; i < grids.size(); ++i){
        grids[i]->draw(image);
    }
    for(int i = 0; i < buttons.size(); ++i){
        buttons[i]->draw(image);
    }
    for(int i = 0; i < charactors.size(); ++i){
        charactors[i]->draw(image);
    }
}

void PlayScene::addPlant(Plant *p){
    charactors.push_back(p);
    plants.push_back(p);
}

void PlayScene::addZombie(Zombie *z){
    charactors.push_back(z);
    zombies.push_back(z);
}

void PlayScene::addBullet(Bullet *b){
    charactors.push_back(b);
    bullets.push_back(b);
}

void PlayScene::clear(){
    std::vector<Charactor*>::iterator i0 = charactors.begin();
    while(i0 != charactors.end()){
        if((*i0)->isDead()){
            i0 = charactors.erase(i0);
        }
        else{
            i0++;
        }
    }
    std::vector<Plant*>::iterator i1 = plants.begin();
    while(i1 != plants.end()){
        if((*i1)->isDead()){
            delete *i1;
            i1 = plants.erase(i1);
        }
        else{
            i1++;
        }
    }
    std::vector<Zombie*>::iterator i2 = zombies.begin();
    while(i2 != zombies.end()){
        if((*i2)->isDead()){
            delete *i2;
            i2 = zombies.erase(i2);
        }
        else{
            i2++;
        }
    }
    std::vector<Bullet*>::iterator i3 = bullets.begin();
    while(i3 != bullets.end()){
        if((*i3)->isDead()){
            delete *i3;
            i3 = bullets.erase(i3);
        }
        else{
            i3++;
        }
    }
}
