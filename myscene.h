#ifndef MYSCENE_H
#define MYSCENE_H

#include "myobject.h"
#include "mybutton.h"
#include "mypicture.h"
#include <vector>

class MyScene
{
public:
    MyScene();
    ~MyScene();

    std::vector<MyButton*> buttons;
    MyPicture background;
    int isPush;
    MyButton* currentButton;

    virtual void load(const std::string path);
    virtual void update();
    virtual void draw(cv::Mat &image);
    virtual void push(int x, int y);
    virtual void release();
    virtual void addButton(MyButton* button);
};

class PlayScene: public MyScene{
public:
    int shopX;
    int shopY;
    int shopN;
    std::vector<CooldownButton*> cooldownButtons;
    std::vector<Grid*> grids;
    std::vector<Sun*> suns;
    std::vector<Charactor*> charactors;
    std::vector<Plant*> plants;
    std::vector<Zombie*> zombies;
    std::vector<Bullet*> bullets;

    PlayScene();
    void addCooldownButton(CooldownButton* cooldownButton);
    void addSun(Sun* sun);
    void addGrids(Grid* grid);
    void update();
    void draw(cv::Mat &image);
    void addPlant(Plant* p);
    void addZombie(Zombie* z);
    void addBullet(Bullet* b);
    void clear();
};

#endif // MYSCENE_H
