#include "myscene.h"

extern int gridWidth;
extern int gridHeight;

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

#define SHOP_WDITH 100

PlayScene::PlayScene(){
    shopX = 150;
    shopY = 20;
    shopN = 8;
    sunN = 9999;
    leftN = 0;
    rightN = 0;
    for(int i = 0; i < GRID_Y_N; ++i){
        weedKiller[i] = 1;
        for(int j = 0; j < GRID_X_N; ++j){
            plantFlags[i][j] = 0;
        }
    }
    //weedKiller[2] = 0;
    sunPicture.load(SOURCE_PATH+"sunBig.png", 1);
    weedKillerPicture.load(SOURCE_PATH+"weedKiller.png", 1);
    isCheckLeft = 0;
}

void PlayScene::addCooldownButton(CooldownButton *cooldownButton){
    int n = cooldownButtons.size();
    cooldownButton->setPosition(shopX+n*SHOP_WDITH, shopY);
    if(n >= shopN){
        cooldownButton->setDeactive();
        rightN++;
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
    if(isCheckLeft){
        isCheckLeft = 0;
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
    int sunX = 20;
    int sunY = 20;

    std::stringstream ss;
    ss << sunN;
    //cv::rectangle(image, cv::Point(sunX, sunY), cv::Point(sunX+sunPicture.width(), sunY+sunPicture.height()), cv::Scalar(255, 255, 255), -1);
    cv::rectangle(image, cv::Point(sunX, sunY), cv::Point(sunX+sunPicture.width(), sunY+sunPicture.height()), cv::Scalar(20, 105, 139), 3);
    sunPicture.draw(sunX, sunY, image);
    cv::putText(image, ss.str(), cv::Point(sunX, sunY+sunPicture.height()+25), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 2);
    for(int i = 0; i < GRID_Y_N; ++i){
        if(weedKiller[i]){
            weedKillerPicture.draw(GRID_X-weedKillerPicture.width(), GRID_Y+(gridHeight-weedKillerPicture.height())/2+gridHeight*i, image);
        }
    }
}

void PlayScene::addPlant(Plant *p, int x, int y){
    if(plantFlags[y][x]){
        return;
    }
    int pX = GRID_X+gridWidth*x+gridWidth/2-p->getW()/2;
    int pY = GRID_Y+gridHeight*y+gridHeight/2-p->getH()/2;
    p->setPosition(pX, pY);
    charactors.push_back(p);
    plants.push_back(p);
    plantFlags[y][x] = 1;
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
            int gx = ((*i1)->getX() - GRID_X)/gridWidth;
            int gy = ((*i1)->getY() - GRID_Y)/gridHeight;
            delete *i1;
            i1 = plants.erase(i1);
            plantFlags[gy][gx] = 0;
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

Sun* PlayScene::getSun(int x, int y){
    return suns[y*GRID_X_N+x];
}

/*
void PlayScene::checkSun(int n){
    sunN = n;
}
*/

bool PlayScene::isGridValid(int x, int y){
    return plantFlags[y][x];
}

void PlayScene::removePlant(int x, int y){
    if(plantFlags[y][x]){
        for(int i = 0; i < plants.size(); ++i){
            if(plants[i]->atPosition(x, y)){
                plants[i]->die();break;
            }
        }
    }
}

void PlayScene::checkLeft(int gY){
    if(isCheckLeft){
        return;
    }
    else{
        isCheckLeft = 1;
    }
    if(weedKiller[gY]){
        weedKiller[gY] = 0;
        clearLine(gY);
    }
    else{
        std::cout << "[Debug]: Game Over" << std::endl;
        reStart();
        gameOver();
    }
}

void PlayScene::clearLine(int gY){
    for(int i = 0; i <GRID_X_N; ++i){
        grids[gY*GRID_X_N+i]->black();
    }
    for(int i = 0 ;i < zombies.size(); ++i){
        if(zombies[i]->getGY() == gY){
            zombies[i]->die();
        }
    }
}

void PlayScene::reStart(){
    for(int i = 0; i < GRID_Y_N; ++i){
        weedKiller[i] = 1;
        for(int j = 0; j < GRID_X_N; ++j){
            plantFlags[i][j] = 0;
        }
    }
    plants.clear();
    zombies.clear();
    bullets.clear();
    for(int i = 0; i < charactors.size(); ++i){
        delete charactors[i];
    }
    charactors.clear();
    for(int i = 0; i <cooldownButtons.size(); ++i){
        cooldownButtons[i]->reSet();
    }
    for(int i = 0; i < suns.size(); ++i){
        suns[i]->clearSun();
    }
    sunN = 10;
    changeSun(sunN);
}

void PlayScene::addBackupZombie(int gX, int gY){
    BackupZombie* baz;
    if(gX-1 >= 0){
        baz = new BackupZombie();
        baz->setPosition(GRID_X+gridWidth*(gX-1), GRID_Y+gridHeight*gY);
        connect(baz, &Zombie::getLeft, this, &PlayScene::checkLeft);
        addZombie(baz);
    }
    if(gX+1 < GRID_X_N){
        baz = new BackupZombie();
        baz->setPosition(GRID_X+gridWidth*(gX+1), GRID_Y+gridHeight*gY);
        connect(baz, &Zombie::getLeft, this, &PlayScene::checkLeft);
        addZombie(baz);
    }
    if(gY-1 >= 0){
        baz = new BackupZombie();
        baz->setPosition(GRID_X+gridWidth*gX, GRID_Y+gridHeight*(gY-1));
        connect(baz, &Zombie::getLeft, this, &PlayScene::checkLeft);
        addZombie(baz);
    }
    if(gY+1 < GRID_Y_N){
        baz = new BackupZombie();
        baz->setPosition(GRID_X+gridWidth*gX, GRID_Y+gridHeight*(gY+1));
        connect(baz, &Zombie::getLeft, this, &PlayScene::checkLeft);
        addZombie(baz);
    }

}

void PlayScene::addPeaBullet(int x, int y){
    PeaBullet* pb = new PeaBullet();
    pb->setPosition(x, y);
    addBullet(pb);
}

void PlayScene::addSnowBullet(int x, int y){
    SnowBullet* sb = new SnowBullet();
    sb->setPosition(x, y);
    addBullet(sb);
}

void PlayScene::addMelonBullet(int x, int y){
    MelonBullet* mb = new MelonBullet();
    mb->setPosition(x, y);
    connect(mb, &MelonBullet::meloonBulletBreak, this, &PlayScene::melonBulletBreak);
    addBullet(mb);
}

void PlayScene::addSnowMelonBullet(int x, int y){
    SnowMelonBullet* smb = new SnowMelonBullet();
    smb->setPosition(x, y);
    connect(smb, &SnowMelonBullet::snowMeloonBulletBreak, this, &PlayScene::snowMelonBulletBreak);
    addBullet(smb);
}

void PlayScene::addSunNumber(int n){
    sunN += n;
    changeSun(sunN);
}

void PlayScene::addPlantFromName(int plantX, int plantY, std::string plantName, CooldownButton *b){
    if(plantX < 0 || plantX >= GRID_X_N ||plantY < 0 || plantY >= GRID_Y_N){
        return;
    }
    //std::cout << "[Debug]: plantX:" << plantX << " plantY:" << plantY << std::endl;
    if(isGridValid(plantX, plantY)){
        return;
    }
    //std::cout << "[Debug]: plantName" << plantName << std::endl;
    if(plantName == "PeaShooter"){
        PeaShooter* ps = new PeaShooter();
        connect(ps, &PeaShooter::genBullet, this, &PlayScene::addPeaBullet);
        addPlant(ps, plantX, plantY);
    }
    else if(plantName == "SunFlower"){
        SunFlower* sf = new SunFlower();
        connect(sf, &SunFlower::genSun, getSun(plantX, plantY), &Sun::addSun);
        addPlant(sf, plantX, plantY);

    }
    else if(plantName == "SnowPea"){
        SnowPea* sp = new SnowPea();
        connect(sp, &SnowPea::genBullet, this, &PlayScene::addSnowBullet);
        addPlant(sp, plantX, plantY);
    }
    else if(plantName == "MelonPult"){
        MelonPult* mp = new MelonPult();
        connect(mp, &MelonPult::genBullet, this, &PlayScene::addMelonBullet);
        addPlant(mp, plantX, plantY);
    }
    else if(plantName == "SnowMelon"){
        SnowMelon* sm = new SnowMelon();
        connect(sm, &SnowMelon::genBullet, this, &PlayScene::addSnowMelonBullet);
        addPlant(sm, plantX, plantY);
    }
    else if(plantName == "SpikeWeed"){
        SpikeWeed* sw = new SpikeWeed();
        connect(sw, &SpikeWeed::attackSignal, this, &PlayScene::spikeWeedAttack);
        addPlant(sw, plantX, plantY);
    }
    else if(plantName == "Garlic"){
        Garlic* g = new Garlic();
        addPlant(g, plantX, plantY);
    }
    else if(plantName == "Chomper"){
        Chomper* c = new Chomper();
        addPlant(c, plantX, plantY);
    }
    else if(plantName == "Squash"){
        Squash* s = new Squash();
        connect(s, &Squash::squashBreak, this, &PlayScene::squashAttack);
        addPlant(s, plantX, plantY);
    }
    b->cooldown();
    sunN -= b->getCost();
    changeSun(sunN);
    return;
}

void PlayScene::changeSunSiganl(){
    changeSun(sunN);
}

void PlayScene::melonBulletBreak(int x, int gY, int attack){
    for(int i = 0; i < zombies.size(); ++i){
        int zgY = zombies[i]->getGY();
        int zx = zombies[i]->getX();
        if(zgY == gY || zgY == gY-1 || zgY == gY+1){
            if(zx < x +  gridWidth && zx > x - gridWidth){
                zombies[i]->defend(attack);
            }
        }
    }
}

void PlayScene::snowMelonBulletBreak(int x, int gY, int attack){
    for(int i = 0; i < zombies.size(); ++i){
        int zgY = zombies[i]->getGY();
        int zx = zombies[i]->getX();
        if(zgY == gY || zgY == gY-1 || zgY == gY+1){
            if(zx < x + gridWidth && zx > x - gridWidth){
                zombies[i]->defend(attack);
                zombies[i]->slowDown();
            }
        }
    }
}

void PlayScene::spikeWeedAttack(int gX, int gY, int attack){
    for(int i = 0; i < zombies.size(); ++i){
        if(zombies[i]->getGY() == gY && zombies[i]->getGX() == gX){
            zombies[i]->defend(attack);
        }
    }
}

void PlayScene::squashAttack(int gX, int gY){
    grids[gY*GRID_X_N+gX]->black();
    for(int i = 0; i < zombies.size(); ++i){
        if(zombies[i]->getGY() == gY && zombies[i]->getGX() == gX){
            zombies[i]->die();
        }
    }
}

void PlayScene::leftMove(){
    if(leftN == 0){
        return;
    }
    for(int i = 0; i < cooldownButtons.size(); ++i){
        int x = cooldownButtons[i]->getX();
        int y = cooldownButtons[i]->getY();
        cooldownButtons[i]->setPosition(x+SHOP_WDITH, y);
        if(x+SHOP_WDITH < shopX || x+SHOP_WDITH >= shopX+shopN*SHOP_WDITH){
            cooldownButtons[i]->setDeactive();
        }
        else{
            cooldownButtons[i]->setActive();
        }
    }
    leftN--;
    rightN++;
}

void PlayScene::rightMove(){
    if(rightN == 0){
        return;
    }
    for(int i = 0; i < cooldownButtons.size(); ++i){
        int x = cooldownButtons[i]->getX();
        int y = cooldownButtons[i]->getY();
        cooldownButtons[i]->setPosition(x-SHOP_WDITH, y);
        if(x-SHOP_WDITH < shopX || x-SHOP_WDITH >= shopX+shopN*SHOP_WDITH){
            cooldownButtons[i]->setDeactive();
        }
        else{
            cooldownButtons[i]->setActive();
        }
    }
    rightN--;
    leftN++;
}
