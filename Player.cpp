#include "Player.hpp"
#include <cassert>
#include <iostream>
#include <array>

class SimplePlayer : public Player {
    public:
        SimplePlayer(const std::string name);
        const std::string & get_name() const override;
        void add_card(const Card &c) override;
        bool make_trump(const Card &upcard, bool is_dealer,
                        int round, Suit &order_up_suit) const override;
        void add_and_discard(const Card &upcard) override;
        Card lead_card(Suit trump) override;
        Card play_card(const Card &led_card, Suit trump) override;

    private:
        std::string name;
        std::vector<Card> hand;

};

class HumanPlayer : public Player {
    public:
        HumanPlayer(const std::string name);
        const std::string & get_name() const override;
        void add_card(const Card &c) override;
        bool make_trump(const Card &upcard, bool is_dealer,
                        int round, Suit &order_up_suit) const override;
        void add_and_discard(const Card &upcard) override;
        Card lead_card(Suit trump) override;
        Card play_card(const Card &led_card, Suit trump) override;

    private:
        std::string name;
        std::vector<Card> hand;

};

//EFFECTS: Returns a pointer to a player with the given name and strategy
//To create an object that won't go out of scope when the function returns,
//use "return new Simple(name)" or "return new Human(name)"
//Don't forget to call "delete" on each Player* after the game is over
Player * Player_factory(const std::string &name, 
                        const std::string &strategy) {
  // We need to check the value of strategy and return 
  // the corresponding player type.
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new SimplePlayer(name);
  } else if (strategy == "Human") {
    return new HumanPlayer(name); 
    }
    return nullptr;
}


SimplePlayer::SimplePlayer(std::string name_in)
    : name(name_in) {}

//EFFECTS: Prints player's name to os
std::ostream &operator<<(std::ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}

//EFFECTS returns player's name
const std::string &SimplePlayer::get_name() const {
    return name;
}

void SimplePlayer::add_card(const Card &c){
    hand.push_back(c);
}

bool SimplePlayer::make_trump(const Card &upcard, bool is_dealer,
    int round, Suit &order_up_suit) const{
    int goodCards = 0;
    Suit trump = upcard.get_suit();

    if(round == 1){
        for(Card card: hand){
            if(card.get_rank() >= JACK && card.is_trump(trump)){
                goodCards++;
            }
        }
        if(goodCards >= 2) {
            order_up_suit = trump;
            return true;
        }
        return false;
    }
    Suit nextSuit = Suit_next(upcard.get_suit());
    if(round == 2){
        if(is_dealer){
            order_up_suit = nextSuit;
            return true;
        }
        for(const Card &card : hand){
            if(card.get_suit() == nextSuit && card.get_rank() >= JACK){
                order_up_suit = nextSuit;
                return true;
            }
        }

    }

    return false;
}

//REQUIRES Player has at least one card
//EFFECTS  Player adds one card to hand and removes one card from hand.
void SimplePlayer::add_and_discard(const Card &upcard){
    hand.push_back(upcard);
    int lowestIndex = 0;
    for(int i = 1; i < hand.size(); i++){
        if(hand[i].get_rank() < hand[lowestIndex].get_rank()){
            lowestIndex = i;
        }
    }
    hand.erase(hand.begin() + lowestIndex);
}

  //REQUIRES Player has at least one card
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.

Card SimplePlayer::lead_card(Suit trump) {
    Card bestCard = hand.at(0);
    int bestIndex = 0;
    bool foundNonTrump = false;
    //bool foundTrump = false;

    for (const Card &card : hand) {
        if (!card.is_trump(trump)) {
            foundNonTrump = true;
        }
    }
    if(foundNonTrump){
        for(size_t i = 0; i < hand.size(); i ++){
            if (!hand[i].is_trump(trump) &&
                hand[i] > bestCard) {
                bestCard = hand[i];
                bestIndex = i;
            }
        }
    } else {
        // All cards are trump so find highest trump
        for (int i = 0; i < hand.size(); i++) {
            if (hand[i] > bestCard) {
                bestCard = hand[i];
                bestIndex = i;
            }
        }
    }

    hand.erase(hand.begin() + bestIndex);
    return bestCard;
}


  //REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  The card is removed from the player's hand.

  //CHECK THIS
Card SimplePlayer::play_card(const Card &led_card, Suit trump) {
    Card highestCard = hand.at(0);
    Card lowestCard = hand.at(0);
    int highestIndex = 0;
    int lowestIndex = 0;

    bool canFollowSuit = false;

    for (int i = 0; i < hand.size(); i++) {
        const Card &card = hand.at(i);

        if (card.get_suit() == led_card.get_suit()) {
            if (!canFollowSuit || card.get_rank() > highestCard.get_rank()) {
                highestCard = card;
                highestIndex = i;
            }
            canFollowSuit = true;
        }

        // Track lowest card overall
        if (card.get_rank() < lowestCard.get_rank()) {
            lowestCard = card;
            lowestIndex = i;
        }
    }

    if (canFollowSuit) {
        hand.erase(hand.begin() + highestIndex);
        return highestCard;
    } else {
        hand.erase(hand.begin() + lowestIndex);
        return lowestCard;
    }
}

// TO DO (all)
HumanPlayer::HumanPlayer(std::string name_in)
    : name(name_in) {}

//EFFECTS returns player's name
const std::string &HumanPlayer::get_name() const {
    return name;
}

void HumanPlayer::add_card(const Card &c){
    hand.push_back(c);
}

bool HumanPlayer::make_trump(const Card &upcard, bool is_dealer,
        int round, Suit &order_up_suit) const{
   return true;
}

//REQUIRES Player has at least one card
//EFFECTS  Player adds one card to hand and removes one card from hand.
void HumanPlayer::add_and_discard(const Card &upcard){
    //nothing

}

  //REQUIRES Player has at least one card
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.

Card HumanPlayer::lead_card(Suit trump) {
    Card bestTrump = hand.at(0);
    return bestTrump;
}


  //REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  The card is removed from the player's hand.
Card HumanPlayer::play_card(const Card &led_card, Suit trump) {

    Card lowestCard = hand.at(0);
    return lowestCard;

}
