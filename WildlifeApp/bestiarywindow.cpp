/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#include "bestiarywindow.h"
#include "ui_bestiarywindow.h"
#include <QFontDatabase>
#include "flora.h"
#include "fauna.h"

BestiaryWindow::BestiaryWindow(QMainWindow* parent, Model* model,bool isPracticeWindows) :
    QWidget(),
    model(model),
    parent_(parent),
    ui(new Ui::bestiaryWindow),
    belongsToPracticeWindow(isPracticeWindows) {

    ui->setupUi(this);
    animationTimer = new QTimer;
    creaturesList = new std::vector<Wildlife>;
    currentCreature = nullptr;
    currentIdx = 0;

    // connect some things
    QObject::connect(ui->nextButton, &QPushButton::clicked, this, &BestiaryWindow::nextClicked);
    QObject::connect(ui->prevButton, &QPushButton::clicked, this, &BestiaryWindow::prevClicked);
    QObject::connect(ui->jumpToPlantsButton, &QPushButton::clicked, this, &BestiaryWindow::jumpToPlantsClicked);
    QObject::connect(ui->jumpToFungiButton, &QPushButton::clicked, this, &BestiaryWindow::jumpToFungiClicked);
    QObject::connect(ui->jumpToAnimalsButton, &QPushButton::clicked, this, &BestiaryWindow::jumpToAnimalsClicked);

    //animation
    QObject::connect(model, &Model::updateEnemyLocation, this, &BestiaryWindow::updateEnemyPosition);

    if(isPracticeWindows) {
        ui->returnToTitleButton->hide();
    }
    else {
        QObject::connect(ui->returnToTitleButton, &QPushButton::clicked, this, &BestiaryWindow::backToMainWindowClicked);
        setPixelFont(ui->returnToTitleButton, 25);
    }

    // set up custom font
    QFontDatabase::addApplicationFont ( ":/Assets/Fonts/apple_kid.ttf" );
    setPixelFont(ui->prevButton, 25);
    setPixelFont(ui->nextButton, 25);
    setPixelFont(ui->nameLabel, 30);
    setPixelFont(ui->jumpToPlantsButton, 30);
    setPixelFont(ui->jumpToAnimalsButton, 30);
    setPixelFont(ui->jumpToFungiButton, 30);

    // get description label ready to recieve text
    descriptionLabel = new QLabel;
    descriptionLabel->setWordWrap(true);
    descriptionLabel->QWidget::adjustSize();
    descriptionLabel->setMaximumWidth(1100);
    descriptionLabel->setMargin(20);
    setPixelFont(descriptionLabel, 27);
    ui->descriptionScroll->setWidget(descriptionLabel);

    // creature stuff
    setUpCreatures();
    updateGUI();
    setUpIndeces();

    // background stuff
    background = &bestiaryBG;
    backgroundTimer = new QTimer(this);
    connect(backgroundTimer, SIGNAL(timeout()), this,SLOT(doBackgroundAnimation()));
    backgroundTimer->start(50);
}


void BestiaryWindow::setUpIndeces() {
    plantIndex = 0;
    for (uint i = 0; i < creaturesList->size(); i ++) {
        if (creaturesList->at(i).name == "Bighorn Sheep") {
            animalIndex = i;
        } else if (creaturesList->at(i).name == "Chanterelle") {
            fungusIndex = i;
            return;
        }
    }
}

void BestiaryWindow::setPixelFont(QLabel* labelToSet, int fontPoint) {
    QFont pixelFont("apple kid", fontPoint, QFont::Normal);
    labelToSet->setFont(pixelFont);
}

void BestiaryWindow::setPixelFont(QScrollArea* areaToSet, int fontPoint) {
    QFont pixelFont("apple kid", fontPoint, QFont::Normal);
    areaToSet->setFont(pixelFont);
}

void BestiaryWindow::setPixelFont(QPushButton* buttonToSet, int fontPoint) {
    QFont pixelFont("apple kid", fontPoint, QFont::Normal);
    buttonToSet->setFont(pixelFont);
}

void BestiaryWindow::backToMainWindowClicked() {
    closeEvent();
}

void BestiaryWindow::closeEvent(QCloseEvent* event) {
    if(!belongsToPracticeWindow) {
        parent_->show();
    }
    close();
}

void BestiaryWindow::setUpCreatures() {
    creaturesList = model->bestiary;
    // default to beginning of bestiary.
    currentCreature = &creaturesList->at(0);
}

void BestiaryWindow::nextClicked() {
    // if we're at the end of the list, loop back around to index 0.
    if (currentIdx == creaturesList->size() - 1) {
        currentIdx = 0;
    }
    else { currentIdx++; }
    currentCreature = &creaturesList->at(currentIdx);
    updateGUI();
}

void BestiaryWindow::prevClicked() {
    // if we're at the very beginning of the list, loop back around to the end.
    if (currentIdx == 0) {
        currentIdx = uint(creaturesList->size() - 1);
    }
    else { currentIdx--; }
    currentCreature = &creaturesList->at(currentIdx);
    updateGUI();
}

void BestiaryWindow::setPressed(QPushButton * button, bool pressed){
    QString path = ":/Assets/Graphics/Bestiary/"+ button->objectName();
    if(pressed){
        path+="_P";
    }
    path+=".png";
    button->setIcon(QIcon(path));
    repaint();
}

void BestiaryWindow::updateGUI() {
    // prevent immediate crashing in case currentCreature has not been instantiated.
    if (!currentCreature) { return; }
    ui->nameLabel->setText(currentCreature->name);
    descriptionLabel->setText(currentCreature->description);
    descriptionLabel->QWidget::adjustSize();
    QIcon icon(":/Assets/Graphics/Wildlife/" + currentCreature->name + ".png");
    QPixmap pixmap = icon.pixmap(QSize(280, 280));
    ui->pictureLabel->setPixmap(pixmap);
}

void BestiaryWindow::updateEnemyPosition(double x, double y) {
    //smaller space, need to apply some transformation.
    //Nonintuitive movement pattern: keep multiplication lower to minimize irritation/frustrating motion
    int newX = int(cos(x)*7.0);
    int newY = int(sin(y)*7.0);
    ui->pictureLabel->move(870+newX, 150+newY);
}

void BestiaryWindow::jumpToAnimalsClicked() {
    currentIdx = animalIndex;
    currentCreature = &creaturesList->at(currentIdx);
    updateGUI();
}
void BestiaryWindow::jumpToPlantsClicked() {
    currentIdx = plantIndex;
    currentCreature = &creaturesList->at(currentIdx);
    updateGUI();
}

void BestiaryWindow::jumpToFungiClicked() {
    currentIdx = fungusIndex;
    currentCreature = &creaturesList->at(currentIdx);
    updateGUI();
}

void BestiaryWindow::doBackgroundAnimation() {
    QPalette palette;
    palette.setBrush(QPalette::Background,background->getNextFrame().scaled(240,240,Qt::KeepAspectRatio,Qt::FastTransformation));
    this->setPalette(palette);
}


BestiaryWindow::~BestiaryWindow() {
    delete ui;
    delete creaturesList;
    delete currentCreature;
    delete animationTimer;
    delete parent_;
}

void BestiaryWindow::on_prevButton_pressed(){
    setPressed(ui->prevButton,true);
}

void BestiaryWindow::on_prevButton_released(){
    setPressed(ui->prevButton,false);
}

void BestiaryWindow::on_nextButton_pressed(){
    setPressed(ui->nextButton,true);
}

void BestiaryWindow::on_nextButton_released(){
    setPressed(ui->nextButton,false);
}
