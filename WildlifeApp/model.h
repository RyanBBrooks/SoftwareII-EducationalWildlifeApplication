/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#ifndef MODEL_H
#define MODEL_H

#include "wildlife.h"
#include "flora.h"
#include "fauna.h"
#include "Box2D/Box2D.h"
#include <queue>

class Model : public QObject {

    Q_OBJECT

public:
    /**
     * zero-param constructor
     */
    Model();

    /**
     * copy constructor
     */
    Model(const Model&);

    /**
     * '=' override
     */
    Model& operator=(Model);

    /**
     *  cleans up resources used by the class.
     */
    ~Model();

    /**
     * underlying list of creatures used by multiple other classes and windows.
     *
     * organized by type (plant, animal, fungus) as well as alphabetically within
     * type restrictions.
     */
    std::vector<Wildlife>* bestiary;

public slots:
    /**
     * updates the position of an object in the physics world.
     */
    void updatePositionWorld();

    /**
     * called to get the positon of the object
     */
    void givePos();

    /**
     * called to retrieve the bestiary
     */
    void getBestiary(bool);

signals:
    /**
     * emitted on update of x coordinate of object in world
     */
    void updatedX(int);

    /**
     * emitted on update of y coordinate of object in world
     */
    void updatedY(int);

    /**
     * emitted on bestiary retrieval -- sends the bestiary to whatever called it.
     */
    void retrievedBestiary(std::vector<Wildlife>,bool);

    /**
     * emitted to update the enemy's location on the screen as it bounces around. used
     * to animate the enemy labels and give the program some life.
     */
    void updateEnemyLocation(double, double);

private:
    /**
     * builds the bestiary. should only ever be called once. adds every creature to the
     * bestiary then returns.
     */
    void buildBestiary();

    /**
     * The world used by the physics simulator to get positioning for image animation
     */
    b2World positionWorld;

    /**
     * The body bouncing around the physics world
     */
    b2Body* bouncingBody;

    /**
     * The x-position of the bouncingBody, adjusted to a [-14,14] scale
     */
    double xBouncePos;

    /**
     * The y-position of the bouncingBody, on a [-14,14] scale
     */
    double yBouncePos;
};

#endif // MODEL_H
