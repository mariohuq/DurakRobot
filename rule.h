#pragma once

#include "header.h"

namespace Rule
{
std::vector<iCard*> attack(std::vector<iCard*>& desk, std::vector<iCard*>& inhand); // Analysis for attack
std::vector<iCard*> defend(std::vector<iCard*>& desk, std::vector<iCard*>& inhand); // Analysis for defend
};
