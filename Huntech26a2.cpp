// You can edit anything you want in this file.
// However, you need to implement all public Huntech functions, which are provided below as a template.

#include "Huntech26a2.h"

Huntech::Huntech(): huntersTable(new HashTable<HunterNode>), squads(new AVLTree<Squad, int>),
auraSquads(new AVLTree<AuraSquad, AuraSquad>) {}

Huntech::~Huntech() {
    delete huntersTable;
    auraSquads->deleteNodes();
    delete auraSquads;
    delete squads;
}

StatusType Huntech::add_squad(int squadId) {
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


StatusType Huntech::remove_squad(int squadId) {
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
{
    return StatusType::FAILURE;
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {
    return 0;
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
    return 0;
}

output_t<int> Huntech::get_squad_experience(int squadId) {
    return 0;
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    return 0;
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) {
    return NenAbility();
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    return StatusType::FAILURE;
}
