/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#include "wildlife.h"
#include <string>
#include <QFile>
#include <stdlib.h>
#include <time.h>

Wildlife::Wildlife(QString _name, QString _description, int _healthMod, QVector<char> _healthActions, QVector<char> _escapeActions){
    name=_name;
    description=_description;
    healthMod=_healthMod;
    actionNames.push_back("Eat");
    actionNames.push_back("Back Away");
    actionNames.push_back("Run");
    actionNames.push_back("Play Dead");
    actionNames.push_back("Yell");
    actionNames.push_back("Intimidate");
    healthActions=_healthActions;
    escapeActions=_escapeActions;
}

Wildlife::Wildlife(const Wildlife& other){
    name = other.name;
    description = other.description;
    healthMod = other.healthMod;
    actionNames = other.actionNames;
    reactions = other.reactions;
    escapeActions = other.escapeActions;
    healthActions = other.healthActions;
}

Wildlife& Wildlife::operator=(Wildlife other){
    std::swap(name, other.name);
    std::swap(description, other.description);
    std::swap(healthMod, other.healthMod);
    std::swap(actionNames, other.actionNames);
    std::swap(reactions, other.reactions);
    std::swap(escapeActions, other.escapeActions);
    std::swap(healthActions, other.healthActions);
    return*this;
}

QString Wildlife::getReaction(char actionCode){
    if(reactions.contains(actionCode)){
        return(reactions[actionCode]);
    }
    else{
        return("Error 0001: Action Code Not Found");
    }
}

QString Wildlife::getRandomEscapeAction(){
    //failsafe, we should not get here
    if (escapeActions.size()==0){
        return "giving up";
    }
    int idx = std::rand() % escapeActions.size();
    char key = escapeActions.at(idx);
    //ensure uppercase
    key = std::toupper(key);

    //if it is edible
    if(key=='E'){
        if(healthActions.contains('E')&&healthMod>0){
            return "eating the " + name;
        }
        else{
            return getRandomEscapeAction();
        }
    }
    switch(key){
    case 'I':
        return "intimidation";
    case 'R':
        return "running away";
    case 'B':
        return "backing away";
    case 'P':
        return "playing dead";
    case 'Y':
        return "yelling";
    //failsafe, we should not get here
    default:
        return "giving up";
    }
}

bool Wildlife::isEscapeAction(char actionCode){
    return(escapeActions.contains(actionCode));
}

bool Wildlife::isHealthAction(char actionCode){
    return(healthActions.contains(actionCode));
}
