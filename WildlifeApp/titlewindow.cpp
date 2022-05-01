/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#include "titlewindow.h"
#include "ui_titlewindow.h"
#include <QFontDatabase>
#include <iostream>
#include <QDialog>
#include <QFormLayout>
#include <thread>
#include "model.h"
#include <queue>

TitleWindow::TitleWindow(QWidget *parent, Model * _model)
    : QMainWindow(parent)
    , ui(new Ui::TitleWindow) {
    ui->setupUi(this);
    model = _model;

    ui->welcomeLabel->setPixmap(QPixmap(":/Assets/Graphics/TitleButtons/Logo.png").scaled(385,170,Qt::KeepAspectRatio,Qt::FastTransformation));

    // welcome label setup
    ui->welcomeLabel->setStyleSheet("color: white");
    // setup pixel font
    QFontDatabase::addApplicationFont ( ":/Assets/Fonts/apple_kid.ttf" );
    QFont pixelFont("apple kid", 28, QFont::Normal);
    ui->logoTextLabel->setFont(pixelFont);

    childWindows = new std::vector<QWidget*>();
    // connections
    QObject::connect(ui->learnButton, &QPushButton::clicked, this, &TitleWindow::learnClicked);
    QObject::connect(ui->practiceButton, &QPushButton::clicked, this, &TitleWindow::practiceClicked);
    QObject::connect(ui->adventureButton, &QPushButton::clicked, this, &TitleWindow::adventureClicked);
    QObject::connect(ui->infoButton, &QPushButton::clicked, this, &TitleWindow::infoButtonClicked);
    //connect to model
    QObject::connect(this, &TitleWindow::getBestiary, model, &Model::getBestiary);
    QObject::connect(model, &Model::retrievedBestiary, this, &TitleWindow::initializeRPGWindow);
}

void TitleWindow::infoButtonClicked() {
    QDialog* infoDialog = new QDialog(this);
    infoDialog->setWindowTitle("About this app...");
    infoDialog->setStyleSheet("color: white");

    QFormLayout form(infoDialog);
    QLabel* infoText = new QLabel("This app was created by The Task Managers for Professor David Johnson's CS 3505 "
                                  "class during fall 2019 at the University of Utah.\n\nWe claim no responsibility for the "
                                  "consequences of taking our advice, as nature is unpredictable and diverse. This app is "
                                  "not meant to ensure one's survival in an emergency situation.\n\nOur rule of thumb is: "
                                  "if you don't know what it is, don't eat it, touch it, or challenge it. If you find yourself "
                                  "in trouble, please call the appropriate backup to request medical services or other help."
                                  "\n\nHope you enjoy!\n\n--The Task Managers.");
    QFontDatabase::addApplicationFont ( ":/Assets/Fonts/apple_kid.ttf" );
    QFont pixelFont("Apple Kid", 25, QFont::Normal);
    infoText->setFont(pixelFont);
    infoText->setWordWrap(true);
    form.addRow(infoText);
    infoDialog->exec();
}

void TitleWindow::learnClicked() {
    BestiaryWindow* learnWindow = new BestiaryWindow(this, model,false);
    makeNewWindow(learnWindow);
}

void TitleWindow::practiceClicked() {
    emit getBestiary(true);
}

void TitleWindow::adventureClicked() {
    emit getBestiary(false);
}

void TitleWindow::initializeRPGWindow(std::vector<Wildlife> battleBestiary, bool isPractice){
    if(isPractice){
        // one health because health does not matter as long it is >0 in this mode
        rpg = new RPG(1, battleBestiary, true);
        BestiaryWindow* BW = new BestiaryWindow(this,model,true);
        BW->hide();
        PracticeWindow* pracWindow = new PracticeWindow(this, rpg,BW, model);
        makeNewWindow(pracWindow);
    }
    else{
    rpg = new RPG(100, battleBestiary, false);
    RPGWindow* advWindow = new RPGWindow(this , rpg,false, model);
    makeNewWindow(advWindow);
    }
}

void TitleWindow::makeNewWindow(QWidget* newWindow) {
    newWindow->show();
    childWindows->push_back(newWindow);
    hide();
}

void TitleWindow::closeEvent (QCloseEvent* event) {
    // close all children windows
    for (uint i = 0; i < childWindows->size(); i ++) {
        childWindows->at(i)->close();
    }
    close();
}

TitleWindow::~TitleWindow() {
    delete ui;
    delete childWindows;
    delete rpg;
    // don't delete model because it's already left scope -- the resource it manages has already been deleted.
}

void TitleWindow::setPressed(QPushButton * button, bool pressed){
    QString path = ":/Assets/Graphics/TitleButtons/"+ button->objectName();
    if(pressed){
        path+="_P";
    }
    path+=".png";
    button->setIcon(QIcon(path));
    repaint();
}

void TitleWindow::on_learnButton_pressed(){
    setPressed(ui->learnButton,true);
}

void TitleWindow::on_practiceButton_pressed(){
    setPressed(ui->practiceButton,true);
}

void TitleWindow::on_adventureButton_pressed(){
    setPressed(ui->adventureButton,true);
}

void TitleWindow::on_infoButton_pressed(){
    setPressed(ui->infoButton,true);
}

void TitleWindow::on_learnButton_released(){
    setPressed(ui->learnButton,false);
}


void TitleWindow::on_practiceButton_released(){
    setPressed(ui->practiceButton,false);
}

void TitleWindow::on_adventureButton_released(){
    setPressed(ui->adventureButton,false);
}

void TitleWindow::on_infoButton_released(){
    setPressed(ui->infoButton,false);
}
