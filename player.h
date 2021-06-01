#pragma once

#include "header.h"

struct Card;

class PlayerAbstract
{
public:
	virtual ~PlayerAbstract() {};
	virtual void YouTurn(bool) = 0;
	virtual void PutCard() = 0;
	virtual void TakeCards() = 0;
	virtual void GetHeadTrick() = 0;
	virtual void TakeOneCard(Card*& nc) = 0;
	virtual void ShowCards() = 0;
	virtual bool INeedCard() = 0;
	virtual int GetCardNum() = 0;
};

class Player : public PlayerAbstract {
	// Mapping table between external card and internal card
	iPlayer* weinenemy;
	bool ending = false;

protected:
	CardManager* manager = nullptr;
	Counter* counter = nullptr;
	iPlayer* enemy = nullptr;
	Thinker* thinker = nullptr;

	iPlayer* we;
	
public:
	Player(const char* name, CardManager* manager);
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
