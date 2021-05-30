#pragma once

#include "header.h"

class Counter {
private:
	std::vector<iCard*> _desk; // cards in desk
	std::vector<iCard*> _unknown; // card we don't know temporary

private:
	CardManager* manager;
	std::array<iPlayer*, global::players> players; // all players
	std::array<size_t, global::players> count; // count of every one's cards
	std::array<std::vector<iCard*>, global::players> inhand; // cards for everyone

public:
	Counter(CardManager&, iCard*);
	Counter(const Counter& counter);

	// Move out one card from a specified vector
	static void moveOut(std::vector<iCard*>& set, iCard*& target);

public:
	int join(iPlayer* player); // join a player

public:
	void grab(iPlayer*);
	void replenish(iPlayer*, int);
	void hit(iPlayer*, iCard*);
	void get(iPlayer*, iCard*);
	void hit(iPlayer*, std::string&, std::string&);
	void get(iPlayer*, std::string&, std::string&);

public:
	void clear(void); // clear desk
	std::vector<iCard*>& desk(void); // return desk
	std::vector<iCard*>& unknown(void); // return unknown
	std::vector<iCard*>& hand(iPlayer*); // return inhands for player
	int left(iPlayer*); // return number of remaining cards for player
};
