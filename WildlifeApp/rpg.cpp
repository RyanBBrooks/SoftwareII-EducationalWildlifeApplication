/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#include "rpg.h"
#include <QTimer>
#include <QRandomGenerator>
#include <iostream>


RPG::RPG(int _health, std::vector<Wildlife> _bestiary, bool _isPracticeMode) {
    maxHealth = _health;
    health = _health;
    bestiary = _bestiary;
    isPracticeMode = _isPracticeMode;
    enemiesDefeated = 0;
    std::srand(std::time(NULL));
}

RPG::RPG(const RPG& other){
    maxHealth = other.maxHealth;
    health = other.health;
    bestiary = other.bestiary;
    isPracticeMode = other.isPracticeMode;
    enemiesDefeated = other.enemiesDefeated;
    currentEnemy = other.currentEnemy;
}

RPG& RPG::operator=(RPG other){
    std::swap(health,other.health);
    std::swap(bestiary,other.bestiary);
    std::swap(isPracticeMode,other.isPracticeMode);
    std::swap(maxHealth, other.maxHealth);
    std::swap(enemiesDefeated, other.enemiesDefeated);
    std::swap(currentEnemy, other.currentEnemy);
    return*this;
}

void RPG::getMaxHealth(){
    emit returnMaxHealth(maxHealth);
}

void RPG::generateTip(){
    emit showOutput("A voice whispers \"Try "+currentEnemy->getRandomEscapeAction()+".\"");
    //this just makes a call to re-enable user control
    emit enableControls();
}

void RPG::doTurn(char actionCode){
    //failsafe, ensures actioncodes are uniformly uppercase
    actionCode = std::toupper(actionCode);
    //immediately update output string and
    emit showOutput(currentEnemy->getReaction(actionCode));

    //Practice
    if(isPracticeMode){
        if(currentEnemy->isEscapeAction(actionCode)){
            if(!currentEnemy->isHealthAction(actionCode)){
                enemiesDefeated++;
                emit endBattle();
                return;
            }
        }
        if(currentEnemy->isHealthAction(actionCode)){
            if(actionCode=='E'){
                //if it is a damaging food
                if(currentEnemy->healthMod<0){
                    //alternative to "narrowly avoid damage"
                    emit showOutput("You instinctively spit it out!");
                }
                else{
                    //alternative to "healing" - ends the battle as it is unessecary of the player chose the correct action
                    emit showOutput("It was delicious");
                    emit showOutput("The enemy is no more.");
                    enemiesDefeated++;
                    emit endBattle();
                    return;
                }
            }
            else{
                //practice mode's "you took damage" but not really
                emit showOutput("You narrowly avoided damage!");
            }

        }
        emit endTurn();
    }

    //adventure mode loop
    else{
        //send specific actions with a delay
        if(currentEnemy->isEscapeAction(actionCode)){
            //for eating plants, making escape not immediate, reliant on outcome of update health
            if(currentEnemy->isHealthAction(actionCode)){
                updateHealth();
                emit updateHealthDisplay(health,true);
                return;
            }
            //escape
            else{
                enemiesDefeated++;
                emit endBattle();
                return;
            }
        }
        else if(currentEnemy->isHealthAction(actionCode)){
            //modify health from standard health action
            updateHealth();
            emit updateHealthDisplay(health,false);
        }
        emit endTurn();
    }
}

void RPG::updateHealth(){
    int healthMod = currentEnemy->healthMod;
    //prevent overheal
    if(healthMod>0 && healthMod+health>maxHealth){
        health = maxHealth;
    }
    else{
        health+=currentEnemy->healthMod;
    }
}

void RPG::generateNextBattle(){

    if (isPracticeMode){
        currentEnemy = &bestiary[rand() % bestiary.size()];
    }
    else{
        //Set up the table of encounters
        generateEncounterList();

        int maxWeight = 0;
        // Generate the sum of all encounter weights
        for (const auto& beast : encounterTable) {
            maxWeight += beast.second;
        }

        int chosenEncounter = rand() % maxWeight;
        std::string nextEncounter = "";

        // Read each number into the encounter table, taking into account the varying probability for each
        for (const auto& beast : encounterTable) {
            if (nextEncounter == ""){
                chosenEncounter -= beast.second;
                if (chosenEncounter <= 0){
                    nextEncounter = beast.first;
                }
            }
        }
        currentEnemy = (lookupEncounter(nextEncounter));
    }
    //send it to the display
    // display will return execution to game loop
    emit startBattle(currentEnemy->name);
}

Wildlife* RPG::lookupEncounter(std::string nameToFind) {
    QString name = QString::fromStdString(nameToFind);
    Wildlife* encounterPtr = nullptr;
    for(int i=0; i<bestiary.size();i++){
        if (bestiary.at(i).name == name) {
            encounterPtr = &bestiary[i];
            break;
        }
    }

    return encounterPtr;
}

void RPG::generateEncounterList(){
    // Healing item weight = 30 + (maxHealth - health) / maxHealth * 10 + enemiesDefeated * 1
    // Neutral item weight = 30 + (maxHealth - health) / maxHealth * 5 + enemiesDefeated * 1
    // Slightly Dangerous weight = 25 + enemiesDefeated * 2
    // Moderately Dangerous weight = 20 + enemiesDefeated * 3
    // Very Dangerous weight = 10 + enemiesDefeated * 4

    encounterTable["Bane Berry"] = 10 + enemiesDefeated * 4;
    encounterTable["Jimson Weed"] = 20 + enemiesDefeated * 3;
    encounterTable["Blue Bonnet"] = 10 + enemiesDefeated * 4;
    encounterTable["Nightshade"] = 10 + enemiesDefeated * 4;
    encounterTable["Poison Ivy"] = 25 + enemiesDefeated * 2;
    encounterTable["Poison Oak"] = 25 + enemiesDefeated * 2;
    encounterTable["Stinging Nettle"] = 25 + enemiesDefeated * 2;
    encounterTable["Bighorn Sheep"] = 30 + (maxHealth - health) / maxHealth * 5 + enemiesDefeated * 1;
    encounterTable["Black Bear"] = 10 + enemiesDefeated * 4;
    encounterTable["Black Widow"] = 10 + enemiesDefeated * 4;
    encounterTable["Boreal Toad"] = 30 + (maxHealth - health) / maxHealth * 5 + enemiesDefeated * 1;
    encounterTable["Coyote"] = 20 + enemiesDefeated * 3;
    encounterTable["Gila Monster"] = 20 + enemiesDefeated * 3;
    encounterTable["Grizzly Bear"] = 10 + enemiesDefeated * 4;
    encounterTable["Kangaroo Rat"] = 25 + enemiesDefeated * 2;
    encounterTable["Mountain Lion"] = 10 + enemiesDefeated * 4;
    encounterTable["Mule Deer"] = 25 + enemiesDefeated * 2;
    encounterTable["Rattle Snake"] = 10 + enemiesDefeated * 4;
    encounterTable["Wood Scorpion"] = 20 + enemiesDefeated * 3;
    encounterTable["Chanterelle"] = 20 + enemiesDefeated * 3;
    encounterTable["False Morel"] = 20 + enemiesDefeated * 3;
    encounterTable["Penny Bun"] = 25 + enemiesDefeated * 2;
    encounterTable["Puffball"] = 30 + (maxHealth - health) / maxHealth * 10 + enemiesDefeated * 1;
    encounterTable["Fly Agaric"] = 25 + enemiesDefeated * 2;
    encounterTable["True Morel"] = 20 + enemiesDefeated * 3;
    encounterTable["Dandelion"] = 30 + (maxHealth - health) / maxHealth * 10 + enemiesDefeated * 1;
}
