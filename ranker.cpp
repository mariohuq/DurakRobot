
#include "card.h"
#include "iplayer.h"
#include "ranker.h"

double Ranker::zip_value(double value)
{
    return atan(value) / (0.5 * M_PI) + 1;
}

double Ranker::trump(const iCard* card)
{
    if (card->is_trump())
        return global::trump_factor;
    return 1.0;
}

double Ranker::absolute(const iCard* card)
{
    const double raw = global::iranks[card->rank()];
    return global::unconstrained_factor * raw;
}

double Ranker::progress(size_t left)
{
    const double all = global::total;
    const double progress = all / left;
    return zip_value(progress);
}

double Ranker::repeat(iPlayer* player, const iCard* target)
{
    int count = 0;
    std::string rank = target->rank();
    for (auto& card : player->hand())
        if (card->rank() == rank)
            count++;

    return global::repetition_factor * count;
}