
#include "header.h"

#include "card.h"
#include "thinker.h"
#include "iplayer.h"
#include "counter.h"

#include "player.h"
#include "dealer.h"

CardManager Player::manager {};

void Player::setwe(iPlayer* enemy)
{
    this->weinenemy = enemy;
}
iPlayer* Player::getenemy(void) const
{
    return this->enemy;
}

Player::Player(const char* name)
{

    manager.register_card(Dealer::GetTrump());
    manager.set_trump(manager.icard(Dealer::GetTrump()));

    // Initialization module
    this->counter = new Counter(
        manager);

    // Initialize the player and the imaginary enemy
    this->we = new iPlayer(true, global::defend, name);
    this->enemy = new iPlayer(false, global::attack, "GoHan");

    this->we->set_counter(this->counter);
    this->we->set_index(this->counter->join(this->we));

    this->enemy->set_counter(this->counter);
    this->enemy->set_index(this->counter->join(this->enemy));

    // Initialization analyzer
    this->thinker = new Thinker(this->we, this->enemy, this->counter);

    this->weinenemy = nullptr;
    // Initialization flag
    this->ending = false;
}

Player::~Player(void)
{
    // Release all created resources
    delete this->counter;
    delete this->we;
    delete this->enemy;
    delete this->thinker;
}

// Set our offensive and defensive status
void Player::YouTurn(bool status)
{
    if (this->we->status() != status)
    {
        this->we->toggle();
        this->enemy->toggle();
    }
    this->counter->clear_desk();
}

// Take one card
void Player::TakeOneCard(Card*& nc)
{
    iCard* target = manager.icard(nc);

    // While there is no card for us
    if (target == nullptr)
    {
        this->ending = true;
        return;
    }

    manager.register_card(nc);
    this->we->get(target);

    // Notify enemy that we get one card
    this->weinenemy->replenish();
}

// Return card number
int Player::GetCardNum(void)
{
    return this->we->hand_size();
}

// Return if we need cards
bool Player::INeedCard(void)
{
    // If there is no card for us - ending status
    if (ending) return false;

    if (this->we->hand_size() < global::fullcard)
        return true;
    return false;
}

// Show all cards in screen
void Player::ShowCards(void)
{
    std::cout << this->we->name() << ": " << std::endl;
    for (auto& card : this->we->hand())
        std::cout << *card << std::endl;
    std::cout << std::endl;
}

// Grab all cards in table
void Player::TakeCards(void)
{

    // Take card for our side and enemy side
    for (int order = 0; order < 2; order++)
    {
        for (int index = 0; index < Dealer::maxTrick; index++)
        {

            // Check if get non-meaning card
            Card* target = Dealer::GetheadTrick()[order][index];
            if (target == Dealer::GetPas()
                ||
                target == Dealer::GetNocard()
                ||
                target == nullptr) break;

            // Get into our hands, here we just need to record
            // all status updating will in function call we.grab()
            manager.register_card(target);
        }
    }
    // Updating status
    this->we->grab();

    // Notify enemy that we get one card
    this->weinenemy->grab();
}

// Attack for one card
void Player::PutCard(void)
{
    iCard* target; Card* real;

    std::cout << this->we->name() << " - Analysis: " << std::endl;

    // If the desktop is empty, hit card according to the rank
    // otherwise, hit be analyzer
    if (this->counter->desk().empty())
        target = this->thinker->attack();
    else
        target = this->thinker->attack(0);

    // If we get no card could be use
    if (target == nullptr)
        real = Dealer::GetPas();
    else
        // Otherwise, find real card in card mapping
        real = manager.real_card(target);
    Dealer::Attack(real);

    // update in our record
    if (target != nullptr)
    {
        this->we->hit(target);
        // Notify enemy that we hit one card
        this->weinenemy->hit(target);
    }

    std::cout << "Attack: ";
    if (target != nullptr)
        std::cout << *target << std::endl;
    else
        std::cout << "Pass" << std::endl;
    std::cout << std::endl;
}

// Defend one card
void Player::GetHeadTrick(void)
{
    std::cout << this->we->name() << " - Analysis: " << std::endl;

    Card* last = Dealer::GetLastCard();
    // If attacker chooses to give up
    if (last == Dealer::GetPas())
    {
        Dealer::Defend(Dealer::GetNocard());
        std::cout << "Defend: ";
        std::cout << "Pass" << std::endl << std::endl;
        return;
    }

    iCard* target = this->thinker->defend(manager.icard(last));

    // if we could not handle attack
    Card* real;
    if (target == nullptr)
        real = Dealer::GetPas();
    else
        real = manager.real_card(target);
    Dealer::Defend(real);

    // update in our record
    if (target != nullptr)
    {
        this->we->hit(target);
        // Notify enemy that we hit one card
        this->weinenemy->hit(target);
    }

    std::cout << "Defend: ";
    if (target != nullptr)
        std::cout << *target << std::endl;
    else
        std::cout << "Grab" << std::endl;
    std::cout << std::endl;
}
