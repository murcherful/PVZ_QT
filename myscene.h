#ifndef MYSCENE_H
#define MYSCENE_H

#include "myobject.h"
#include "mybutton.h"
#include "mypicture.h"
#include <vector>

class MyScene:public MyObject
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
    Q_OBJECT
public:
    int shopX;
    int shopY;
    int shopN;
    int sunN;
    MyPicture sunPicture;
    std::vector<CooldownButton*> cooldownButtons;
    std::vector<Grid*> grids;
    std::vector<Sun*> suns;
    std::vector<Charactor*> charactors;
    std::vector<Plant*> plants;
    std::vector<Zombie*> zombies;
    std::vector<Bullet*> bullets;
    bool plantFlags[GRID_Y_N][GRID_X_N];
    bool weedKiller[GRID_Y_N];
    MyPicture weedKillerPicture;
    bool isCheckLeft;
    int leftN;
    int rightN;

    PlayScene();
    void addCooldownButton(CooldownButton* cooldownButton);
    void addSun(Sun* sun);
    Sun* getSun(int x, int y);
    void addGrids(Grid* grid);
    void update();
    void draw(cv::Mat &image);
    void addPlant(Plant* p, int x, int y);
    void addZombie(Zombie* z);
    void addBullet(Bullet* b);
    void clear();
    bool isGridValid(int x, int y);
    void removePlant(int x, int y);
    void clearLine(int gY);
    void reStart();
    void addPlantFromName(int plantX, int plantY, std::string plantName, CooldownButton* b);
    void changeSunSiganl();

signals:
    void gameOver();
    void changeSun(int sunN);

public slots:
    //void checkSun(int n);
    void addSunNumber(int n);
    void checkLeft(int gY);
    void addBackupZombie(int gX, int gY);

    void addPeaBullet(int x, int y);
    void addSnowBullet(int x, int y);
    void addMelonBullet(int x, int y);
    void addSnowMelonBullet(int x, int y);
    void melonBulletBreak(int x, int gY, int attack);
    void snowMelonBulletBreak(int x, int gY, int attack);
    void spikeWeedAttack(int gX, int gY, int attack);
    void squashAttack(int gX, int gY);
    void leftMove();
    void rightMove();
    void potatoMineAttack(int gX, int gY);
};

#endif // MYSCENE_H
