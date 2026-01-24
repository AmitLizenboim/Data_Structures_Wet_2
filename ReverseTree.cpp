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
    // function inserts a new hunter into the tree - O(1)
    if (!nenAbility.isValid() || fightsHad < 0) {
        return nullptr;
    }
    HunterNode* newNode = new HunterNode(nenAbility, fightsHad, root);
    size++;
    *newNode->bonusNenAbility += *sumNenAbility;
    if (root != nullptr) {
        *newNode->bonusNenAbility -= *root->bonusNenAbility;
        newNode->bonusFights -= root->bonusFights;
    }else {
        root = newNode;
    }
    *sumNenAbility += nenAbility;
    return newNode;
}

ReverseTree* ReverseTree::Union(ReverseTree &other) {
    // U-F Union function, uniting the tree with other, O(1)
    if (other.root == nullptr) {
        return this;
    }
    if (size >= other.size) {
        other.root->parent = root;
        *other.root->bonusNenAbility += (*sumNenAbility - *root->bonusNenAbility);
        other.root->bonusFights -= root->bonusFights;
    } else {
        root->parent = other.root;
        *other.root->bonusNenAbility += *sumNenAbility;
        *root->bonusNenAbility -= *other.root->bonusNenAbility;
        root->bonusFights -= other.root->bonusFights;
        root = other.root;
    }
    size += other.size;
    *sumNenAbility += *other.sumNenAbility;
    return this;
}

void ReverseTree::die() { // function turns the squad dead - O(1)
    if (root != nullptr) {
        root->alive = false;
    }
}

void ReverseTree::fight() { // function increases the squads fight total - O(1)
    if (root != nullptr) {
        root->bonusFights ++;
    }
}


bool ReverseTree::isEmpty() { // function returns true if the tree is empty - O(1)
    return (root == nullptr);
}

NenAbility* ReverseTree::getTotalNenAbility() { // function returns the squad's NenAbility - O(1)
    return sumNenAbility;
}

void pathCompression(HunterNode *hunter, HunterNode *root) {
    // function compresses the tree path from hunter to root - O(log*(Nodes in the tree)) amortized
    if (hunter == nullptr || hunter == root || hunter->parent == root) {
        return;
    }
    pathCompression(hunter->parent, root);
    hunter->bonusFights += hunter->parent->bonusFights;
    *hunter->bonusNenAbility += *hunter->parent->bonusNenAbility;
    hunter->alive = (hunter->alive && hunter->parent->alive);
    hunter->parent = root;
}

HunterNode* findRoot(HunterNode *hunter) {
    // U-F Find function, returning the tree's root - O(log*(Nodes in the tree)) amortized
    if (hunter == nullptr) {
        return nullptr;
    }
    if (hunter->parent == nullptr) {
        return hunter;
    }
    return findRoot(hunter->parent);
}

int findFights(HunterNode *hunter) {
    // function returns the number of fights for hunter, O(log*(Nodes in the tree)) amortized
    if (hunter == nullptr) {
        return 0;
    }
    pathCompression(hunter, findRoot(hunter));
    if (hunter->parent == nullptr) {
        return hunter->fightsHad + hunter->bonusFights;
    }
    return hunter->fightsHad + hunter->bonusFights + hunter->parent->bonusFights;
}

bool findLife(HunterNode *hunter) {
    if (hunter == nullptr) {
        return false;
    }
    if (hunter->parent == nullptr) {
        return hunter->alive;
    }
    pathCompression(hunter, findRoot(hunter));
    return (hunter->alive && hunter->parent->alive);
}


NenAbility findPartialNenAbility(HunterNode *hunter) {
    // function returns the total NenAbility up to hunter by chronological order
    // O(log*(Nodes in the tree)) amortized
    if (hunter == nullptr) {
        return NenAbility::zero();
    }
    pathCompression(hunter, findRoot(hunter));
    if (hunter->parent == nullptr) {
        return (*hunter->bonusNenAbility + *hunter->nenAbility);
    }
    return (*hunter->bonusNenAbility + *hunter->nenAbility + *hunter->parent->bonusNenAbility);
}