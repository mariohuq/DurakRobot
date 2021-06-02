
#include "card.h"
#include "rule.h"
#include "ranker.h"
#include "iplayer.h"
#include "counter.h"

std::vector<iCard*> iPlayer::analysis(void)
{
    std::vector<iCard*> total;

    // The cards on our hands are known
    if (this->us())
    {
        total = this->hand();
    }
    // For enemy need calculate
    else
    {
        std::vector<iCard*>& unknown = this->unknown();
        total.insert(total.end(), this->hand().begin(), this->hand().end());
        total.insert(total.end(), unknown.begin(), unknown.end());
    }

    // analysis for possible cards
    std::vector<iCard*>& desk = this->desk();
    if (this->status() == global::attack)
        return Rule::attack(desk, total);
    else
        return Rule::defend(desk, total);
}

double iPlayer::possibility(iCard* card)
{
    std::vector<iCard*>& unknown = this->unknown();

    // For us if card in hand - 1, otherwise - 0
    if (this->us())
    {
        auto finder = std::find(this->hand().begin(), this->hand().end(), card);
        if (finder == this->hand().end()) return 0.;
        return 1.;
    }

    // For enemy if card in hand - 1
    auto finder = std::find(this->hand().begin(), this->hand().end(), card);
    if (finder != this->hand().end()) return 1.;

    // If card not in unknown, return 0;
    finder = std::find(unknown.begin(), unknown.end(), card);
    if (finder == unknown.end()) return 0.;

    // Otherwise, we need calculate possibility
    // let :
    double a = unknown.size();
    double b = this->hand().size();
    double c = this->hand_size();
    //	m = c - b; n = a + m = a + c - b
    //	The problem is equivalent to extracting m from n objects 
    //  and looking for the specified one;
    //  
    //  Its probability is equal to :
    //		C(n-1, m-1)/C(n, m) = m / n = (c - b) / (a + c - b)


    return (c - b) / (a + c - b);
}

double iPlayer::rank(iCard* card)
{

    double init = 1.0;
    init *= Ranker::absolute(card);
    init *= Ranker::trump(card);
    init *= Ranker::progress(unknown().size());

    // If it's enemy's player
    if (!this->us()) return init;

    // if it's our player
    const double repeat = Ranker::repeat(this, card);
    if (this->status() == global::attack)
        init *= 1 - repeat;
    else
        init *= 1 + repeat;
    return init;
}

int iPlayer::hand_size(void)
{
    return this->counter->hand_size(this);
}
std::vector<iCard*>& iPlayer::desk(void)
{
    return this->counter->desk();
}
const std::vector<iCard*>& iPlayer::hand(void) const
{
    return this->counter->hand(this);
}
std::vector<iCard*>& iPlayer::unknown(void)
{
    return this->counter->unknown();
}
bool iPlayer::status(void) const
{
    return this->_status;
}
int iPlayer::index(void) const
{
    return this->_index;
}
std::string iPlayer::name(void) const
{
    return this->_name;
}
void iPlayer::set_index(int index)
{
    this->_index = index;
}
void iPlayer::set_counter(Counter* counter)
{
    this->counter = counter;
}
bool iPlayer::us(void) const
{
    return this->_we;
}
iPlayer::iPlayer(bool we, bool status, std::string name)
    : _index(0)
    , _we(we)
    , _status(status)
    , _name(std::move(name))
    , counter(nullptr)
{ }

void iPlayer::grab(void)
{
    this->counter->grab(this);
}
void iPlayer::replenish(void)
{
    this->counter->replenish(this);
}
void iPlayer::get(iCard* card)
{
    this->counter->get(this, card);
}
void iPlayer::hit(iCard* card)
{
    this->counter->hit(this, card);
}
void iPlayer::toggle(void)
{
    this->_status = !this->_status;
}
