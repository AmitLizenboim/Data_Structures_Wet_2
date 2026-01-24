//
// Created by itayb on 23/01/2026.
//
#include "Squad.h"

Squad::Squad(int squadID) : auraSquad(new AuraSquad(squadID)), experience(0), hunters(new ReverseTree()) {} // O(1)

Squad::~Squad() { // O(1)
    delete auraSquad;
    delete hunters;
}
bool Squad::operator<(int key) { // O(1)
    return auraSquad->getSquadID() < key;
}
bool Squad::operator>(int key) { // O(1)
    return auraSquad->getSquadID() > key;
}
AuraSquad* Squad::getAuraSquad() {return auraSquad;} // O(1)
int Squad::getExperience() {return experience;} // O(1)
ReverseTree* Squad::getHunters() {return hunters;} // O(1)

HunterNode* Squad::addHunter(const NenAbility &nenAbility, int fightsHad,int aura) {
    // function adds the hunter to the tree - O(1)
    if (aura < 0) {
        return nullptr;
    }
    HunterNode* hunter = hunters->insert(nenAbility, fightsHad);
    if (hunter == nullptr) {
        return nullptr;
    }
    auraSquad->addAura(aura);
    return hunter;
}
bool Squad::forceJoin(Squad &other) { // function unites the other squad - O(1)
    if (isEmpty() || (experience + auraSquad->getTotalAura() + hunters->getTotalNenAbility()->getEffectiveNenAbility())
        <=  (other.experience + other.auraSquad->getTotalAura() +
            other.hunters->getTotalNenAbility()->getEffectiveNenAbility())) {
            return false;
    }
    hunters->Union(*other.hunters);
    experience += other.getExperience();
    auraSquad->addAura(other.auraSquad->getTotalAura());
    return true;
}
int Squad::duel(Squad &other) { // function for squad duel - O(1)
    if (isEmpty() || other.isEmpty()) {
        return -1;
    }
    hunters->fight();
    other.hunters->fight();
    int effectiveAura = experience + auraSquad->getTotalAura();
    int otherEffectiveAura = other.experience + other.auraSquad->getTotalAura();
    if (effectiveAura > otherEffectiveAura) {
        experience += 3;
        return 1;
    } else if (effectiveAura < otherEffectiveAura) {
        other.experience += 3;
        return 3;
    } else {
        if (hunters->getTotalNenAbility() > other.hunters->getTotalNenAbility()) {
            experience += 3;
            return 2;
        }else if (hunters->getTotalNenAbility() < other.hunters->getTotalNenAbility()) {
            other.experience += 3;
            return 4;
        }
        experience += 1;
        other.experience += 1;
        return 0;
    }
}

void Squad::die() { // O(1)
    hunters->die();
}
bool Squad::isEmpty() { // O(1)
    return hunters->isEmpty();
}
