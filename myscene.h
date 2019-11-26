#ifndef MYSCENE_H
#define MYSCENE_H

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

    void load(const std::string path);
    void update();
    void draw(cv::Mat &image);
    void push(int x, int y);
    void release();
    void addButton(MyButton* button);
};

class PlayScene: public MyScene{
public:
    int shopX;
    int shopY;
    int shopN;
    std::vector<CooldownButton*> cooldownButtons;

    PlayScene();
    void addCooldownButton(CooldownButton* cooldownButton);
};

#endif // MYSCENE_H
