/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#ifndef WILDLIFE_H
#define WILDLIFE_H
#include <string>
#include <QFile>
#include <QImage>
#include <QVector>
#include <QMap>

class Wildlife
{
public:
    Wildlife(QString, QString, int, QVector<char>, QVector<char>);
    /**
     * A copy constructor for Wildlife
     */
    Wildlife(const Wildlife&);
    /**
     * Overrides '=' for Wildlife
     */
    Wildlife& operator=(Wildlife);

    /**
     * returns the reaction associated with a certain char
     */
    QString getReaction(char);

    /**
     * checks if a char is associated with an escape action
     */
    bool isEscapeAction(char);

    /**
     * checks if a char is associated with a health action
     */
    bool isHealthAction(char);

    /**
     * A Brief Description of the wildlife
     */
    QString description;

    /**
     * Added to the players health when interacting with the entity
     */
    int healthMod;

    /**
     * name of this entity, also used to find image file and sound file
     */
    QString name;

    /**
     * list of names associated with the actions for this entity
     */
    QVector<QString> actionNames;

    /**
     * returns a random escape action
     */
    QString getRandomEscapeAction();

protected:
    /**
     * Maps Char Action code integer to response
     */
    QMap<char,QString> reactions;

    /**
     * actions as char references that allow escape from this creature
     */
    QVector<char> escapeActions;

    /**
     * actions as char references that result in healthModification from this creature
     */
    QVector<char> healthActions;

private:


};

#endif // WILDLIFE_H
