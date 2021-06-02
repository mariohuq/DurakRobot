#pragma once

#include "header.h"

namespace Rule
{
std::vector<iCard*> attack(std::vector<iCard*>& desk, std::vector<iCard*>& hand); // Analysis for attack
std::vector<iCard*> defend(std::vector<iCard*>& desk, std::vector<iCard*>& hand); // Analysis for defend
};
