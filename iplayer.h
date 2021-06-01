#pragma once

#include "header.h"

class iPlayer {
	int _index; // our index
	bool _we; // its our player
	bool _turn; // our turn or not
	bool _status; // attack or defend
	std::string _name;

	Counter* counter;

public:
	int total(); // return total count of our cards
	std::vector<iCard*>& desk(); // return ref for desk cards
	std::vector<iCard*>& hand(); // return ref for inhand's cards
	std::vector<iCard*>& unknown(); // return ref for unknown cards

	iPlayer(bool we, bool turn, bool status, std::string name);
	bool us() const; // return if it's our player
	bool turn() const; // return if it's our turn
	bool status() const; // return status for player
	int index() const; // return index
	std::string name(void) const; // return name
	void set_index(int);
	void set_counter(Counter* counter); // reset counter for think

	void grab(); // grab all cards
	void replenish(); // get some cards we don't know
	void get(iCard*); // get one card
	void hit(iCard*); // hit one card
	void get(std::string&, std::string&); // get one card
	void hit(std::string&, std::string&); // hit one card

	std::vector<iCard*> analysis(); // analysis we could play
	double possibility(iCard* card); // return for one card's possibility
	double rank(iCard* card); // return for one card's rank

	void transform(); // take in/out our turn's control
	void toggle(); // change our attack/defend mode
};
	