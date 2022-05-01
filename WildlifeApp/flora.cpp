/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#include "flora.h"

Flora::Flora(QString _name,  QString _description, int _healthMod)
    : Wildlife(_name, _description, _healthMod, QVector<char>{'E'}, QVector<char>{'R','B','E'}) {
    //set standard reactions
    reactions.insert('R',"You run away from the "+name+"...");
    reactions.insert('P',"You play dead. The "+name+" cleverly sees through your ruse.");
    reactions.insert('Y',"You yell at the "+name+". It remains unsurprised by your screaming.");
    reactions.insert('I',"You glare menacingly at the "+name+". It boldly ignores you.");
    reactions.insert('E',"You eat the "+name+"... ");
    reactions.insert('B',"You back away from the "+name+"...");
}

