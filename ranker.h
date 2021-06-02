#pragma once

#include "header.h"

namespace Ranker
{
// Add weight to the trump card
double trump(const iCard* card);

// Scale the function value field to [1,2]
double zip_value(double);

// Calc absolute rank for card
double absolute(const iCard* card);

// Calc for progress coefficient
// left - number of cards unknown
double progress(size_t left);

// Calc repeat rank for attack and defend
double repeat(iPlayer*, const iCard*);
};
