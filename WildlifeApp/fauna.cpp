/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#include "fauna.h"

Fauna::Fauna(QString _name,  QString _description, int _healthMod,  QVector<char> _healthActions,
             QVector<char> _escapeActions, QString TextR, QString TextP, QString TextI,
             QString TextY, QString TextB)
    : Wildlife(_name, _description, _healthMod, _healthActions, _escapeActions) {
    //set standard reactions
    reactions.insert('R',TextR); //Run away
    reactions.insert('P',TextP); //Play Dead
    reactions.insert('Y',TextY); //Yell
    reactions.insert('I',TextI); //Intimidate
    reactions.insert('B',TextB); //Backaway
    reactions.insert('E',"Try as you might, you could not eat the "+name+"."); //Eat
}
