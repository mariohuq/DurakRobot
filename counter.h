#pragma once

#include "header.h"

class Counter
{
    std::vector<iCard*> _desk; // cards in desk
    std::vector<iCard*> _unknown; // card we don't know temporary

    CardManager* manager;
    std::array<iPlayer*, global::players> players; // all players
    std::array<size_t, global::players> hand_sizes; // count of every one's cards
    std::array<std::vector<iCard*>, global::players> hands; // cards for everyone

public:
    Counter(CardManager&);

    int join(iPlayer* player); // join a player

    void grab(iPlayer*);
    // we don't know which card player has taken
    void replenish(iPlayer* player);
    void hit(iPlayer*, iCard*);
    void get(iPlayer*, iCard*);

    void clear_desk(void); // clear desk
    std::vector<iCard*>& desk(void); // return desk
    std::vector<iCard*>& unknown(void); // return unknown
    const std::vector<iCard*>& hand(const iPlayer*) const; // return hand for player
    int hand_size(iPlayer*) const; // return number of remaining cards for player
};
