/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#include "practicewindow.h"

PracticeWindow::PracticeWindow(QMainWindow* parent,RPG * rpg,BestiaryWindow* BW, Model* model) : RPGWindow(parent , rpg, true, model) {
    setWindowTitle("Practice what you've learned!");
    bestiaryWindow = BW;
}
