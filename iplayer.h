#pragma once

#include "header.h"

class iPlayer
{
    int _index; // our index
    bool _we; // its our player
    bool _status; // attack or defend
    std::string _name;

    Counter* counter;

public:
    int hand_size(); // return total count of our cards
    std::vector<iCard*>& desk(); // return ref for desk cards
    const std::vector<iCard*>& hand() const; // return ref for hands's cards
    std::vector<iCard*>& unknown(); // return ref for unknown cards

    iPlayer(bool we, bool status, std::string name);
    bool us() const; // return if it's our player
    bool status() const; // return status for player
    int index() const; // return index
    std::string name(void) const; // return name
    void set_index(int);
    void set_counter(Counter* counter); // reset counter for think

    void grab(); // grab all cards
    void replenish(); // get some cards we don't know
    void get(iCard*); // get one card
    void hit(iCard*); // hit one card

    std::vector<iCard*> analysis(); // analysis we could play
    double possibility(iCard* card); // return for one card's possibility
    double rank(iCard* card); // return for one card's rank

    void toggle(); // change our attack/defend mode
};
