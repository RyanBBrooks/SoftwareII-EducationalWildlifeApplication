/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#ifndef PRACTICEWINDOW_H
#define PRACTICEWINDOW_H
#include "rpgwindow.h"
#include <queue>

/**
 * @brief The PracticeWindow class
 * inherits from RPGWindow. does not hide the hint buttons. provides the
 * user with useful tips and does not keep track of health.
 */
class PracticeWindow : public RPGWindow {
public:
    // constructor
    PracticeWindow(QMainWindow*, RPG *,BestiaryWindow*, Model*);
};

#endif // PRACTICEWINDOW_H
