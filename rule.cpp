
#include "card.h"
#include "rule.h"

// Return last card in desk
iCard* last(std::vector<iCard*>& desk)
{
    if (desk.empty()) return nullptr;
    return desk.back();
}

// Return list for shown card
std::unordered_set<std::string> shown_ranks(std::vector<iCard*>& desk)
{
    std::unordered_set<std::string> shown;
    for (auto& card : desk)
        shown.insert(card->rank());
    return shown;
}

std::vector<iCard*> Rule::attack(std::vector<iCard*>& desk, std::vector<iCard*>& hand)
{

    // If hand is empty
    if (hand.empty()) return std::vector<iCard*>();

    // When the offense board's desktop is empty
    // you can play every cards.
    if (desk.empty()) return hand;

    // Otherwise, you can only play cards that you have ever shown.
    std::vector<iCard*> possible;
    std::unordered_set<std::string> shown = shown_ranks(desk);

    for (auto& card : hand)
    {

        // Deal with existed card
        const std::string rank = card->rank();
        if (shown.find(rank) != shown.end())
            possible.push_back(card);
    }

    return possible;
}

std::vector<iCard*> Rule::defend(std::vector<iCard*>& desk, std::vector<iCard*>& hand)
{

    // If hand or desk is empty
    if (hand.empty()) return hand;

    // In defend module, you can play card with bigger or trump card
    std::vector<iCard*> possible;
    const iCard* last_card = last(desk);

    std::string last_suit = last_card->suit();
    bool defend_trump = last_card->is_trump();

    for (auto& card : hand)
    {

        std::string card_suit = card->suit();

        // When in the trump defensive mode, 
        // the current card needs to be greater than the last;
        if (defend_trump == true)
        {

            // If it is general card
            if (!card->is_trump())
                continue;

            if (card->operator>(last_card))
                possible.push_back(card);

            continue;
        }

        // Otherwise, if the current card is a trump card, 
        // we can defend successfully;
        if (card->is_trump())
        {
            possible.push_back(card);
            continue;
        }
        // When both cards are not ace, 
        // check if the two cards are the same suit;
        if (card_suit == last_suit)
        {
            // Defensive success when the same suit 
            // and the current card is larger
            if (card->operator>(last_card))
                possible.push_back(card);
        }
    }
    return possible;
}
