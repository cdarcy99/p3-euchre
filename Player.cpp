#include "Player.hpp"
#include <cassert>
#include <iostream>
#include <array>
#include <algorithm>

using namespace std;

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
        void print_hand() const;
        

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
        for(const Card card : hand){
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
    Suit trump = upcard.get_suit();
    int lowestIndex = 0;
    for(int i = 1; i < (int)hand.size(); i++){
        bool isLower = Card_less(hand[i], hand[lowestIndex], trump);
        bool isTie = !Card_less(hand[lowestIndex], 
            hand[i], trump) && !isLower;
        bool higherSuit = hand[i].get_suit() >
            hand[lowestIndex].get_suit();
        if(isLower || (isTie && higherSuit)){
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
    int bestIndex = -1;

    // highest non-trump
    for (int i = 0; i < (int)hand.size(); i++) {
        if (!hand[i].is_trump(trump)) {
            bool isHigher = bestIndex == -1 || 
                Card_less(hand[bestIndex], hand[i], trump);
            bool isTie = bestIndex != -1 && 
                !Card_less(hand[bestIndex], hand[i], trump)
                         && !Card_less(hand[i], hand[bestIndex], trump);
            if (isHigher || 
                (isTie && hand[i].get_suit() > hand[bestIndex].get_suit())) {
                bestIndex = i;
            }
        }
    }

    // highest trump
    if (bestIndex == -1) {
        for (int i = 0; i < (int)hand.size(); i++) {
            bool isHigher = bestIndex == -1 || 
                Card_less(hand[bestIndex], hand[i], trump);
            bool isTie = bestIndex != -1 && 
                !Card_less(hand[bestIndex], hand[i], trump)
                         && !Card_less(hand[i], hand[bestIndex], trump);
            if (isHigher || 
                (isTie && hand[i].get_suit() > hand[bestIndex].get_suit())) {
                bestIndex = i;
            }
        }
    }

    Card result = hand[bestIndex];
    hand.erase(hand.begin() + bestIndex);
    return result;
}
  //REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  The card is removed from the player's hand.
Card SimplePlayer::play_card(const Card &led_card, Suit trump) {
    Suit ledSuit = led_card.get_suit(trump);

    bool canFollow = false;
    for (size_t i = 0; i < hand.size(); i++) {
        if (hand[i].get_suit(trump) == ledSuit) {
            canFollow = true;
            break;
        }
    }
    int bestIndex = -1;
    if (canFollow) {
    for (size_t i = 0; i < hand.size(); i++) {
        bool followsSuit = hand[i].get_suit(trump) == ledSuit;
        bool isHigher = bestIndex == -1 || 
            Card_less(hand[bestIndex], hand[i], trump);
        if (followsSuit && isHigher) {
            bestIndex = i;
        }
    }
    } else {
        for (size_t i = 0; i < hand.size(); i++) {
            bool isLower = bestIndex == -1 || 
                Card_less(hand[i], hand[bestIndex], trump);
            bool isTie = bestIndex != -1 && 
                !Card_less(hand[i], hand[bestIndex], trump)
                        && !Card_less(hand[bestIndex], hand[i], trump);
            bool higherSuit = hand[i].get_suit() > hand[bestIndex].get_suit();
            if (isLower || (isTie && higherSuit)) {
                bestIndex = i;
            }
        }
}

    Card result = hand[bestIndex];
    hand.erase(hand.begin() + bestIndex);
    return result;
}


HumanPlayer::HumanPlayer(std::string name_in)
    : name(name_in) {}

//EFFECTS returns player's name
const std::string &HumanPlayer::get_name() const {
    return name;
}

void HumanPlayer::add_card(const Card &c){
    hand.push_back(c);
    std::sort(hand.begin(), hand.end());
}

bool HumanPlayer::make_trump(const Card &upcard, bool is_dealer,
        int round, Suit &order_up_suit) const{
   print_hand();
   cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
   string input;
   cin >> input;
   if (input == "pass") { 
        return false; 
    } 
    order_up_suit = string_to_suit(input); 
    return true;

}

void HumanPlayer::print_hand() const {
  for (size_t i=0; i < hand.size(); ++i)
    std::cout << "Human player " << name << "'s hand: "
         << "[" << i << "] " << hand[i] << "\n";
}

//REQUIRES Player has at least one card
//EFFECTS  Player adds one card to hand and removes one card from hand.
void HumanPlayer::add_and_discard(const Card &upcard){
    hand.push_back(upcard);
    std::sort(hand.begin(), hand.end());

    print_hand();
    std::cout << "Discard upcard: [-1]\n";
    std::cout << "Human player " << name 
              << ", please select a card to discard:\n";

    int index;
    std::cin >> index;

    if(index != -1){
        hand.erase(hand.begin() + index);
    }
}
  //REQUIRES Player has at least one card
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.

Card HumanPlayer::lead_card(Suit trump) {
    print_hand();
    std::cout << "Human player " << name << ", please select a card:\n";
    std::string cardIndex;
    std::cin >> cardIndex;
    Card card = hand.at(stoi(cardIndex));
    hand.erase(hand.begin() + stoi(cardIndex));
    return card;
}


  //REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  The card is removed from the player's hand.
Card HumanPlayer::play_card(const Card &led_card, Suit trump) {
    print_hand();
    cout << "Human player " << name << ", please select a card:\n";

    string input;
    cin >> input;

    int idx = stoi(input);          
    Card chosen = hand.at(idx);   
    hand.erase(hand.begin() + idx);
    return chosen;
}
