#pragma once

#include "header.h"

struct Card;

class PlayerAbstract
{
public:
    virtual ~PlayerAbstract() = default;
    virtual void YouTurn(bool) = 0;
    virtual void PutCard() = 0;
    virtual void TakeCards() = 0;
    virtual void GetHeadTrick() = 0;
    virtual void TakeOneCard(Card*& nc) = 0;
    virtual void ShowCards() = 0;
    virtual bool INeedCard() = 0;
    virtual int GetCardNum() = 0;
};

class Player : public PlayerAbstract
{
    static CardManager& manager;
    Counter* counter = nullptr;
    Thinker* thinker = nullptr;

    iPlayer* enemy = nullptr;
    iPlayer* weinenemy;
    iPlayer* we;

public:
    Player(const char* name);
    ~Player(void);
    void YouTurn(bool);
    void PutCard(void);
    void TakeCards(void);
    void GetHeadTrick(void);
    void TakeOneCard(Card*& nc);
    void ShowCards(void);
    bool INeedCard(void);
    int GetCardNum(void);

    void setwe(iPlayer* enemy); // Get our player's address
    iPlayer* getenemy(void) const; // Set the address of the imaginary enemy
};
