#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <map>
#include <cmath>
#include <ctime>
#include <stack>
#include <array>
#include <queue>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <unordered_map>

#include "dealer.h"

namespace global {
	constexpr auto players = 2;
	constexpr auto suits_count = Dealer::maxSuits;
	constexpr auto ranks_count = Dealer::maxRanks;
	constexpr auto total = suits_count * ranks_count; // Total number of cards

	/* Constants for card.h */
	static const std::array<std::string, suits_count> suits = { "Clubs", "Spades", "Hearts", "Diamonds" };
	static const std::array<std::string, ranks_count> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };
	static std::unordered_map<std::string, short> iranks = {
		{"2", 0}, {"3", 1}, {"4", 2}, {"5", 3}, {"6", 4}, {"7", 5}, {"8", 6},
		{"9", 7}, {"10", 8}, {"Jack", 9}, {"Queen", 10}, {"King", 11}, {"Ace", 12}
	};



	/* Constants for rule.h */
	constexpr bool attack = true;
	constexpr bool defend = false;

	/* Constants for situation.h */
	constexpr auto fullcard = 6;
	constexpr auto our = true;
	constexpr auto enemies = false;

	/* Constants for thinker.h */
	constexpr auto depth = 4;
	constexpr auto possibility_factor = 0.5;
	constexpr auto rank_factor = 1.2;

	/* Constants for ranker.h */
	constexpr auto unconstrained_factor = 0.1;
	constexpr auto trump_factor = 5;
	constexpr auto repetition_factor = 0.15;
}

struct Data;
class Node;
class Tree;

class iCard;
class iPlayer;
class Thinker;
class CardManager;

class Counter;