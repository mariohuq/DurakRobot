#pragma once

#include "header.h"

class Ranker {
	const iCard* _trump;

public:
	static double zipvalue(double);
	static double absolute(const iCard* card);

	Ranker(const iCard*);

	// Calc for progress coefficient
  // left - number of cards unknown
	static double progress(size_t left);
	// Add weight to the trump card
	double trump(const iCard* card);

	// Calc repeat rank for attack and defend
	static double repeat(iPlayer*, const iCard*);
};
