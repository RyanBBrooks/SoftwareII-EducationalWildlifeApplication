/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#ifndef FLORA_H
#define FLORA_H
#include "wildlife.h"

/**
 * @brief The Flora class
 *
 * describes a plant object. has a name, description, and health modifier.
 */
class Flora : public Wildlife {
public:
    Flora(QString, QString, int);
};

#endif // FLORA_H
