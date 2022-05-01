/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#ifndef TITLEWINDOW_H
#define TITLEWINDOW_H

#include <QMainWindow>
#include "bestiarywindow.h"
#include "rpgwindow.h"
#include "practicewindow.h"
#include <queue>

QT_BEGIN_NAMESPACE
namespace Ui { class TitleWindow; }
QT_END_NAMESPACE

/**
 * the driving interface the user interacts with to select different ways of using our program. contains
 * several options for learning, practicing, or adventuring through the Utah wilderness. this is a splash
 * window intended to redirect the user to whichever game mode is selected.
 */
class TitleWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * constructor to create the window
     */
    TitleWindow(QWidget *parent = nullptr, Model *backingModel = nullptr);

    /**
     * destructor to clean up resources
     */
    ~TitleWindow();

private:
    /**
     * the main interface through which the user interacts with the program.
     */
    Ui::TitleWindow *ui;

    /**
     * the underlying data which makes up the bestiary. contains the creatures and plants
     * which we teach the user about. also holds data for the 2-dimensional bouncing animation
     * of the picture labels.
     */
    Model* model;

    /**
     * provides the underlying data related to running the RPG game in both adventure mode
     * and practice mode.
     */
    RPG* rpg;

    /**
     * parameter unused. overridden event to make sure that if the main window is somehow closed,
     * the windows it opened are also closed.
     */
    void closeEvent (QCloseEvent *event);

    /**
     * list of "child" windows to the title parent. contains all windows the title window
     * opened as a result of clicking redirectory buttons.
     */
    std::vector<QWidget*>*childWindows;

    /**
     * creates and runs a child window. adds the window to the list of children windows. helper
     * method which makes window creation more succint.
     */
    void makeNewWindow(QWidget*);

    /**
     * creates the necessary objects to run the RPG game and opens the RPG window.
     */
    void initializeRPGWindow(std::vector<Wildlife>,bool);

    /**
     *  sets the image of a button to either on or off
     */
    void setPressed(QPushButton *,bool);

public slots:
    /**
     * called when the LEARN! button is clicked. opens the bestiary window.
     */
    void learnClicked();

    /**
     * called when the PRACTICE! button is clicked. opens the practice window.
     */
    void practiceClicked();

    /**
     * called when the ADVENTURE! button is clicked. opens the RPG window.
     */
    void adventureClicked();

    /**
     * called when the information button is clicked. opens a dialog providing information
     * to the user concerning who we are and why we made this program.
     */
    void infoButtonClicked();

signals:
    /**
     * emitted during creation of the bestiary window. retrieves the bestiary based on
     * whether the user has selected practice mode or not.
     */
    void getBestiary(bool);

private slots:
    // BUTTON UPDATES:
    // the following methods are used to detect when buttons are pressed and un-pressed
    // in order to provide the user with a visual update to the interface to make it
    // obvious that a button is in fact being pressed.
    void on_learnButton_pressed();
    void on_practiceButton_pressed();
    void on_adventureButton_pressed();
    void on_infoButton_pressed();
    void on_learnButton_released();
    void on_practiceButton_released();
    void on_adventureButton_released();
    void on_infoButton_released();
    // end button updates
};

#endif // TITLEWINDOW_H
