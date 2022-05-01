/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#ifndef FAUNA_H
#define FAUNA_H
#include "wildlife.h"

/**
 * @brief The Fauna class
 *
 * describes an animal object for the bestiary -- has a name, description,
 * health modifier, and some valid actions/reactions to an encounter with
 * the creature.
 */
class Fauna : public Wildlife {
public:
    Fauna(QString, QString, int, QVector<char>, QVector<char>, QString, QString, QString, QString, QString);
};

#endif // FAUNA_H
