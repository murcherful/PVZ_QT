#include "myobject.h"

extern int gridHeight;
extern int gridWidth;

MyObject::MyObject(QObject *parent) : QObject(parent)
{
    x = 0;
    y = 0;
    w = 0;
    h = 0;
}

void MyObject::setName(const std::string name){
    this->name = name;
}

std::string MyObject::getName(){
    return name;
}

void MyObject::setPosition(int x, int y){
    this->x = x;
    this->y = y;
}

void MyObject::loadPicture(const std::string path){
    picture.load(path, 1.0);
    w = picture.width();
    h = picture.height();
}

void MyObject::update(){

}

void MyObject::draw(cv::Mat &image){
    picture.draw(x, y, image);
}

int MyObject::getX(){
    return x;
}

int MyObject::getY(){
    return y;
}

int MyObject::getW(){
    return w;
}

int MyObject::getH(){
    return h;
}

Grid::Grid(){
    blackCount = 0;
}

void Grid::loadBlackPicture(const std::string path){
    blackPicture.load(path, 1.0);
}

void Grid::update(){
    if(blackCount > 0){
        blackCount--;
    }
}

void Grid::black(){
    blackCount = GRID_BALCK_TIME;
}

void Grid::draw(cv::Mat &image){
    if(blackCount > 0){
        blackPicture.draw(x, y, image);
    }
    else{
        MyObject::draw(image);
    }
}

Charactor::Charactor(){
    attackStart = 0;
    attackCount = 0;
    isAttack = 0;
}

void Charactor::setGY(int gY){
    this->gY = gY;
}

int Charactor::getGY(){
    return gY;
}

int Charactor::getGX(){
    return gX;
}

void Charactor::setAttackAttributions(int hp, int attack, int defense, int attackSpeed){
    this->hp = hp;
    this->hpCopy = hp;
    this->attack = attack;
    this->defense = defense;
    this->attackSpeed = attackSpeed;
}

void Charactor::defend(int objAttack){
    if(objAttack == -1){
        hp = 0;
    }
    int hurt = (int)((double)objAttack*(1-(double)defense/(defense+ARG_DEFENSE)));
    if(hurt <= 0){
        hurt = 1;
    }
    if(hp-hurt<0){
        hp = 0;
    }
    else{
        hp -= hurt;
    }
}

bool Charactor::getIsAttack(){
    if(attackStart && attackCount == 1){
        return 1;
    }
    else{
        return 0;
    }
}

void Charactor::setPosition(int x, int y){
    MyObject::setPosition(x, y);
    gY = (y - GRID_Y)/gridHeight;
    gX = (x - GRID_X)/gridWidth;
}

bool Charactor::atPosition(int x, int y){
    return x == gX && y == gY;
}

int Charactor::getAttack(){
    return attack;
}

void Charactor::update(){
    if(attackStart){
        attackCount = (attackCount+1)%attackSpeed;
    }
    gX = (x - GRID_X)/gridWidth;
}

void Charactor::startAttack(){
    attackStart = 1;
}

void Charactor::stopAttack(){
    attackStart = 0;
}

void Charactor::draw(cv::Mat &image){
    MyObject::draw(image);
    std::stringstream hpString;
    hpString << name;
    int baseLine = 0;
    cv::Size textSize = cv::getTextSize(hpString.str(), cv::FONT_HERSHEY_SIMPLEX, 0.25, 1, &baseLine);
    cv::rectangle(image, cv::Point(x, y-textSize.height), cv::Point(x+textSize.width, y), cv::Scalar(200, 200, 200), -1);
    cv::rectangle(image, cv::Point(x, y-textSize.height), cv::Point(x+textSize.width*((double)hp/(double)hpCopy), y), cv::Scalar(106, 106, 255), -1);
    cv::rectangle(image, cv::Point(x-1, y-textSize.height-1), cv::Point(x+textSize.width+1, y+1), cv::Scalar(0, 0, 0), 1);
    cv::putText(image, hpString.str(), cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, 0.25, cv::Scalar(0, 0, 0), 1);
}

bool Charactor::getIsAttackStart(){
    return attackStart;
}

bool Charactor::isDead(){
    if(hp <= 0){
        return 1;
    }
    else{
        return 0;
    }
}

void Charactor::die(){
    hp = 0;
}

Plant::Plant(){
    isZombieValid = 1;
    needSunNumber = 0;
    cooldownTime = 0;
    hasBullet = 0;
    shootY = 0;
}

void Plant::setPlantAttributions(int needSunNumber, int cooldownTime, bool isZombieValid, bool hasBullet){
    this->needSunNumber = needSunNumber;
    this->cooldownTime = cooldownTime;
    this->isZombieValid = isZombieValid;
    this->hasBullet = hasBullet;
}

bool Plant::getIsZombieValid(){
    return isZombieValid;
}

bool Plant::getHasBullet(){
    return hasBullet;
}

int Plant::getNeedSunNumbwr(){
    return needSunNumber;
}

int Plant::getCooldownTime(){
    return cooldownTime;
}

MyPicture* Plant::getpicture(){
    return &picture;
}

void Plant::update(){
    Charactor::update();
    if(hasBullet && getIsAttack()){
        genBullet(x+getW(), y+shootY);
        //std::cout << "[Debug]: shoot" << std::endl;
        stopAttack();
    }
}

void Plant::setShootY(int shootY){
    this->shootY = shootY;
}

void Plant::interactive(Zombie *z){
    if(getHasBullet()){
        if(getGY() == z->getGY() && getX() <= z->getX()){
            if(!getIsAttackStart()){
                startAttack();
            }
        }
    }
    else if(attack != 0){
        if(getGY() == z->getGY() && getX()+getW() >= z->getX() && getX() <= z->getX()){
            if(!getIsAttackStart()){
                startAttack();
            }
            else if(getIsAttack()){
                z->defend(getAttack());
                stopAttack();
            }
        }
    }
}

Zombie::Zombie(){
    moveCount = 0;
    slowDownCount = 0;
    isInteractive = 0;
}

void Zombie::setZombieAttributions(double speed){
    this->speed = speed;
    this->speedCopy = speed;
}

void Zombie::setPosition(int x, int y){
    Charactor::setPosition(x, y);
    rx = x;
}

void Zombie::update(){
    Charactor::update();

    /*
    if(moveCount >= speed){
        x--;
        moveCount = 0;
    }
    */
    if(hp > 0 && x < GRID_X){
        getLeft(gY);
    }
    if(slowDownCount != 0){
        slowDownCount--;
    }
    else if(!Charactor::getIsAttackStart()&&attackCount!=1){
        //moveCount++;
        rx -= speed;
        x = rx;
    }
    if(!isInteractive){
        attackCount = 0;
        stopAttack();
    }
    isInteractive = 0;
}


void Zombie::interactive(Plant* p){
    if(p->getIsZombieValid() && getGY() == p->getGY() && (p->getX()+p->getW() >= getX() && p->getX() <= getX())){
        isInteractive = 1;
        if(!getIsAttackStart()){
            startAttack();
            //std::cout << "["
        }
        else if(getIsAttack()){
            p->defend(getAttack());
            //std::cout << "[Debug]: attack" << std::endl;
            stopAttack();
        }
    }
}

void Zombie::draw(cv::Mat &image){
    MyObject::draw(image);
    std::stringstream hpString;
    hpString << name;
    int baseLine = 0;
    cv::Size textSize = cv::getTextSize(hpString.str(), cv::FONT_HERSHEY_SIMPLEX, 0.25, 1, &baseLine);
    cv::rectangle(image, cv::Point(x, y-textSize.height), cv::Point(x+textSize.width, y), cv::Scalar(200, 200, 200), -1);
    if(slowDownCount != 0){
        cv::rectangle(image, cv::Point(x, y-textSize.height), cv::Point(x+textSize.width*((double)hp/(double)hpCopy), y), cv::Scalar(237, 149, 100), -1);
    }
    else{
        cv::rectangle(image, cv::Point(x, y-textSize.height), cv::Point(x+textSize.width*((double)hp/(double)hpCopy), y), cv::Scalar(106, 106, 255), -1);
    }

    cv::rectangle(image, cv::Point(x-1, y-textSize.height-1), cv::Point(x+textSize.width+1, y+1), cv::Scalar(0, 0, 0), 1);
    cv::putText(image, hpString.str(), cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, 0.25, cv::Scalar(0, 0, 0), 1);
}

void Zombie::slowDown(){
    slowDownCount = (PFS*0.4);
}

void Zombie::randomUpDonw(){
    if(y-gridHeight <= GRID_Y){
        y+=gridHeight;
        gY++;
    }
    else if(y+gridHeight >= MW_H){
        y-=gridHeight;
        gY--;
    }
    else{
        if(rand()%2){
            y+=gridHeight;
            gY++;
        }
        else{
            y-=gridHeight;
            gY--;
        }
    }
}

Bullet::Bullet(){
    hp = 1;
    defense = 0;
    attackSpeed = -1;
}

void Bullet::setAttackAttributions(int attack){
    Charactor::setAttackAttributions(1, attack, 0,  -1);
}

void Bullet::setBulletAttributions(int speed){
    this->speed = speed;
}

void Bullet::update(){
    x += speed;
    if(x+w >= MW_W || x < 0){
        hp = 0;
    }
}

void Bullet::draw(cv::Mat &image){
    MyObject::draw(image);
}


void Bullet::interactive(Zombie* z){
    if(isDead()){
        return;
    }
    if(z->getGY() == getGY() && getX()+getW() >= z->getX()){
        z->defend(attack);
        hp = 0;
    }
}

SunFlower::SunFlower(){
    setName("SunFlower");
    loadPicture(SOURCE_PATH+"SunFlower.png");
    setAttackAttributions(SUNFLOWER_HP, SUNFLOWER_ATTACK, SUNFLOWER_DEFENSE, SUNFLOWER_ATTACK_SPEED);
    setPlantAttributions(SUNFLOWER_NEED_SUN_NUMBER, SUNFLOWER_COOLDOWN_TIME, 1, 0);
    genSunCount = 0;
    genSunSpeed = SUNFLOWER_GEN_SUN_SPEED;
}

void SunFlower::update(){
    Charactor::update();
    if(genSunCount == 1){
        genSun(1);
    }
    genSunCount = (genSunCount+1)%genSunSpeed;
}

PeaShooter::PeaShooter(){
    setName("PeaShooter");
    setShootY(PEASHOOTER_SHOOT_Y);
    loadPicture(SOURCE_PATH+"PeaShooter.png");
    setAttackAttributions(PEASHOOTER_HP, PEASHOOTER_ATTACK, PEASHOOTER_DEFENSE, PEASHOOTER_ATTACK_SPEED);
    setPlantAttributions(PEASHOOTER_NEED_SUN_NUMBER, PEASHOOTER_COOLDOWN_TIME, 1, 1);
}

SnowPea::SnowPea(){
    setName("SnowPea");
    setShootY(SNOWPEA_SHOOT_Y);
    loadPicture(SOURCE_PATH+"SnowPea.png");
    setAttackAttributions(SNOWPEA_HP, SNOWPEA_ATTACK, SNOWPEA_DEFENSE, SNOWPEA_ATTACK_SPEED);
    setPlantAttributions(SNOWPEA_NEED_SUN_NUMBER, SNOWPEA_COOLDOWN_TIME, 1, 1);
}

MelonPult::MelonPult(){
    setName("MelonPult");
    setShootY(MELONPULT_SHOOT_Y);
    loadPicture(SOURCE_PATH+"MelonPult.png");
    setAttackAttributions(MELONPULT_HP, MELONPULT_ATTACK, MELONPULT_DEFENSE, MELONPULT_ATTACK_SPEED);
    setPlantAttributions(MELONPULT_NEED_SUN_NUMBER, MELONPULT_COOLDOWN_TIME, 1, 1);
}

SnowMelon::SnowMelon(){
    setName("SnowMelon");
    setShootY(SNOWMELON_SHOOT_Y);
    loadPicture(SOURCE_PATH+"SnowMelon.png");
    setAttackAttributions(SNOWMELON_HP, SNOWMELON_ATTACK, SNOWMELON_DEFENSE, SNOWMELON_ATTACK_SPEED);
    setPlantAttributions(SNOWMELON_NEED_SUN_NUMBER, SNOWMELON_COOLDOWN_TIME, 1, 1);
}

SpikeWeed::SpikeWeed(){
    setName("SpikeWeed");
    //setShootY(SPIKEWEED_SHOOT_Y);
    loadPicture(SOURCE_PATH+"SpikeWeed.png");
    setAttackAttributions(SPIKEWEED_HP, SPIKEWEED_ATTACK, SPIKEWEED_DEFENSE, SPIKEWEED_ATTACK_SPEED);
    setPlantAttributions(SPIKEWEED_NEED_SUN_NUMBER, SPIKEWEED_COOLDOWN_TIME, 0, 0);
}

void SpikeWeed::interactive(Zombie *z){
    if(getGY() == z->getGY() && getX()+getW() >= z->getX() && getX() <= z->getX()){
        if(!getIsAttackStart()){
            startAttack();
        }
        else if(getIsAttack()){
            attackCount++;
            attackSignal(gX, gY, attack);
            hp -= SPIKEWEED_HP_DESC;
            //std::cout << "[Debug]: sw attack" << std::endl;
            stopAttack();
        }
    }
}

Garlic::Garlic(){
    setName("Garlic");
    //setShootY(SPIKEWEED_SHOOT_Y);
    loadPicture(SOURCE_PATH+"Garlic.png");
    setAttackAttributions(GARLIC_HP, GARLIC_ATTACK, GARLIC_DEFENSE, SPIKEWEED_ATTACK_SPEED);
    setPlantAttributions(GARLIC_NEED_SUN_NUMBER, GARLIC_COOLDOWN_TIME, 0, 0);
}

void Garlic::interactive(Zombie *z){
    if(getGY() == z->getGY() && getX()+getW() >= z->getX() && getX() <= z->getX()){
        hp -= GARLIC_HP_DESC;
        //std::cout << "[Debug]: sw attack" << std::endl;
        //stopAttack();
        z->randomUpDonw();
    }
}

Chomper::Chomper(){
    setName("Chomper");
    //setShootY(SPIKEWEED_SHOOT_Y);
    loadPicture(SOURCE_PATH+"Chomper.png");
    setAttackAttributions(CHOMPER_HP, CHOMPER_ATTACK, CHOMPER_DEFENSE, CHOMPER_ATTACK_SPEED);
    setPlantAttributions(CHOMPER_NEED_SUN_NUMBER, CHOMPER_COOLDOWN_TIME, 1, 0);
}

void Chomper::update(){
    Charactor::update();
    if(attackCount != 0){
        attackCount--;
        if(attackCount == 0){
            loadPicture(SOURCE_PATH+"Chomper.png");
        }
    }
}

void Chomper::interactive(Zombie *z){
    if(getGY() == z->getGY() && getX()+getW() >= z->getX() && getX() <= z->getX()){
        attackCount = attackSpeed;
        z->defend(attack);
        loadPicture(SOURCE_PATH+"Chomper_close.png");
    }
}

Squash::Squash(){
    isBreak = 0;
    breakCount = 0;
    setName("Squash");
    //setShootY(SPIKEWEED_SHOOT_Y);
    loadPicture(SOURCE_PATH+"Squash.png");
    setAttackAttributions(SQUASH_HP, SQUASH_ATTACK, SQUASH_DEFENSE, SQUASH_ATTACK_SPEED);
    setPlantAttributions(SQUASH_NEED_SUN_NUMBER, SQUASH_COOLDOWN_TIME, 0, 0);
}

void Squash::update(){
    Plant::update();
    if(isBreak && breakCount != 0){
        breakCount--;
    }
    else if(isBreak && breakCount == 0){
        die();
    }
}

void Squash::interactive(Zombie *z){
    if(isBreak){
        return;
    }
    if(getGY() == z->getGY() && getX()+getW()+gridWidth/2 >= z->getX() && getX() <= z->getX()){
        squashBreak(gX+1, gY);
        x += gridWidth;
        gX++;
        isBreak = 1;
        breakCount = (PFS);
    }
}


NormalZombie::NormalZombie(){
    setName("NormalZombie");
    loadPicture(SOURCE_PATH+"NormalZombie.png");
    setAttackAttributions(NORMALZOMBIE_HP, NORMALZOMBIE_ATTACK, NORMALZOMBIE_DEFENSE, NORMALZOMBIE_ATTACK_SPEED);
    setZombieAttributions(NORMALZOMBIE_SPEED);
}

ConeZombie::ConeZombie(){
    setName("ConeZombie");
    loadPicture(SOURCE_PATH+"ConeZombie.png");
    setAttackAttributions(CONEZOMBIE_HP, CONEZOMBIE_ATTACK, CONEZOMBIE_DEFENSE, CONEZOMBIE_ATTACK_SPEED);
    setZombieAttributions(CONEZOMBIE_SPEED);
}

BucketZombie::BucketZombie(){
    setName("BucketZombie");
    loadPicture(SOURCE_PATH+"BucketZombie.png");
    setAttackAttributions(BUCKETZOMBIE_HP, BUCKETZOMBIE_ATTACK, BUCKETZOMBIE_DEFENSE, BUCKETZOMBIE_ATTACK_SPEED);
    setZombieAttributions(BUCKETZOMBIE_SPEED);
}


DoorZombie::DoorZombie(){
    setName("DoorZombie");
    loadPicture(SOURCE_PATH+"DoorZombie.png");
    setAttackAttributions(DOORZOMBIE_HP, DOORZOMBIE_ATTACK, DOORZOMBIE_DEFENSE, DOORZOMBIE_ATTACK_SPEED);
    setZombieAttributions(DOORZOMBIE_SPEED);
}


NewsZombie::NewsZombie(){
    isBreak = 0;
    setName("NewsZombie");
    loadPicture(SOURCE_PATH+"NewsZombie.png");
    setAttackAttributions(NEWSZOMBIE_HP, NEWSZOMBIE_ATTACK, NEWSZOMBIE_DEFENSE, NEWSZOMBIE_ATTACK_SPEED);
    setZombieAttributions(NEWSZOMBIE_SPEED);
}

void NewsZombie::update(){
    Zombie::update();
    if(!isBreak && hp < NEWSZOMBIE_HP*0.75){
        isBreak = 1;
        loadPicture(SOURCE_PATH+"NewsZombieBreak.png");
        setZombieAttributions(NEWSZOMBIE_SPEED*3);
    }
}

PoleZombie::PoleZombie(){
    isJump = 0;
    setName("PoleZombie");
    loadPicture(SOURCE_PATH+"PoleZombie.png");
    setAttackAttributions(POLEZOMBIE_HP, POLEZOMBIE_ATTACK, POLEZOMBIE_DEFENSE, POLEZOMBIE_ATTACK_SPEED);
    setZombieAttributions(POLEZOMBIE_SPEED);
}

void PoleZombie::interactive(Plant *p){
    if(p->getIsZombieValid() && getGY() == p->getGY() && (p->getX()+p->getW() >= getX() && p->getX() <= getX())){
        isInteractive = 1;
        if(isJump == 0){
            rx -= gridWidth;
            isJump = 1;
            speed = NORMALZOMBIE_SPEED;
            picture.load(SOURCE_PATH+"NormalZombie.png", 1);
            return;
        }
        if(!getIsAttackStart()){
            startAttack();
        }
        else if(getIsAttack()){
            p->defend(getAttack());
            stopAttack();
        }
    }
}

BackupZombie::BackupZombie(){
    setName("BackupZombie");
    loadPicture(SOURCE_PATH+"BackupZombie.png");
    setAttackAttributions(BACKUPZOMBIE_HP, BACKUPZOMBIE_ATTACK, BACKUPZOMBIE_DEFENSE, BACKUPZOMBIE_ATTACK_SPEED);
    setZombieAttributions(BACKUPZOMBIE_SPEED);
}

DancingZombie::DancingZombie(){
    genZomCount = 0;
    genZomSpeed = DANCINGZOMBIE_GEN_ZOM_SPEED;
    setName("DancingZombie");
    loadPicture(SOURCE_PATH+"DancingZombie.png");
    setAttackAttributions(DANCINGZOMBIE_HP, DANCINGZOMBIE_ATTACK, DANCINGZOMBIE_DEFENSE, DANCINGZOMBIE_ATTACK_SPEED);
    setZombieAttributions(DANCINGZOMBIE_SPEED);
}

void DancingZombie::update(){
    Zombie::update();
    genZomCount = (genZomCount+1)%genZomSpeed;
    if(genZomCount == genZomSpeed-1){
        genZombie(gX, gY);
    }
}

PeaBullet::PeaBullet(){
    setName("PeaBullet");
    loadPicture(SOURCE_PATH+"PeaBullet.png");
    setAttackAttributions(PEABULLET_ATTACK);
    setBulletAttributions(PEABULLET_SPEED);
}

SnowBullet::SnowBullet(){
    setName("SnowBullet");
    loadPicture(SOURCE_PATH+"SnowBullet.png");
    setAttackAttributions(SNOWBULLET_ATTACK);
    setBulletAttributions(SNOWBULLET_SPEED);
}

void SnowBullet::interactive(Zombie* z){
    if(isDead()){
        return;
    }
    if(z->getGY() == getGY() && getX()+getW() >= z->getX()){
        z->defend(attack);
        z->slowDown();
        hp = 0;
    }
}

MelonBullet::MelonBullet(){
    setName("MelonBullet");
    loadPicture(SOURCE_PATH+"MelonBullet.png");
    setAttackAttributions(MELONBULLET_ATTACK);
    setBulletAttributions(MELONBULLET_SPEED);
}

void MelonBullet::interactive(Zombie* z){
    if(isDead()){
        return;
    }
    if(z->getGY() == getGY() && getX()+getW() >= z->getX()){
        meloonBulletBreak(x, gY, attack);
        //z->defend(attack);
        //z->slowDown();
        hp = 0;
    }
}

SnowMelonBullet::SnowMelonBullet(){
    setName("SnowMelonBullet");
    loadPicture(SOURCE_PATH+"SnowMelonBullet.png");
    setAttackAttributions(SNOWMELONBULLET_ATTACK);
    setBulletAttributions(SNOWMELONBULLET_SPEED);
}

void SnowMelonBullet::interactive(Zombie* z){
    if(isDead()){
        return;
    }
    if(z->getGY() == getGY() && getX()+getW() >= z->getX()){
        snowMeloonBulletBreak(x, gY, attack);
        //z->defend(attack);
        //z->slowDown();
        hp = 0;
    }
}
