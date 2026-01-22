//
// Created by itayb on 21/01/2026.
//

#include "ReverseTree.h"

ReverseTree::ReverseTree() : root(nullptr), size(0), sumNenAbility(new NenAbility(NenAbility::zero())) {}
ReverseTree::~ReverseTree() {
    root = nullptr;
    delete sumNenAbility;
}
HunterNode* ReverseTree::insert(const NenAbility &nenAbility, int fightsHad) {
    HunterNode* newNode = new HunterNode(nenAbility, fightsHad, root);
    size++;
    *newNode->bonusNenAbility += *sumNenAbility;
    if (root != nullptr) {
        *newNode->bonusNenAbility -= *root->bonusNenAbility;
        newNode->bonusFights -= root ->bonusFights;
    }
    *sumNenAbility += nenAbility;
    return newNode;
}
ReverseTree* ReverseTree::Union(ReverseTree &other) {
    if (other.root == nullptr) {
        return this;
    }
    if (size >= other.size) {
        other.root -> parent = root;
        *other.root ->bonusNenAbility += (*sumNenAbility - *root->bonusNenAbility);
        other.root->bonusFights -= root ->bonusFights;
    } else {
        root -> parent = other.root;
        *other.root ->bonusNenAbility += *sumNenAbility;
        *root -> bonusNenAbility -= *other.root->bonusNenAbility;
        root->bonusFights -= other.root ->bonusFights;
        root = other.root;
    }
    size += other.size;
    *sumNenAbility += *other.sumNenAbility;
    return this;
}
void ReverseTree::die() {
    root ->alive = false;
}

bool ReverseTree::isEmpty() {
    return (root == nullptr);
}

void pathCompression(HunterNode *hunter, HunterNode *root) {
    if (hunter == nullptr || hunter == root || hunter->parent == root) {
        return;
    }
    pathCompression(hunter->parent, root);
    hunter ->bonusFights += hunter->parent->bonusFights;
    *hunter ->bonusNenAbility += *hunter->parent->bonusNenAbility;
    hunter ->parent = root;
}

HunterNode* findRoot(HunterNode *hunter) {
    if (hunter == nullptr) {
        return nullptr;
    }
    if (hunter->parent == nullptr) {
        return hunter;
    }
    return findRoot(hunter->parent);
}

int findFights(HunterNode *hunter) {
    if (hunter == nullptr) {
        return 0;
    }
    pathCompression(hunter, findRoot(hunter));
    if (hunter -> parent == nullptr) {
        return hunter->fightsHad + hunter->bonusFights;
    }
    return hunter->fightsHad + hunter->bonusFights + hunter->parent->bonusFights;
}
NenAbility findPartialNenAbility(HunterNode *hunter) {
    if (hunter == nullptr) {
        return NenAbility::zero();
    }
    pathCompression(hunter, findRoot(hunter));
    if (hunter -> parent == nullptr) {
        return (*hunter->bonusNenAbility + *hunter->nenAbility);
    }
    return (*hunter->bonusNenAbility + *hunter->nenAbility + *hunter->parent->bonusNenAbility);
}