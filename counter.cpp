#pragma once

#include "card.h"
#include "iplayer.h"
#include "counter.h"

void Counter::moveOut(std::vector<iCard*>& set, iCard* target)
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
        this->count[index] = 0;
        this->inhand[index] = std::vector<iCard*>();
    }

    // Move out trump
    this->moveOut(this->_unknown, manager.trump());
}

Counter::Counter(const Counter& counter)
{
    this->_desk = counter._desk;
    this->_unknown = counter._unknown;
    this->players = counter.players;
    this->count = counter.count;
    this->inhand = counter.inhand;
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
    std::vector<iCard*>& inhand = this->inhand[index];
    std::vector<iCard*>& desk = this->_desk;

    this->count[index] += desk.size();
    for (auto& card : desk) inhand.push_back(card);
    desk.clear();
}

void Counter::replenish(iPlayer* player, int count)
{
    const int index = player->index();
    const int current = this->count[index];
    if (count > current)
        this->count[index] = count;
}

void Counter::get(iPlayer* player, iCard* card)
{
    const int index = player->index();
    std::vector<iCard*>& inhand = this->inhand[index];

    this->count[index] += 1;
    inhand.push_back(card);
    this->moveOut(this->_unknown, card);
}

void Counter::hit(iPlayer* player, iCard* card)
{
    const int index = player->index();
    std::vector<iCard*>& inhand_ = this->inhand[index];

    this->moveOut(inhand_, card); // for us
    this->moveOut(this->_unknown, card); // for enemy
    this->count[index] -= 1;
    this->_desk.push_back(card);
}

void Counter::get(iPlayer* player, std::string& rank, std::string& suit)
{
    const int index = player->index();
    iCard* card = this->manager->get(rank, suit);
    std::vector<iCard*>& inhand = this->inhand[index];

    this->count[index] += 1;
    inhand.push_back(card);
    this->moveOut(this->_unknown, card);
}

void Counter::hit(iPlayer* player, std::string& rank, std::string& suit)
{
    const int index = player->index();
    iCard* card = this->manager->get(rank, suit);
    std::vector<iCard*>& inhand_ = this->inhand[index];

    this->moveOut(inhand_, card); // for us
    this->moveOut(this->_unknown, card); // for enemy
    this->count[index] -= 1;
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
void Counter::clear(void)
{
    this->_desk.clear();
}
std::vector<iCard*>& Counter::hand(iPlayer* player)
{
    return this->inhand[player->index()];
}
int Counter::left(iPlayer* player)
{
    return this->count[player->index()];
}
