#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"
#include <algorithm>

using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
//EFFECTS returns the Rank corresponding to str, for example "Two" -> TWO
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
  return {};
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  if(is >> str) {
    rank = string_to_rank(str);
  }
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
//EFFECTS returns the Suit corresponding to str, for example "Clubs" -> CLUBS
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
  return {};
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}


/////////////// Write your implementation for Card below ///////////////

// Card implementation functions:
//EFFECTS Initializes Card to the Two of Spades
  Card::Card(){
    rank = TWO;
    suit = SPADES;
  }

  //EFFECTS Initializes Card to specified rank and suit
  Card::Card(Rank rank_in, Suit suit_in){
    rank = rank_in;
    suit = suit_in;
  }


  //EFFECTS Returns the rank
  Rank Card::get_rank() const{
    return rank;
  }

  //EFFECTS Returns the suit.  Does not consider trump.
  Suit Card::get_suit() const{
    return suit;
  }

  //EFFECTS Returns the suit
  //HINT: the left bower is the trump suit!
  Suit Card::get_suit(Suit trump) const{
    if(rank == JACK){
      if((suit == SPADES && trump == CLUBS) || (suit == CLUBS && trump == SPADES)
        || (suit == HEARTS && trump == DIAMONDS) || 
        (suit == DIAMONDS && trump == HEARTS)){
        return trump;
      }
    }
    return suit;
  }

  bool Card::is_face_or_ace() const{
    if(rank >= 9) return true;
    return false;
  }

  //EFFECTS Returns true if card is the Jack of the trump suit
  bool Card::is_right_bower(Suit trump) const{
    if(rank == JACK && suit == trump) return true;
    return false;
  }

  //EFFECTS Returns true if card is the Jack of the next suit
  bool Card::is_left_bower(Suit trump) const{
    if(rank == JACK){
      if((suit == SPADES && trump == CLUBS) || 
        (suit == CLUBS && trump == SPADES)
        || (suit == DIAMONDS && trump == HEARTS) || 
        (suit == HEARTS && trump == DIAMONDS)){
        return true;
      }
    }
    return false;
  }

  //EFFECTS Returns true if the card is a trump card.  All cards of the trump
  // suit are trump cards.  The left bower is also a trump card.
  bool Card::is_trump(Suit trump) const{
    if(suit == trump) return true;
    if (is_left_bower(trump)) return true;
    return false;
  }
// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec before implementing
// the following operator overload functions:
//   operator<<
std::ostream& operator<<(std::ostream& os, const Card& c) {
  os << c.get_rank() << " of " << c.get_suit(); 
  return os; 
}
//   operator>>
std::istream& operator>>(std::istream &is, Card &c) {
  std::string junk;
  is >> c.rank >> junk >> c.suit; 
  return is; 
}
//   operator<
bool operator<(const Card &firstCard, const Card &secondCard) {
  if (firstCard.get_rank() != secondCard.get_rank())
    return firstCard.get_rank() < secondCard.get_rank();
  return firstCard.get_suit() < secondCard.get_suit();
}
//   operator<=
bool operator<=(const Card &firstCard, const Card &secondCard) {
  return !(firstCard > secondCard);
}
//   operator>
bool operator>(const Card &firstCard, const Card &secondCard) {
  return secondCard < firstCard;
}
//   operator>=
bool operator>=(const Card &firstCard, const Card &secondCard) {
  return !(firstCard < secondCard);
}
//   operator==
bool operator==(const Card &firstCard, const Card &secondCard) {
  return firstCard.get_rank() == secondCard.get_rank() &&
  firstCard.get_suit() == secondCard.get_suit();
}
//   operator!=
bool operator!=(const Card &firstCard, const Card &secondCard) {
  return !(firstCard == secondCard);
}


  //EFFECTS returns the next suit, which is the suit of the same color
Suit Suit_next(Suit suit){
  Suit nextSuit;
  if(suit == DIAMONDS) nextSuit = HEARTS; 
  if(suit == HEARTS) nextSuit = DIAMONDS; 
  if(suit == SPADES) nextSuit = CLUBS; 
  if(suit == CLUBS) nextSuit = SPADES; 
  return nextSuit;
}

//EFFECTS Returns true if a is lower value than b.  Uses trump to determine
// order, as described in the spec.
bool Card_less(const Card &a, const Card &b, Suit trump) {

  // Right bower highest
  if (a.is_right_bower(trump)) return false;
  if (b.is_right_bower(trump)) return true;

  // Left bower second highest
  if (a.is_left_bower(trump)) return false;
  if (b.is_left_bower(trump)) return true;

  // Trump beats non-trump
  if (a.is_trump(trump) && !b.is_trump(trump)) return false;
  if (!a.is_trump(trump) && b.is_trump(trump)) return true;

  // Otherwise compare rank normally
  return a.get_rank() < b.get_rank();
}
//EFFECTS Returns true if a is lower value than b.  Uses both the trump suit
//  and the suit led to determine order, as described in the spec.
bool Card_less(const Card &a, const Card &b,
               const Card &led_card, Suit trump) {

  //  compare as trump cards
  if (a.is_trump(trump) || b.is_trump(trump)) {
    return Card_less(a, b, trump);
  }

  Suit led_suit = led_card.get_suit(trump);

  bool a_follows = (a.get_suit(trump) == led_suit);
  bool b_follows = (b.get_suit(trump) == led_suit);

  // If one follows suit and other doesn't
  if (a_follows && !b_follows) return false;
  if (!a_follows && b_follows) return true;

  // If both follow suit
  if (a_follows && b_follows) {
    return a.get_rank() < b.get_rank();
  }

  // Neither follows suit
  return a.get_rank() < b.get_rank();
}