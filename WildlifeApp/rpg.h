/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#ifndef RPG_H
#define RPG_H
#include <queue>
#include "wildlife.h"
#include <map>

/**
 * rpg is created in the main model. model for rpgview speaks to the rpg view
 */
class RPG : public QObject {

Q_OBJECT

public:
    /**
     * RPG contructor
     */
    RPG(int, std::vector<Wildlife>, bool);

    /**
      * A copy constructor for RPG
     */
    RPG(const RPG&);

    /**
     * Overrides '=' for RPG
     */
    RPG& operator=(RPG);

    /**
     * player health
     */
    int health;

    /**
     * max player health
     */
    int maxHealth;

    /**
     * number of enemies defeated
     */
    int enemiesDefeated;

    /**
     * the current enemy being battled
     */
    Wildlife * currentEnemy;

    /**
     * bool if the rpg is practice mode
     */
    bool isPracticeMode;

    /**
     * queue of enemies to pick from
     */
    std::vector<Wildlife> bestiary;

public slots:
    /**
     * Takes the turn of the player
     */
    void doTurn(char);

    /**
     * Initiates the next battle
     */
    void generateNextBattle();

    /**
     * retrieves singally info about player max health
     * this could be done with a method instead of symbols
     * but that is bad code style
     */
    void getMaxHealth();

    /**
     * sets up the initial map of encounters
     */
    void generateEncounterList();

    /**
     * generates a tip
     */
    void generateTip();

signals:
    /**
     * sends singal to prepare enable controls
     */
    void enableControls();

    /**
     * signal to show the ending sequence
     */
    void showVictorySequence();

    /**
     * signal sent with the current creature name and number of completed battles
     * signal to update the creature display
     */
    void startBattle(QString);

    /**
     * singal to show dialog
     */
    void showOutput(QString);

    /**
     * signal to end the battle in frontend
     */
    void endBattle();

    /**
     *  signal to update frontend health
     */
    void updateHealthDisplay(int,bool);

    /**
     * sends max health to view for initialization
     */
    void returnMaxHealth(int);

    /**
     *  enables control
     */
    void endTurn();

private:
    /**
     * updates the internal health of the player based on current encounter
     */
    void updateHealth();

    /**
     * Saves space for the encounter list so it does
     * not need to be constantly reallocated
     */
    std::map<std::string,int> encounterTable;

    /**
     * Given a string name of the creature, plant, or fungus, lookup the encounter in the bestiary.
     */
    Wildlife* lookupEncounter(std::string);
};

#endif // RPG_H
