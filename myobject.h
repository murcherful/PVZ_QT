#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>
#include <mypicture.h>
#include <mytool.h>

class MyObject : public QObject
{
    Q_OBJECT
public:
    explicit MyObject(QObject *parent = nullptr);

    int x,y,w,h;
    int type;
    std::string name;
    MyPicture picture;

    virtual void setName(const std::string name);
    virtual void setPosition(int x, int y);
    virtual void loadPicture(const std::string path);
    virtual void update();
    virtual void draw(cv::Mat &image);
    virtual int getX();
    virtual int getY();
    virtual int getW();
    virtual int getH();

signals:

public slots:
};

#define GRID_BALCK_TIME (PFS*2)

class Grid : public MyObject{
    Q_OBJECT
public:
    MyPicture blackPicture;
    int blackCount;

    Grid();
    void loadBlackPicture(const std::string path);
    void update();
    void draw(cv::Mat &image);
public slots:
    void black();
};

#define ARG_DEFENSE 50

class Charactor : public MyObject{
    Q_OBJECT
public:
    int gY;

    bool attackStart;
    int attackCount;
    bool isAttack;

    int hp;
    int attack;
    int defense;
    int attackSpeed;

    Charactor();
    void setPosition(int x, int y);
    void update();
    void draw(cv::Mat &image);
    virtual void setGY(int gY);
    virtual int getGY();
    virtual void setAttackAttributions(int hp, int attack, int defense, int attackSpeed);
    virtual void defend(int objAttack);
    virtual bool getIsAttack();
    virtual int getAttack();
    virtual void startAttack();
    virtual void stopAttack();
    virtual bool getIsAttackStart();
    virtual bool isDead();
};

class Zombie;

class Plant : public Charactor{
    Q_OBJECT
public:
    bool isZombieValid;
    int needSunNumber;
    int cooldownTime;
    bool hasBullet;

    Plant();
    void update();
    virtual void setPlantAttributions(int needSunNumber, int cooldownTime, bool isZombieValid, bool hasBullet);
    virtual bool getIsZombieValid();
    virtual bool getHasBullet();
    virtual int getNeedSunNumbwr();
    virtual int getCooldownTime();
    virtual void interactive(Zombie* z);
    virtual MyPicture* getpicture();

signals:
    void genBullet(int x, int y);
    void genSun(int n);

};

class Zombie : public Charactor{
    Q_OBJECT
public:
    double speed;
    double rx;

    double speedCopy;
    int moveCount;
    int slowDownCount;
    bool isInteractive;

    Zombie();
    void setPosition(int x, int y);
    virtual void setZombieAttributions(double speed);
    void update();
    //void draw(cv::Mat &image);
    virtual void interactive(Plant* p);
    virtual void slowDown();
    virtual void randomUpDonw();

};

class Bullet : public Charactor{
    Q_OBJECT
public:
    int speed;

    Bullet();
    void setAttackAttributions(int attack);
    virtual void setBulletAttributions(int speed);
    void update();
    void draw(cv::Mat &image);
    virtual void interactive(Zombie* z);
};


#define SUNFLOWER_GEN_SUN_SPEED   (PFS*4)
#define SUNFLOWER_HP              150
#define SUNFLOWER_ATTACK          0
#define SUNFLOWER_DEFENSE         70
#define SUNFLOWER_ATTACK_SPEED    10000
#define SUNFLOWER_NEED_SUN_NUMBER 4
#define SUNFLOWER_COOLDOWN_TIME   70

class SunFlower : public Plant{
    Q_OBJECT
public:
    int genSunCount;
    int genSunSpeed;

    SunFlower();
    //~SunFlower();
    void update();
};


#define PEASHOOTER_HP              125
#define PEASHOOTER_ATTACK          0
#define PEASHOOTER_DEFENSE         50
#define PEASHOOTER_ATTACK_SPEED    20
#define PEASHOOTER_NEED_SUN_NUMBER 8
#define PEASHOOTER_COOLDOWN_TIME   100

class PeaShooter:public Plant{
    Q_OBJECT
public:
    PeaShooter();
    //~PeaShooter();
};


#define NORMALZOMBIE_HP             180
#define NORMALZOMBIE_ATTACK         50
#define NORMALZOMBIE_DEFENSE        50
#define NORMALZOMBIE_ATTACK_SPEED   (PFS)
#define NORMALZOMBIE_SPEED          (PFS/100.0)

class NormalZombie:public Zombie{
    Q_OBJECT
public:
    NormalZombie();
    //~NormalZombie();
};


#define PEABULLET_SPEED      2
#define PEABULLET_ATTACK     50

class PeaBullet:public Bullet{
    Q_OBJECT
public:
    PeaBullet();
    //~PeaBullet();
};

#endif // MYOBJECT_H