// You can edit anything you want in this file.
// However, you need to implement all public Huntech functions, which are provided below as a template.

#include "Huntech26a2.h"

Huntech::Huntech(): huntersTable(new HashTable<HunterNode>), squads(new AVLTree<Squad, int>),
auraSquads(new AVLTree<AuraSquad, AuraSquad>) {} // O(1)

Huntech::~Huntech() { // O(n + k)
    delete huntersTable;
    auraSquads->deleteNodes();
    delete auraSquads;
    delete squads;
}

StatusType Huntech::add_squad(int squadId) { // O(log(k))
    if (squadId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (squads->find(squadId) != nullptr) {
        return StatusType::FAILURE;
    }
    Squad* squadToInsert;
    try {
        squadToInsert = new Squad(squadId);
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR; // failed to create Squad
    }
    try {
        squads->insert(squadToInsert, squadId); // AVL insertion
    }
    catch (const std::bad_alloc&) {

        delete squadToInsert;
        return StatusType::ALLOCATION_ERROR; // failed to create node
    }
    try {
        auraSquads->insert(squadToInsert->getAuraSquad(), *squadToInsert->getAuraSquad()); // AVL insertion
    }
    catch (const std::bad_alloc&) {
        squads->remove(squadId);
        delete squadToInsert;
        return StatusType::ALLOCATION_ERROR; // failed to create node
    }
    return StatusType::SUCCESS;
}


StatusType Huntech::remove_squad(int squadId) { // O(log(k))
    if (squadId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    Squad* squadToRemove = squads->find(squadId);
    if (squadToRemove == nullptr) {
        return StatusType::FAILURE;
    }
    try {
        auraSquads->remove(*squadToRemove->getAuraSquad()); // AVL removal
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        squads->remove(squadId); // AVL removal
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    delete squadToRemove;
    return StatusType::SUCCESS;
}


StatusType Huntech::add_hunter(int hunterId,
                               int squadId,
                               const NenAbility &nenType,
                               int aura,
                               int fightsHad)
{ // O(log(k)) amortized avg
    if (hunterId <= 0 || squadId <= 0 || !nenType.isValid() || aura < 0 || fightsHad < 0) {
        return StatusType::INVALID_INPUT; // invalid input
    }
    Squad* squadToInsert = squads->find(squadId);
    if(huntersTable->find(hunterId) != nullptr || squadToInsert == nullptr) {
        return StatusType::FAILURE; // hunter already in system or squad not in system
    }
    try {
        auraSquads->remove(*squadToInsert->getAuraSquad());
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR; // memory allocation failed
    }
    HunterNode* hunterToInsert;
    try {
        hunterToInsert = squadToInsert->addHunter(nenType, fightsHad, aura);
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR; // memory allocation failed
    }
    try {
        auraSquads->insert(squadToInsert->getAuraSquad(),*squadToInsert->getAuraSquad());
    }
    catch (const std::bad_alloc&) {
        delete hunterToInsert;
        return StatusType::ALLOCATION_ERROR; // memory allocation failed
    }
    try {
        huntersTable->insert(hunterId, hunterToInsert);
    }
    catch (const std::bad_alloc&) {
        delete hunterToInsert;
        return StatusType::ALLOCATION_ERROR; // memory allocation failed
    }
    return StatusType::SUCCESS;
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) { // O(log(k))
    if (squadId1 <= 0 || squadId2 <= 0 || squadId1 == squadId2) {
        return StatusType::INVALID_INPUT; // invalid inputs
    }
    Squad *squad1, *squad2;
    squad1 = squads->find(squadId1);
    squad2 = squads->find(squadId2);
    if (squad1 == nullptr || squad2 == nullptr ||
        !squad1->getHunters()->isAlive() || !squad2->getHunters()->isAlive() ||
        squad1->isEmpty() || squad2->isEmpty()) {
        return StatusType::FAILURE; // dead or empty squad
    }
    return squad1->duel(*squad2); // SUCCESS returned
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) { // O(log*(n)) amortized avg
    if (hunterId <= 0) {
        return StatusType::INVALID_INPUT; // invalid input
    }
    HunterNode* hunter = huntersTable->find(hunterId);
    if (hunter == nullptr) {
        return StatusType::FAILURE; // hunter not in system
    }

    return ;
}

output_t<int> Huntech::get_squad_experience(int squadId) { // O(log(k))
    if (squadId <= 0) {
        return StatusType::INVALID_INPUT; // invalid input
    }
    Squad* squad = squads->find(squadId);
    if (squad == nullptr) {
        return StatusType::FAILURE; // squad not in system
    }
    return squad->getExperience();
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) { // O(log(k))
    if (squads == nullptr || i < 1) {
        return StatusType::FAILURE; // no squads of invalid input
    }
    Squad* squad = squads->getIthValue(i);
    if (squad == nullptr) {
        return StatusType::FAILURE; // invalid input
    }
    return squad->getAuraSquad()->getSquadID();
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) { // O(log*(n)) amortized avg
    if (hunterId <= 0) {
        return StatusType::INVALID_INPUT; // invalid input
    }
    if (huntersTable->find(hunterId) == nullptr) {
        return StatusType::FAILURE; // hunter not in system
    }

    return NenAbility();
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    // O(log*(n) + log(k)) amortized
    if (forcingSquadId <= 0 || forcedSquadId <= 0 || forcingSquadId == forcedSquadId) {
        return StatusType::INVALID_INPUT; // invalid input
    }
    Squad *forcingSquad, *forcedSquad;
    forcingSquad = squads->find(forcingSquadId);
    forcedSquad = squads->find(forcedSquadId);
    if (forcingSquad == nullptr || forcedSquad == nullptr) {
        return StatusType::FAILURE; // squads not in system
    }
    bool joined = forcingSquad->forceJoin(*forcedSquad);
    if (!joined) {
        return StatusType::FAILURE; // join criteria not matched
    }
    return StatusType::SUCCESS;
}
