//
// Created by itayb on 21/01/2026.
//
#include "wet2util.h"
#ifndef DATA_STRUCTURES_WET_2_REVERSE_TREE_H
#define DATA_STRUCTURES_WET_2_REVERSE_TREE_H

struct HunterNode {
	const NenAbility *nenAbility;
	const int fightsHad;
	HunterNode *parent;
	int bonusFights;
	NenAbility *bonusNenAbility;
	bool alive;

	HunterNode(const NenAbility &nenAbility, int fightsHad, HunterNode *parent): nenAbility(&nenAbility),fightsHad(fightsHad),
	parent(parent), bonusFights(0), bonusNenAbility(new NenAbility(NenAbility::zero())), alive(true) {} // O(1)
	~HunterNode() = default; // O(1)
};

class ReverseTree {
	private:
		HunterNode *root;
		int size;
		NenAbility *sumNenAbility;

	public:
		ReverseTree();
		~ReverseTree();
		ReverseTree(const ReverseTree &other) = delete;
		ReverseTree &operator=(const ReverseTree &other) = delete;
		HunterNode* insert(const NenAbility &nenAbility, int fightsHad);
		void pop();
		ReverseTree* Union(ReverseTree &other);
		void die();
		void fight();
		bool isEmpty();
		bool isAlive();
		NenAbility* getTotalNenAbility();
};

int findFights(HunterNode *hunter);
NenAbility findPartialNenAbility(HunterNode *hunter);

#endif //DATA_STRUCTURES_WET_2_REVERSE_TREE_H