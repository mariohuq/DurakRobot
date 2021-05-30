#pragma once

#include "header.h"

struct Data {
	const double rank;
  const double possibility;
  const iCard* card;

  Data(const iCard* card, double rank, double possibility);
};
