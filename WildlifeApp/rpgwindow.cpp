/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#include "rpgwindow.h"
#include "ui_rpgwindow.h"
#include <stdlib.h>
#include <thread>
#include <model.h>
#include <backgroundanimation.h>
#include <windows.h>
#include <rpg.h>
#include <QTimer>
#include <QFontDatabase>
#include <QQueue>
#include <QSound>
#include <time.h>
#include <QMessageBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDesktopWidget>
#include <iostream>


RPGWindow::RPGWindow(QMainWindow* parent, RPG * _rpg,bool _isPractice, Model* _model) :
    QWidget(),
    ui(new Ui::rpgwindow),
    model(_model),
    parent_(parent),
    rpg(_rpg) {

    ui->setupUi(this);
    soundTimer = new QTimer;
    textTimer = new QTimer(this);
    textTimer->start(0);
    textTimer->stop();
    std::srand(std::time(NULL));
    isBackgroundEmpty=false;
    isPractice=_isPractice;

    //set up fonts
    QFontDatabase::addApplicationFont ( ":/Assets/Fonts/apple_kid.ttf" );
    QFont pixelFont("apple kid", 34, QFont::Normal);
    //set font for elements
    ui->eatButton->setFont(pixelFont);
    ui->yellButton->setFont(pixelFont);
    ui->runAwayButton->setFont(pixelFont);
    ui->backAwayButton->setFont(pixelFont);
    ui->playDeadButton->setFont(pixelFont);
    ui->intimidateButton->setFont(pixelFont);
    ui->healthBarLabel->setFont(pixelFont);
    ui->bestiaryButton->setFont(pixelFont);
    ui->bestiaryButton->setEnabled(isPractice);
    ui->logLabel->setFont(pixelFont);
    ui->hintButton->setFont(pixelFont);
    // slightly smaller size
    pixelFont = QFont("apple kid", 23, QFont::Normal);
    ui->defeatedLabel->setFont(pixelFont);

    // CONNECTIONS:
    QObject::connect(ui->hintButton, &QPushButton::clicked, this, &RPGWindow::on_tipsButton_pressed);
    QObject::connect(ui->eatButton, &QPushButton::clicked, this, &RPGWindow::eatButtonClicked);
    QObject::connect(ui->yellButton, &QPushButton::clicked, this, &RPGWindow::yellButtonClicked);
    QObject::connect(ui->runAwayButton, &QPushButton::clicked, this, &RPGWindow::runAwayButtonClicked);
    QObject::connect(ui->backAwayButton, &QPushButton::clicked, this, &RPGWindow::backAwayButtonClicked);
    QObject::connect(ui->playDeadButton, &QPushButton::clicked, this, &RPGWindow::playDeadButtonClicked);
    QObject::connect(ui->intimidateButton, &QPushButton::clicked, this, &RPGWindow::intimidateButtonClicked);
    // RPG to rpgwindow
    QObject::connect(this, &RPGWindow::doTurn, rpg, &RPG::doTurn);
    // output
    QObject::connect(rpg, &RPG::showOutput, this, &RPGWindow::printToLog);
    // consequences
    QObject::connect(rpg, &RPG::updateHealthDisplay, this, &RPGWindow::updateHealthDisplay);
    QObject::connect(rpg, &RPG::endBattle, this, &RPGWindow::endBattle);
    QObject::connect(rpg, &RPG::endTurn, this, &RPGWindow::endTurn);
    // health info
    QObject::connect(this, &RPGWindow::getMaxHealth, rpg, &RPG::getMaxHealth);
    QObject::connect(rpg, &RPG::returnMaxHealth, this, &RPGWindow::initializeHealthInfo);
    // starting battles
    QObject::connect(rpg, &RPG::startBattle, this, &RPGWindow::startBattle);
    // starting rpg
    QObject::connect(this, &RPGWindow::startRPG, rpg, &RPG::generateNextBattle);
    // for practice mode, clicking bestiarywindow
    QObject::connect(ui->bestiaryButton, &QPushButton::clicked, this, &RPGWindow::openBestiaryWindow);
    // tips
    QObject::connect(ui->hintButton, &QPushButton::clicked, rpg, &RPG::generateTip);
    // controls
    QObject::connect(rpg, &RPG::enableControls, this, &RPGWindow::prepareEnableControls);
    // animation
    QObject::connect(model, &Model::updateEnemyLocation, this, &RPGWindow::updateEnemyLocation);

    // set initial values:
    isAwaitingLog=false;
    ui->healthBar->setStyleSheet("QProgressBar::chunk {background: QLinearGradient(stop: 0 #42E37A);}");
    setWindowTitle("Time for an adventure in the Utah wilderness!");
    //initialize health info
    emit getMaxHealth();
    //begin first battle by calling generate
    setEnabledControls(false);
    emit startRPG();
    backgroundTimer = new QTimer(this);
    connect(backgroundTimer, SIGNAL(timeout()), this,SLOT(doAnimation()));
    backgroundTimer->start(70);
    enemiesDefeated=0;

    // enable/disable visibility of certain buttons that provide help to the user based on
    // if practice is enabled.
    ui->bestiaryButton->setVisible(isPractice);
    ui->hintButton->setVisible(isPractice);
    ui->hintBackgroundLabel->setVisible(isPractice);
    ui->healthBar->setVisible(!isPractice);
    ui->healthBarLabel->setVisible(!isPractice);
}

void RPGWindow::doAnimation() {
    QPixmap pixmap;
    if(isBackgroundEmpty){
        pixmap = QPixmap(1422,800);
        pixmap.fill(QColor(0,0,0));
    }
    else{
        pixmap = QPixmap::fromImage(backgrounds.at(backgroundIndex)->getNextFrame().scaled(1422,800,Qt::KeepAspectRatio,Qt::FastTransformation));
    }
    ui->backgroundLabel->setPixmap(pixmap);
}

void RPGWindow::endTurn() {
    printToLog("What do you do?");
    prepareEnableControls();
}

void RPGWindow::prepareEnableControls() {
    if(textTimer&&textTimer->isActive()){
        isAwaitingLog=true;
    }
    else{
        QTimer::singleShot(200, this, SLOT(enableControls()));
    }
}

void RPGWindow::initializeHealthInfo(int maxRPGHealth) {
    displayHealth = maxRPGHealth;
    maxHealth = maxRPGHealth;
    ui->healthBar->setMinimum(0);
    ui->healthBar->setMaximum(maxHealth);
    ui->healthBar->setValue(maxHealth);
    ui->healthBarLabel->setText(QString::number(displayHealth) + "/" + QString::number(maxHealth));
}

void RPGWindow::openBestiaryWindow() {
    bestiaryWindow->show();
}

void RPGWindow::closeEvent(QCloseEvent* event) {
    parent_->show();
    close();
}

RPGWindow::~RPGWindow() {
    delete ui;
    delete parent_;
    delete textTimer;
    delete soundTimer;
    delete backgroundTimer;
}

void RPGWindow::printToLog(QString text) {
    if(textTimer->remainingTime()<0){
        lines.push_back("~ ");
        remainingText = text;
        textTimer = new QTimer(this);
        //update text on a timer
        connect(textTimer, SIGNAL(timeout()), this,SLOT(updateLog()));
        textTimer->start(20);
    }
    else{
        //append to outputting text
        remainingText += "\n" + text;
    }
}

void RPGWindow::updateLog() {
    QString logText;
    //ensure lines have not maxed out
    int maxLineLength = 53;
    unsigned int maxLogLines = 7    ;
    if(lines.size() - 1 >= int(maxLogLines)){
        lines.pop_front();
    }
    //display the stored lines on the label
    for(QString line : lines){
        logText+=line+"\n";
    }
    ui->logLabel->setText(logText);
    if(remainingText.length()<=0){
        //done updating text
        textTimer->stop();
        if(isAwaitingLog){
            isAwaitingLog=false;
            QTimer::singleShot(200, this, SLOT(enableControls()));
        }
        return;
    }
    //text remains
    else{
        //save vars for easy access
        int lineLength = lines.back().length();
        //skip leading SPACES in new line
        if(lineLength==0){
            while(remainingText[0]==' '){
                remainingText = remainingText.right(remainingText.length()-1);
            }
        }
        //create a new "bullet" from the newline character so continual output can proceed
        if(remainingText[0]=='\n'){
            lines.push_back("~ ");
            if(lines.size() - 1 >= int(maxLogLines)){
                lines.pop_front();
            }
            //skip the '\n' character as it was already pushed into storage in a new line
            remainingText = remainingText.right(remainingText.length()-1);
            return;
        }
        //the char is SPACE, and the line length > 0 so determine how to push chars
        if(remainingText[0]==' '){
            //find the length of the next word (space to space exclusive)
            int length;
            for(length=0;(length+1<remainingText.length() && (remainingText[length+1]!=' '&&remainingText[length+1]!='\n'));length++);
            //if the word will make the line spill out of the text box, line is over
            if(lineLength+length>=maxLineLength){
                lines.push_back("    ");
                //skip the SPACE so that new lines are not indented randomly
                remainingText = remainingText.right(remainingText.length()-1);
                return;
            }
            //SPACE is not at the front or end, print it
            lines.back()+=" ";
            remainingText = remainingText.right(remainingText.length()-1);
            return;
        }
        //the char is not a SPACE, so determine what to do in case of punctuation
        else{
            QCharRef currentChar = remainingText[0];
            //the word reaches the end of the line
            if(lineLength>=maxLineLength){
                //the next character is not space
                QCharRef nextChar = remainingText[1];
                if(nextChar!=' '){
                    //allow punctuation to go over the end of the line, since punctuation is small
                    if(nextChar.isLetterOrNumber()){
                        lines.back()+=currentChar;
                        remainingText = remainingText.right(remainingText.length()-1);
                        return;
                    }
                    //char is letter or number, add '-' to signify line is continued
                    lines.back()+="-";
                }
                //spacing for the newline
                lines.push_back("    ");
            }
            //add the next char to the back line
            lines.back()+=currentChar;
            //update remaining text
            remainingText = remainingText.right(remainingText.length()-1);
        }
    }
}

void RPGWindow::playSound(const QSound* sound) {
    // easiest way to multithread the sound playing to reduce lag impact
    // as much as possible :)
    soundTimer->singleShot(1, sound, SLOT(play()));
}

void RPGWindow::updateHealthDisplay(int health, bool isEating) {
    //dont want current health to display as -1, protect against that
    int healthChange = health-displayHealth;
    displayHealth=std::max(0,health);
    //update display of player health (bar value)
    ui->healthBar->setValue(displayHealth);
    //update color with health percentage
    double healthPercentage = (double(displayHealth))/(double(maxHealth));
    if(healthPercentage<=.25){
        ui->healthBar->setStyleSheet("QProgressBar::chunk {background: QLinearGradient(stop: 0 #E34742);}");
    }
    else if(healthPercentage<=.50){
         ui->healthBar->setStyleSheet("QProgressBar::chunk {background: QLinearGradient(stop: 0 #E6D243);}");
    }
    else{
        ui->healthBar->setStyleSheet("QProgressBar::chunk {background: QLinearGradient(stop: 0 #42E37A);}");
    }
    repaint();
    //update display of player health (text value)
    ui->healthBarLabel->setText(QString::number(displayHealth) + "/" + QString::number(maxHealth));
    //output change to log to inform the player
    if(healthChange>0){
        //healing
        printToLog("You were healed for " + QString::number(healthChange) + " HP.");
        playSound(heal);
        if(health==maxHealth){
            printToLog("Your HP was maxed out!");
        }
    }
    else if(healthChange<0){
        //damage
        printToLog("You took " +  QString::number(std::abs(healthChange)) + " HP of damage.");
        playSound(hurt);
    }
    //health change is 0 - failsafe, should not happen
    else{
        printToLog("Yet nothing happened...");
    }
    //the player has died
    if(health<=0){
        printToLog("You died...");
        QTimer::singleShot(2300, this, SLOT(showEndGameDialog()));
        return;
    }
    //if the enemy was eaten
    else if(isEating){
        printToLog("The enemy is no more.");
        endBattle();
    }
    //turn continues
    else{
        endTurn();
    }
}

void RPGWindow::endBattle() {
    playSound(run);
    ui->enemyLabel->setPixmap(QPixmap());
    //set the background to black to signify change between battles
    isBackgroundEmpty=true;
    //print escape text
    printToLog(selectRandomWord(excitementWords)+"! You escaped sucessfully. \nYou continue wandering...");
    //begin the next battle, call to generate enemy, do this on a delay to buffer between
    QTimer::singleShot(bufferTime*2, rpg, SLOT(generateNextBattle()));
    enemiesDefeated++;
    ui->defeatedLabel->setText("Enemies Defeated: "+QString::number(enemiesDefeated));
}

void RPGWindow::updateEnemyLocation(double  x, double y) {
    //adjust label from starting position (hardcoded) to new coords
    ui->enemyLabel->move(int(950 + (10*x)),int(180 + (10*y)));
}

void RPGWindow::showEndGameDialog() {
    QDialog endGameDialog(this);
    endGameDialog.setWindowTitle("GAME OVER");
    QFormLayout form(&endGameDialog);
    endGameDialog.setStyleSheet("background-color: rgb(0,0,0); color: rgb(255, 255, 255)");

    // FORM SETUP:
    // row 1: game over image label
    QLabel* imgLabel = new QLabel();
    QPixmap gameOverImg(":/Assets/Graphics/MiscButtons/gameover.png");
    imgLabel->setPixmap(gameOverImg.scaled(322, 261));
    imgLabel->setAlignment(Qt::AlignCenter);
    form.addRow(imgLabel);

    // row 2: introduce options w/ text label
    QLabel* textLabel = new QLabel("You hit zero HP!\nGame over...\n\n"
                                   "You encountered a total\n"
                                   "of " + QString::number(enemiesDefeated) + " organisms in the\n"
                                   "Utah Wilderness.\n\n"
                                   "Thanks for playing!\nWould you like to play again?\n");
    std::cout << enemiesDefeated << std::endl;
    textLabel->setAlignment(Qt::AlignCenter);
    QFont pixelFont("apple kid", 28, QFont::Normal);
    textLabel->setFont(pixelFont);
    form.addRow(textLabel);

    // rows 3 & 4: buttons for receiving choice made
    QPushButton* yesButton = new QPushButton(&endGameDialog);
    yesButton->setFont(pixelFont);
    yesButton->setText("Yes!");
    yesButton->setStyleSheet("border:1px solid white;");
    QPushButton* noButton = new QPushButton(&endGameDialog);
    noButton->setFont(pixelFont);
    noButton->setText("No!");
    noButton->setStyleSheet("border:1px solid white;");
    form.addRow(yesButton);
    form.addRow(noButton);
    // end FORM SETUP

    QObject::connect(yesButton, SIGNAL(clicked()), &endGameDialog, SLOT(accept()));
    QObject::connect(noButton, SIGNAL(clicked()), &endGameDialog, SLOT(reject()));

    if (endGameDialog.exec() == QDialog::Accepted) { // open a new RPG window
        std::vector<Wildlife> newRPGBestiary;
        RPGWindow* advWindow = new RPGWindow(parent_, rpg, false, model);
        advWindow->show();
        endGameDialog.close();
        this->close();
        parent_->hide();
    }
    else { // just close the window
        endGameDialog.close();
        this->close();
    }

    // clean up
    delete imgLabel;
    delete textLabel;
    delete yesButton;
    delete noButton;
}

QString RPGWindow::selectRandomWord(std::vector<QString> options){
     int idx = int(std::rand() % long(options.size()));
     return options.at(unsigned(long(idx)));
}

void RPGWindow::startBattle(QString enemyName){
    //make background not black
    isBackgroundEmpty=false;
    //shift the background Index, so that for the next fight the next background will be loaded
    if(backgroundIndex<backgrounds.size()-1){
        backgroundIndex++;
    }
    else{
        backgroundIndex=0;
    }
//          play enemyName.wav, if it is a plant play default.wav (if the name is none of the ones found in sound effects, wildlife
//          but the space needs to be removed and the first of the two words in each name needs to be de - capitalized
    // update enemy display
    QIcon icon(":/Assets/Graphics/Wildlife/" + enemyName + ".png");
    QPixmap pixmap = icon.pixmap(QSize(ui->enemyLabel->width(), ui->enemyLabel->height()));
    ui->enemyLabel->setPixmap(pixmap);

    //add a random word to the entry text to spice things up
    printToLog("A " + enemyName + " " + selectRandomWord(entryWords) + " in.");

    //re-enable controls
    endTurn();
}

void RPGWindow::enableControls(){
    setEnabledControls(true);
}

void RPGWindow::setEnabledControls(bool status){
    //disable screen buttons
    ui->eatButton->setEnabled(status);
    ui->yellButton->setEnabled(status);
    ui->runAwayButton->setEnabled(status);
    ui->backAwayButton->setEnabled(status);
    ui->intimidateButton->setEnabled(status);
    ui->playDeadButton->setEnabled(status);
    ui->hintButton->setEnabled(status);
    //this consequently disables keyboard shortcuts because of our implementation
}

void RPGWindow::eatButtonClicked(){
    setEnabledControls(false);
    emit doTurn('E');
}

void RPGWindow::yellButtonClicked(){
    setEnabledControls(false);
    playSound(yell);
    emit doTurn('Y');
}

void RPGWindow::runAwayButtonClicked(){
    setEnabledControls(false);
    emit doTurn('R');
}

void RPGWindow::backAwayButtonClicked(){
    setEnabledControls(false);
    emit doTurn('B');
}

void RPGWindow::playDeadButtonClicked(){
    setEnabledControls(false);
    emit doTurn('P');
}

void RPGWindow::intimidateButtonClicked(){
    setEnabledControls(false);
    emit doTurn('I');
}

void RPGWindow::setPressed(QPushButton * button, bool pressed){
    //switch between button.png and button_P.png (P for pressed)
    QString path = ":/Assets/Graphics/ActionButtons/"+ button->objectName();
    if(pressed){
        path+="_P";
    }
    button->setIcon(QIcon(path));
    repaint();
}

void RPGWindow::on_backAwayButton_released(){
    setPressed(ui->backAwayButton,false);
}

void RPGWindow::on_runAwayButton_released(){
     setPressed(ui->runAwayButton,false);
}

void RPGWindow::on_playDeadButton_released(){
     setPressed(ui->playDeadButton,false);
}

void RPGWindow::on_eatButton_released(){
     setPressed(ui->eatButton,false);
}

void RPGWindow::on_intimidateButton_released(){
     setPressed(ui->intimidateButton,false);
}

void RPGWindow::on_yellButton_released(){
     setPressed(ui->yellButton,false);
}

void RPGWindow::on_runAwayButton_pressed(){
    setPressed(ui->runAwayButton,true);
}

void RPGWindow::on_backAwayButton_pressed(){
    setPressed(ui->backAwayButton,true);
}

void RPGWindow::on_playDeadButton_pressed(){
    setPressed(ui->playDeadButton,true);
}

void RPGWindow::on_yellButton_pressed(){
    setPressed(ui->yellButton,true);
}

void RPGWindow::on_intimidateButton_pressed(){
    setPressed(ui->intimidateButton,true);
}

void RPGWindow::on_eatButton_pressed(){
    setPressed(ui->eatButton,true);
}

void RPGWindow::on_tipsButton_pressed(){
    setPressed(ui->hintButton,true);
}

void RPGWindow::on_tipsButton_released(){
    setPressed(ui->hintButton, false);
}

void RPGWindow::on_tipsButton_clicked(){
    setEnabledControls(false);
}
