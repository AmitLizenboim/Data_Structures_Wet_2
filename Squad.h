//
// Created by itayb on 22/01/2026.
//

#ifndef DATASTRUCTURES_WET2_SQUAD_H
#define DATASTRUCTURES_WET2_SQUAD_H
#include "ReverseTree.h"

class AuraSquad {
private:
    const int squadID;
    int totalAura;
public:
    AuraSquad(int squadID) : squadID(squadID), totalAura(0) {}
    ~AuraSquad() = default;
    AuraSquad(const AuraSquad& other) = default;
    AuraSquad& operator=(const AuraSquad& other) = default;
    bool operator<(AuraSquad &other) { // O(1)
        if (totalAura == other.totalAura) {
            return squadID < other.squadID;
        }
        return totalAura < other.totalAura;
    }
    bool operator>(AuraSquad &other) { // O(1)
        if (totalAura == other.totalAura) {
            return squadID > other.squadID;
        }
        return totalAura > other.totalAura;
    }
    const int getSquadID() {return squadID;}
    int getTotalAura() {return totalAura;}
    void addAura(int aura) { // O(1)
        totalAura += aura;
    }
};

class Squad {
private:
    AuraSquad* auraSquad;
    int experience;
    ReverseTree* hunters;
public:
    Squad(int squadID);
    ~Squad() = default;
    Squad(const Squad&) = delete;
    Squad& operator=(const Squad&) = delete;
    bool operator<(int key);
    bool operator>(int key);
    int getExperience();
    AuraSquad* getAuraSquad();
    HunterNode* addHunter(const NenAbility &nenAbility, int fightsHad,int aura);
    bool forceJoin(Squad &other);
    int duel(Squad &other);
    void die();
    bool isEmpty();
};

#endif //DATASTRUCTURES_WET2_SQUAD_H