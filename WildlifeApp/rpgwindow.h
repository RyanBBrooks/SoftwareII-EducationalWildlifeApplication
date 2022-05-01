/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#ifndef RPGWINDOW_H
#define RPGWINDOW_H

#include <QWidget>
#include <QCloseEvent>
#include <QMainWindow>
#include <wildlife.h>
#include <rpg.h>
#include <backgroundanimation.h>
#include <QQueue>
#include <QSound>
#include <QSoundEffect>
#include <QPushButton>
#include "bestiarywindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class rpgwindow; }
QT_END_NAMESPACE

/**
 * the UI for the RPG game. may be in practice mode or not, based on the boolean passed in.
 * builds the UI accordingly.
 */
class RPGWindow : public QWidget {
    Q_OBJECT

public:
    /**
     * constructs the window.
     */
    RPGWindow(QMainWindow*,RPG*,bool, Model*);

    /**
     * destroys resources used by the window.
     */
    ~RPGWindow();

    /**
     * the number of which background is being displayed behind all the buttons.
     */
    int backgroundNumber;

signals:
    /**
     *  signal to get max health
     */
    void getMaxHealth();

    /**
     * signal to start the rpg
     */
    void startRPG();

    /**
     * requests a tip from the rpg model to keep the mvc
     */
    void requestTip();

    /**
     *  does a turn action
     */
    void doTurn(char actionCode);

    /**
     * signal to title that opens the bestiary
     */
    void sendOpenBestiary();

    /**
     * emitted to retrieve the location of the enemy label. used to animate the enemy.
     */
    void getNewEnemyPos();

public slots:
    // BUTTON CLICKS:
    // the following methods are used to detect button clicks and redirect to the proper
    // reaction which should be performed by the program.
    void eatButtonClicked();
    void yellButtonClicked();
    void runAwayButtonClicked();
    void backAwayButtonClicked();
    void playDeadButtonClicked();
    void intimidateButtonClicked();
    // end button clicks

    /**
     * runs the enemy animation
     */
    void doAnimation();

    /**
     * called when a new enemy location is retrieved from the model. changes the location
     * of the enemy's picture label. used in part of the animation process.
     */
    void updateEnemyLocation(double, double);

    /**
     * readies controls to be enabled
     */
    void prepareEnableControls();

    /**
     * When the player's health reaches zero, show a dialog asking what they would like to do next.
     */
    void showEndGameDialog();

    /**
     * returns control to the player after a delay
     * intermediary function, may seem pointless but neseccary because of struture and
     * use of variables in certain signals
     */
    void endTurn();

    /**
     * performs protocol for ending the battle on the display end
     */
    void endBattle();

    /**
     * outputs a piece of text to the log.
     * !!! important !!! - send this as raw text, new lines will be added automatically
     */
    void printToLog(QString);

    /**
     * performs protocol for beginning the battle on the display side
     */
    void startBattle(QString);

    /**
     * intitalizes rpgwindow's knowledge about player health and maxHealth
     */
    void initializeHealthInfo(int);

    /**
     * helper method for printing to log recursively
     */
    void updateLog();

    /**
     * plays the sound at a certain file path
     */
    void playSound(const QSound*);

    /**
     * wrapper for qtimer arg passing for setEnabledControls
     */
    void enableControls();

    /**
     * opens the bestiary window for reference when the player needs it in practice mode
     */
    void openBestiaryWindow();


private slots:
    // BUTTON UPDATES:
    // the following methods are used to detect when buttons are pressed and un-pressed
    // in order to provide the user with a visual update to the interface to make it
    // obvious that a button is in fact being pressed.
    void on_backAwayButton_released();
    void on_runAwayButton_released();
    void on_playDeadButton_released();
    void on_eatButton_released();
    void on_intimidateButton_released();
    void on_yellButton_released();
    void on_runAwayButton_pressed();
    void on_backAwayButton_pressed();
    void on_playDeadButton_pressed();
    void on_yellButton_pressed();
    void on_intimidateButton_pressed();
    void on_eatButton_pressed();
    void on_tipsButton_pressed();
    void on_tipsButton_released();
    void on_tipsButton_clicked();
    // end button updates

protected:
    /**
     * used in practice mode rpg game, if the user opens the bestiary button
     */
    BestiaryWindow* bestiaryWindow;

private:
    /**
     * the main interface/UI of this window.
     */
    Ui::rpgwindow* ui;

    /**
     * the underlying methods and information necessary to run the game.
     */
    Model* model;

    /**
     * Sounds used by the UI
     */
    const QSound* hurt = new QSound(":/Assets/Sound Effects/Action/hurt.wav");
    const QSound* heal = new QSound(":/Assets/Sound Effects/Action/heal.wav");
    const QSound* text = new QSound(":/Assets/Sound Effects/Action/text.wav");
    const QSound* yell = new QSound(":/Assets/Sound Effects/Action/yell.wav");
    const QSound* run = new QSound(":/Assets/Sound Effects/Action/run.wav");

    /**
     * is this Practice
     */
    bool isPractice;

    /**
     * is the game awaiting log to end turn
     */
    bool isAwaitingLog;

    /**
     * retrieves a random word from the input vector of strings
     */
    QString selectRandomWord(std::vector<QString>);

    /**
     * an array of words to pick from to describe an enemy's entry into the battle
     */
    std::vector<QString> entryWords{"saunters","shuffles","bumbles","stumbles","marches",
                                    "dashes","darts","trots","scurries","scampers",
                                    "scrambles","charges","boogies"};

    /**
     * an array of words to pick from to describe an enemy's entry into the battle
     */
    std::vector<QString> excitementWords{"Tubular","Wow","Radical","Excellent","Gnarly",
                                         "Cool","Nice","Tight","Righteous","Sweet","Far out"};

    /**
     * changes the graphics of a button
     */
    void setPressed(QPushButton *,bool);

    /**
     * timer for playing sounds
     */
    QTimer* soundTimer;

    /**
     * standard time to wait between events
     */
    int bufferTime = 2700;

    /**
     *  Bool weather background should be just black
     */
    bool isBackgroundEmpty;

    /**
     * animates the background of the encounters
     */
    QTimer* backgroundTimer;

    /**
     * a tool used to place text one letter at a time on the screen
     */
    QTimer* textTimer;

    /**
     * x.setEnabled(bool) for all buttons
     * disables selection with keyboard controls -- UNIMPLEMENTED
     */
    void setEnabledControls(bool);

    /**
     * current text stored in the log
     */
    QQueue<QString> lines;

    /**
     * keeps track of the remaining text which will be printed out by the
     * printToLog method.
     */
    QString remainingText;

    /**
     * a container for the window which created this window. used to bypass
     * the default of creating a new QWidget(parent) in the constructor, which
     * causes all related windows to be shown and hidden simultaneously.
     */
    QMainWindow* parent_;

    /**
     * Overloaded method which opens the previous window before closing the current one.
     */
    void closeEvent(QCloseEvent* event = nullptr);

    /**
     * change health display and output health change to text log
     */
    void updateHealthDisplay(int,bool);

    /**
     * the current health displayed by rpgView
     */
    int displayHealth;

    /**
     * number of enemies escaped
     */
    int enemiesDefeated;

    /**
     * max health displayed by rpgView
     */
    int maxHealth = 0;

    /**
     * index of current background animation
     */
    int backgroundIndex = 0;

    /**
     * rpg backing rpgView
     */
    RPG * rpg;

    // BACKGROUNDS USED BY THIS WINDOW
    //Background 0
    BackgroundAnimation background0{ QImage(":/Assets/Graphics/Backgrounds/B_0.png"),
        QVector<QColor>{
                    QColor(0, 0, 0),
                    QColor(116, 214, 147),
                    QColor(168, 185, 66),
                    QColor(255, 70, 70),
        }, 14 //animation style
    };

    //Background 1
    BackgroundAnimation background1{ QImage(":/Assets/Graphics/Backgrounds/B_1.png"),
        QVector<QColor>{
            //padding
            QColor(250, 250, 110), QColor(250, 250, 110), QColor(250, 250, 110),
            //switch colors
            QColor(189, 234, 117), QColor(161, 225, 123),
            QColor(108, 204, 134), QColor(84, 193, 138),
            QColor(35, 170, 143), QColor(4, 157, 143),
            QColor(0, 133, 137), QColor(0, 120, 130),
            //padding
            QColor(31, 95, 112), QColor(31, 95, 112), QColor(31, 95, 112),
            //reverse colors
            QColor(0, 120, 130), QColor(0, 133, 137),
            QColor(4, 157, 143), QColor(35, 170, 143),
            QColor(84, 193, 138), QColor(108, 204, 134),
            QColor(161, 225, 123), QColor(189, 234, 117),
        }, 15 //animation style
    };

    //Background 2
    BackgroundAnimation background2{ QImage(":/Assets/Graphics/Backgrounds/B_2.png"),
        QVector<QColor>{
            //padding
            QColor(240, 44, 96), QColor(240, 44, 96),
            //switch colors
            QColor(247, 71, 81), QColor(250, 96, 68), QColor(249, 119, 56),
            QColor(245, 141, 47), QColor(238, 162, 44),
            //padding
            QColor(229, 182, 50), QColor(229, 182, 50),
            //reverse colors
            QColor(238, 162, 44), QColor(245, 141, 47), QColor(249, 119, 56),
            QColor(250, 96, 68), QColor(247, 71, 81)
        }, 5 //animation style
    };

    //Background 3
    BackgroundAnimation background3{ QImage(":/Assets/Graphics/Backgrounds/B_3.png"),
        QVector<QColor>{
            //padding
            QColor(29, 34, 65), QColor(29, 34, 65), QColor(29, 34, 65), QColor(29, 34, 65),
            //switch colors
            QColor(82, 58, 88), QColor(129, 88, 107), QColor(170, 122, 125),
            QColor(204, 162, 147), QColor(255, 250, 216),
            QColor(170, 122, 125), QColor(82, 58, 88)
        }, 3 //animation style
    };

    //Background 4
    BackgroundAnimation background4{ QImage(":/Assets/Graphics/Backgrounds/B_4.png"),
        QVector<QColor>{
            //padding
            QColor(149, 247, 220), QColor(130, 216, 183), QColor(111, 186, 149),
            QColor(94, 156, 116), QColor(76, 127, 85), QColor(60, 99, 56),
            QColor(43, 73, 29), QColor(61, 92, 35), QColor(81, 111, 40),
            QColor(104, 131, 44), QColor(129, 150, 48), QColor(156, 170, 51),
            QColor(185, 189, 55), QColor(172, 202, 84), QColor(159, 213, 113),
            QColor(150, 223, 142), QColor(144, 232, 170), QColor(143, 240, 196),
            //switch colors

        }, 5 //animation style
    };

    //Background 5
    BackgroundAnimation background5{ QImage(":/Assets/Graphics/Backgrounds/B_5.png"),
        QVector<QColor>{
            QColor(168, 123, 233),
            QColor(162, 146, 244), QColor(160, 167, 251), QColor(165, 186, 255),
            QColor(176, 203, 255), QColor(193, 220, 255), QColor(216, 235, 255),
            QColor(189, 239, 255), QColor(160, 244, 254), QColor(144, 247, 233),
            QColor(153, 247, 199), QColor(183, 243, 159), QColor(224, 233, 123),
            QColor(255, 204, 103), QColor(255, 172, 110), QColor(255, 142, 135),
            QColor(255, 121, 169), QColor(233, 116, 205)
        }, 13 //animation style
    };

    //Background 6
    BackgroundAnimation background6{ QImage(":/Assets/Graphics/Backgrounds/B_6.png"),
        QVector<QColor>{
            QColor(252, 184, 19), QColor(255, 171, 49), QColor(255, 159, 74),
            QColor(255, 147, 99), QColor(255, 138, 124), QColor(255, 132, 149),
            QColor(255, 130, 174), QColor(255, 133, 198), QColor(255, 138, 220),
            QColor(255, 146, 239), QColor(215, 149, 244), QColor(173, 151, 242),
            QColor(133, 150, 232), QColor(97, 147, 216), QColor(69, 142, 196),
            QColor(53, 135, 173), QColor(52, 126, 149), QColor(59, 117, 127),
            QColor(67, 106, 108), QColor(57, 120, 115), QColor(53, 133, 115),
            QColor(60, 145, 110), QColor(78, 157, 99), QColor(104, 167, 85),
            QColor(135, 175, 67), QColor(171, 181, 47), QColor(210, 184, 27)

        }, 7 //animation style
    };

    //Background 7
    BackgroundAnimation background7{ QImage(":/Assets/Graphics/Backgrounds/B_7.png"),
        QVector<QColor>{
            QColor(149, 247, 220), QColor(130, 216, 183), QColor(111, 186, 149),
            QColor(94, 156, 116), QColor(76, 127, 85), QColor(60, 99, 56),
            QColor(43, 73, 29), QColor(61, 92, 35), QColor(81, 111, 40),
            QColor(104, 131, 44), QColor(129, 150, 48), QColor(156, 170, 51),
            QColor(185, 189, 55), QColor(172, 202, 84), QColor(159, 213, 113),
            QColor(150, 223, 142), QColor(144, 232, 170), QColor(143, 240, 196),
        }, 7 //animation style
    };
    // END BACKGROUNDS

    //vector of available backgrounds
    QVector<BackgroundAnimation*> backgrounds{
        &background0,&background1,&background2,&background3,&background4,&background5,&background6,&background7
    };
};

#endif // RPGWINDOW_H
