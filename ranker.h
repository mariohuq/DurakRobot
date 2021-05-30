#pragma once

#include "header.h"

class Ranker {
private:
	Counter* _counter;
	const iCard* _trump;

public:
	static double zipvalue(double);
	static double absolute(const iCard* card);

	Ranker(Counter*, const iCard*);
	double progress(void);
	double trump(const iCard* card);
	double repeat(iPlayer*, const iCard*);
};
