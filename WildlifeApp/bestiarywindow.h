/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#ifndef BESTIARYWINDOW_H
#define BESTIARYWINDOW_H

#include <QWidget>
#include "wildlife.h"
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include "model.h"
#include <QScrollArea>
#include "backgroundanimation.h"

namespace Ui { class bestiaryWindow; }

/**
 * @brief The BestiaryWindow class
 *
 * builds and runs the bestiary window as a QWidget. to be used by the user like a glossary
 * of animals, plants, and fungi for learning about Utah wildlife.
 */
class BestiaryWindow : public QWidget {
    Q_OBJECT

public:
    explicit BestiaryWindow(QMainWindow*, Model*,bool);
    ~BestiaryWindow();

public slots:
    /**
     * called when the next arrow button is clicked to display the next creature.
     */
    void nextClicked();

    /**
     * called when the next arrow button is clicked to display the previous creature.
     */
    void prevClicked();

    // BUTTON CLICKS:
    // used to handle what should happen when specific buttons are clicked.
    void jumpToAnimalsClicked();
    void jumpToPlantsClicked();
    void jumpToFungiClicked();
    // end button clicks

    /**
     * loads a .png image into the image label in the window and changes the name label
     * to reflect the change in creature currently being encountered.
     */
    void updateGUI();

    /**
     * overridden method which is essential to the smooth transition from window to window.
     * in this case, opens the parent window before closing this one.
     */
    void closeEvent(QCloseEvent* event = nullptr);

    /**
     * returns the user to the main title screen and closes the bestiary.
     */
    void backToMainWindowClicked();

    /**
     * performs the background animation transition.
     */
    void doBackgroundAnimation();


private slots:
    // handle button clicks to navigate the bestiary
    void on_prevButton_pressed();
    void on_prevButton_released();
    void on_nextButton_pressed();
    void on_nextButton_released();

    /**
     * recieved from the model to update the position of the picture label. used to
     * animate a bouncing motion using Box2D
     */
    void updateEnemyPosition(double x, double y);

private:
    /**
     * the underlying data contained in the bestiary. holds every one of the animals
     * and plants, etc.
     */
    Model * model;

    /**
     * makes a button look visually pressed
     */
    void setPressed(QPushButton * button, bool pressed);

    /**
     * a container for the window which created this window. used to bypass
     * the default of creating a new QWidget(parent) in the constructor, which
     * causes all related windows to be shown and hidden simultaneously.
     */
    QMainWindow* parent_;

    /**
     * the main elements of the GUI that the user is able to see and interact with.
     */
    Ui::bestiaryWindow* ui;
    bool belongsToPracticeWindow;

    /**
     * sets the font of the incoming object to our custom font called Pixeled,
     * which can be found in our Q Resources file.
     */
    void setPixelFont(QPushButton*, int);

    /**
     * sets the font of the incoming object to our custom font called Pixeled,
     * which can be found in our Q Resources file.
     */
    void setPixelFont(QLabel*, int);

    /**
     * sets the font of the incoming object to our custom font called Pixeled,
     * which can be found in our Q Resources file.
     */
    void setPixelFont(QScrollArea*, int);

    /**
     * advances the internal schematics to the next creature in the creaturesList,
     * then updates the GUI to reflect the new creature's information.
     */
    void goToNextCreature();

    /**
     * reverts the internal schematics to the previous creature in the creaturesList,
     * then updates the GUI to reflect the new creature's information.
     */
    void goToPrevCreature();

    /**
     * used to add each of the unique creatures to the creaturesList.
     */
    void setUpCreatures();

    /**
     * used to save the indeces of specific marker animals. for example, the Big Horn Sheep
     * is the first of all of the animals, so its index should be saved so that the user can
     * jump around between different topics taught by the bestiary.
     */
    void setUpIndeces();

    /**
     * a container for all of the creatures we'd like to teach people about.
     */
    std::vector<Wildlife>* creaturesList;

    /**
     * a timer used for updating the location. calls moveCreatureIcon to give the
     * creature a new location.
     */
    QTimer* animationTimer;

    /**
     * animates the background of the bestiary
     */
    QTimer* backgroundTimer;

    /**
     * bestiary's background
     */
    BackgroundAnimation* background;

    /**
     * index of the current location in the creatureList.
     */
    uint currentIdx;

    /**
     * current creature based on the current index in the creatureList.
     */
    Wildlife* currentCreature;

    /**
     * pointer to the actual description label. saved here instead of the .ui so that we
     * can directly access and adjust it as necessary since it's inside a scroll area.
     */
    QLabel* descriptionLabel;

    // indexes of the first of each topic
    uint plantIndex;
    uint animalIndex;
    uint fungusIndex;
    // end indeces

    // BACKGROUND used by this window for flavor and flare
    BackgroundAnimation bestiaryBG{ QImage(":/Assets/Graphics/Backgrounds/B_B.png"),
        QVector<QColor>{
            QColor(52,230,183),
            QColor(48,144,230)
        }, 2 //animation style
    };
    // end background
};

#endif // BESTIARYWINDOW_H
