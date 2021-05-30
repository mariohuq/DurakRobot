#pragma once

#include "header.h"

class Thinker {
private:
	iPlayer* we;
	iPlayer* enemy;
	Counter* situation;

private:
	Tree* maketree(bool status, const iCard* card);
	static double ranksum(bool starting, std::stack<Node*>& path);
	static const iCard* byrank(iPlayer*& player);
	static void showpath(std::stack<Node*> path, double rank);

public:
	Thinker(iPlayer* we, iPlayer* enemy,
		Counter* situation);

public:
	const iCard* attack(void);
	const iCard* attack(int);
	const iCard* defend(const iCard* action);
};

// Node for prediction, using in task queue
struct Prediction {
	bool mode; // attack or defense
	int layer; // current prediction's layer
	const iCard* card; // operate card
	Node* father; // father node
	Counter* situation; // layer
};

class TreeMaker {
private:
	Node* root = nullptr; // root node
	int maxlayer = global::depth;
	iPlayer* attacker; iPlayer* defender;

private:
	std::queue<Prediction*> tasks; // tasks queue
	static Node* complex(const iCard*& card, iPlayer*& player);
	std::vector<Counter*> trashbin;

public:
	TreeMaker(bool mode, const iCard* rootcard, iPlayer* attacker, 
		iPlayer* defender, Counter* situation);
	~TreeMaker(void);

public:
	Tree* make(void); // remember to delete tree after use!
};
