#pragma once

#include "header.h"

namespace Rule
{
std::vector<iCard*> attack(std::vector<iCard*>&, std::vector<iCard*>&); // Analysis for attack
std::vector<iCard*> defend(std::vector<iCard*>&, std::vector<iCard*>&); // Analysis for defend
};
