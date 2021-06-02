#pragma once

#include "card.h"
#include "iplayer.h"
#include "counter.h"

// Move out one card from a specified vector
void moveOut(std::vector<iCard*>& set, iCard* target)
{
    const auto it = std::find(set.begin(), set.end(), target);
    if (it == set.end()) return;
    set.erase(it);
}

Counter::Counter(
    CardManager& manager)
{
    this->_desk.clear();
    this->_unknown = manager.getall();
    this->manager = &manager;
    for (int index = 0; index < global::players; index++)
    {
        this->players[index] = nullptr;
        this->hand_sizes[index] = 0;
        this->hands[index] = std::vector<iCard*>();
    }

    // Move out trump
    moveOut(this->_unknown, manager.trump());
}

int Counter::join(iPlayer* player)
{
    int index = 0;
    for (; index < global::players; index++)
        if (this->players[index] == nullptr)
        {
            this->players[index] = player;
            return index;
        }
    return index;
}

void Counter::grab(iPlayer* player)
{
    int index = player->index();
    std::vector<iCard*>& hand = this->hands[index];
    std::vector<iCard*>& desk = this->_desk;

    this->hand_sizes[index] += desk.size();
    for (auto& card : desk) hand.push_back(card);
    desk.clear();
}

void Counter::replenish(iPlayer* player, int count)
{
    const int index = player->index();
    const int current = this->hand_sizes[index];
    if (count > current)
        this->hand_sizes[index] = count;
}

void Counter::get(iPlayer* player, iCard* card)
{
    const int index = player->index();
    std::vector<iCard*>& hand = this->hands[index];

    this->hand_sizes[index] += 1;
    hand.push_back(card);
    moveOut(this->_unknown, card);
}

void Counter::hit(iPlayer* player, iCard* card)
{
    const int index = player->index();
    std::vector<iCard*>& hand = this->hands[index];

    moveOut(hand, card); // for us
    moveOut(this->_unknown, card); // for enemy
    this->hand_sizes[index] -= 1;
    this->_desk.push_back(card);
}

std::vector<iCard*>& Counter::desk(void)
{
    return this->_desk;
}
std::vector<iCard*>& Counter::unknown(void)
{
    return this->_unknown;
}
void Counter::clear_desk(void)
{
    this->_desk.clear();
}
const std::vector<iCard*>& Counter::hand(const iPlayer* player) const
{
    return this->hands[player->index()];
}
int Counter::hand_size(iPlayer* player) const
{
    return this->hand_sizes[player->index()];
}
