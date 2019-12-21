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
    virtual std::string getName();
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
    int gX;

    bool attackStart;
    int attackCount;
    bool isAttack;

    int hp;
    int hpCopy;
    int attack;
    int defense;
    int attackSpeed;

    Charactor();
    void setPosition(int x, int y);
    void update();
    void draw(cv::Mat &image);
    virtual void setGY(int gY);
    virtual int getGY();
    virtual int getGX();
    virtual void setAttackAttributions(int hp, int attack, int defense, int attackSpeed);
    virtual void defend(int objAttack);
    virtual bool getIsAttack();
    virtual int getAttack();
    virtual void startAttack();
    virtual void stopAttack();
    virtual bool getIsAttackStart();
    virtual bool isDead();
    virtual void die();
    virtual bool atPosition(int x, int y);
};

class Zombie;

class Plant : public Charactor{
    Q_OBJECT
public:
    bool isZombieValid;
    int needSunNumber;
    int cooldownTime;
    bool hasBullet;
    int shootY;

    Plant();
    void update();
    virtual void setPlantAttributions(int needSunNumber, int cooldownTime, bool isZombieValid, bool hasBullet);
    virtual void setShootY(int shootY);
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
    void draw(cv::Mat &image);
    //void draw(cv::Mat &image);
    virtual void interactive(Plant* p);
    virtual void slowDown();
    virtual void randomUpDonw();

signals:
    void getLeft(int gY);

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
#define SUNFLOWER_COOLDOWN_TIME   (PFS*7)

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
#define PEASHOOTER_ATTACK_SPEED    (PFS*2)
#define PEASHOOTER_NEED_SUN_NUMBER 8
#define PEASHOOTER_COOLDOWN_TIME   (PFS*10)
#define PEASHOOTER_SHOOT_Y         15

class PeaShooter:public Plant{
    Q_OBJECT
public:
    PeaShooter();
    //~PeaShooter();
};

#define SNOWPEA_HP              150
#define SNOWPEA_ATTACK          0
#define SNOWPEA_DEFENSE         50
#define SNOWPEA_ATTACK_SPEED    (PFS*2)
#define SNOWPEA_NEED_SUN_NUMBER 10
#define SNOWPEA_COOLDOWN_TIME   (PFS*14)
#define SNOWPEA_SHOOT_Y         15

class SnowPea:public Plant{
    private:
    public:
        SnowPea();
        //~SnowPea();
};

#define MELONPULT_HP              125
#define MELONPULT_ATTACK          0
#define MELONPULT_DEFENSE         50
#define MELONPULT_ATTACK_SPEED    (PFS*4)
#define MELONPULT_NEED_SUN_NUMBER 12
#define MELONPULT_COOLDOWN_TIME   (PFS*16)
#define MELONPULT_SHOOT_Y         15

class MelonPult:public Plant{
    private:
    public:
        MelonPult();
        //~MelonPult();
};

#define SNOWMELON_HP              125
#define SNOWMELON_ATTACK          0
#define SNOWMELON_DEFENSE         50
#define SNOWMELON_ATTACK_SPEED    (PFS*4)
#define SNOWMELON_NEED_SUN_NUMBER 16
#define SNOWMELON_COOLDOWN_TIME   (PFS*20)
#define SNOWMELON_SHOOT_Y         15

class SnowMelon:public Plant{
    private:
    public:
        SnowMelon();
        //~SnowMelon();
};

#define SPIKEWEED_HP              150
#define SPIKEWEED_ATTACK          25
#define SPIKEWEED_DEFENSE         50
#define SPIKEWEED_ATTACK_SPEED    (PFS)
#define SPIKEWEED_NEED_SUN_NUMBER 12
#define SPIKEWEED_COOLDOWN_TIME   (PFS*14)
#define SPIKEWEED_HP_DESC         5

class SpikeWeed:public Plant{
    Q_OBJECT
    private:
    public:
        SpikeWeed();
        //~SpikeWeed();
        void interactive(Zombie* z);
signals:
        void attackSignal(int gX, int gY, int attack);
};

#define NORMALZOMBIE_HP             180
#define NORMALZOMBIE_ATTACK         50
#define NORMALZOMBIE_DEFENSE        50
#define NORMALZOMBIE_ATTACK_SPEED   (PFS)
#define NORMALZOMBIE_SPEED          (1)

class NormalZombie:public Zombie{
    Q_OBJECT
public:
    NormalZombie();
    //~NormalZombie();
};


#define CONEZOMBIE_HP             220
#define CONEZOMBIE_ATTACK         50
#define CONEZOMBIE_DEFENSE        70
#define CONEZOMBIE_ATTACK_SPEED   (PFS)
#define CONEZOMBIE_SPEED          (1)

class ConeZombie:public Zombie{
    private:
    public:
        ConeZombie();
        //~ConeZombie();
};

#define BUCKETZOMBIE_HP             280
#define BUCKETZOMBIE_ATTACK         50
#define BUCKETZOMBIE_DEFENSE        120
#define BUCKETZOMBIE_ATTACK_SPEED   (PFS)
#define BUCKETZOMBIE_SPEED          (1)

class BucketZombie:public Zombie{
    private:
    public:
        BucketZombie();
        //~BucketZombie();
};

#define DOORZOMBIE_HP             400
#define DOORZOMBIE_ATTACK         50
#define DOORZOMBIE_DEFENSE        150
#define DOORZOMBIE_ATTACK_SPEED   (PFS)
#define DOORZOMBIE_SPEED          (1)

class DoorZombie:public Zombie{
    private:
    public:
        DoorZombie();
        //~DoorZombie();
};

#define NEWSZOMBIE_HP             220
#define NEWSZOMBIE_ATTACK         50
#define NEWSZOMBIE_DEFENSE        50
#define NEWSZOMBIE_ATTACK_SPEED   (PFS)
#define NEWSZOMBIE_SPEED          (1)

class NewsZombie:public Zombie{
    private:
    public:
        bool isBreak;
        NewsZombie();
        //~NewsZombie();
        void update();
};

#define POLEZOMBIE_HP             150
#define POLEZOMBIE_ATTACK         50
#define POLEZOMBIE_DEFENSE        50
#define POLEZOMBIE_ATTACK_SPEED   (PFS)
#define POLEZOMBIE_SPEED          (3)

class PoleZombie:public Zombie{
    private:
    public:
        bool isJump;

        PoleZombie();
        //~PoleZombie();
        void interactive(Plant* p);
};

#define BACKUPZOMBIE_HP             150
#define BACKUPZOMBIE_ATTACK         50
#define BACKUPZOMBIE_DEFENSE        50
#define BACKUPZOMBIE_ATTACK_SPEED   (PFS)
#define BACKUPZOMBIE_SPEED          (1)

class BackupZombie:public Zombie{
    private:
    public:

        BackupZombie();
        //~BackupZombie();
};

#define DANCINGZOMBIE_HP             200
#define DANCINGZOMBIE_ATTACK         50
#define DANCINGZOMBIE_DEFENSE        100
#define DANCINGZOMBIE_ATTACK_SPEED   (PFS)
#define DANCINGZOMBIE_SPEED          (1)
#define DANCINGZOMBIE_GEN_ZOM_SPEED  (PFS*10)

class DancingZombie:public Zombie{
    Q_OBJECT
    private:
    public:
        int genZomCount;
        int genZomSpeed;

        DancingZombie();
        //~DancingZombie();
        void update();
signals:
        void genZombie(int gX, int gY);
};

#define PEABULLET_SPEED      10
#define PEABULLET_ATTACK     50

class PeaBullet:public Bullet{
    Q_OBJECT
public:
    PeaBullet();
    //~PeaBullet();
};

#define SNOWBULLET_SPEED      10
#define SNOWBULLET_ATTACK     50

class SnowBullet:public Bullet{
    private:
    public:
        SnowBullet();
        void interactive(Zombie *z);
        //~SnowBullet();
};

#define MELONBULLET_SPEED      5
#define MELONBULLET_ATTACK     55

class MelonBullet:public Bullet{
   Q_OBJECT
    private:
    public:
        MelonBullet();
        void interactive(Zombie *z);
        //~SnowBullet();
signals:
        void meloonBulletBreak(int x, int gY, int attack);
};

#define SNOWMELONBULLET_SPEED      5
#define SNOWMELONBULLET_ATTACK     55

class SnowMelonBullet:public Bullet{
   Q_OBJECT
    private:
    public:
        SnowMelonBullet();
        void interactive(Zombie *z);
        //~SnowBullet();
signals:
        void snowMeloonBulletBreak(int x, int gY, int attack);
};

#endif // MYOBJECT_H
