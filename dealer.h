#pragma once

enum { PAS = 300, NOCARD = 400 };
//PAS - sign of skip of the move (no way to move or beat)
//NOCARD - sign of skip of the move (has no cards in hand)

static const char* suits[] = { "Clubs", "Spades", "Hearts", "Diamonds" };
static const char* suitsSymb[] = { "\x5", "\x6", "\x3", "\x4" };
static const char* ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };

struct Card {
private:
	int suit;
	int rank;

	bool operator>(const Card& card);
	friend class Dealer;

	Card(int suit = -1, int rank = -1);
};


class Dealer {
public:
	static const int maxSuits = 4;
	static const int maxRanks = 13;
	static const int maxTrick = 6;
private:
	static int currentCard;
	static Card* trump;  // Trump card (Kozyr)
	static Card* noCard, * pasCard; // card - sign "no cards" и "pass"
	static Card deck[maxSuits * maxRanks]; //deck

	static bool tableRanks[maxRanks];   // ranks of cards, that are on the table

	static int currentHeadTrik; // No. of move in round
	static Card* headTrick[2][maxTrick]; // table [0] - move of player, [1] - successful defense of a card
	static void GenerateDeck();



public:

	// shuffle the deck - initializes the deck and all variables.
	// chooses trump
	static void ShuffleDec();
	// take a card from the deck. returns true, if the deck is not empty.
	static bool GetCard(Card*& outCard);
	// returns current trump by the struct Card (only suit matters).
	static Card* GetTrump();
	// returns a number of cards went out from the deck
	static int getcurrentCard();


	// returns pointer to the table
	static Card* (*GetheadTrick())[maxTrick];


	// returns string literal containing suit
	static const char* SuitName(const Card* card);
	// returns string literal — rank of the card
	static const char* RankName(const Card* card);


	// * returns index corresponding to suit
	static int SuitIndex(const Card* card);
	// returns index corresponding to rank of the card
	static int RankIndex(const Card* card);
	// returns No. of the attack (0-5)
	static int GetCurrentHeadTrik();
	// Checks if next attack possible (number of attacks < 6, defender didn't passed)
	static bool NextTrikEnable();


	// * displays the card
	static void ShowCard(const Card* card);
	// displays the table
	static void ShowTable();
	//* returns pointer to card "pass" and "no cards"
	static Card* GetPas();
	static Card* GetNocard();
	// returns pointer to the last attack card and to the last defend card
	static Card* GetLastCard();
	static Card* GetLastDefendCard();


	//clears the table
	static void ClearTable();
	//checks cards on the table if they are correct
	static bool CheckHeadTrick();

	//attack the defender by card
	static void Attack(Card* card);
	//defend by card
	static void Defend(Card* card);

	~Dealer();
};