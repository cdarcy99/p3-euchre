#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(check_make_trump){
    Player * john = Player_factory("John", "Simple");
    
    Suit order_up_suit = SPADES;
    Card upcard(ACE, DIAMONDS);

    john->add_card(Card(KING, HEARTS));
    john->add_card(Card(NINE, SPADES));

    //check when the dealer defaults
    ASSERT_TRUE(john->make_trump(upcard, true, 2, order_up_suit));
    ASSERT_EQUAL(order_up_suit, Suit_next(DIAMONDS));

    // check round 2 works
    ASSERT_TRUE(john->make_trump(upcard, false, 2, order_up_suit));

    //check doesnt work
    Card card2(KING, DIAMONDS);
    john->add_card(card2);
    ASSERT_TRUE(!john->make_trump(upcard, false, 1, order_up_suit));

    delete john;
}

TEST(test_left_bower_counts_as_trump) {
    Player * john = Player_factory("John", "Simple");

    Card upcard(ACE, HEARTS);
    Suit order_up_suit = CLUBS;

    john->add_card(Card(JACK, DIAMONDS)); // left bower
    john->add_card(Card(JACK, HEARTS));   // right bower

    ASSERT_TRUE(john->make_trump(upcard, false, 1, order_up_suit));
    ASSERT_EQUAL(order_up_suit, HEARTS);

    delete john;
}


TEST(test_add_card){
    Player * john = Player_factory("John", "Simple");
    Card upCard(TEN, SPADES);

    john->add_card(Card(ACE, DIAMONDS));
    john->add_card(Card(NINE, HEARTS));

    john->add_and_discard(upCard);

    Suit trump = CLUBS;
    ASSERT_NOT_EQUAL(john->lead_card(trump), Card(NINE, HEARTS));

    delete john;
}

TEST(test_add_and_discard) {
    Player* john = Player_factory("John", "Simple");

    john->add_card(Card(QUEEN, HEARTS)); 
    john->add_card(Card(TEN, CLUBS));   
    john->add_card(Card(NINE, SPADES));  // lowest card

    Card upcard(ACE, DIAMONDS);
    john->add_and_discard(upcard);

    bool hasAce = false, hasTen = false, hasQueen = false;
    for (int i = 0; i < 3; i++) {
        Card c = john->lead_card(SPADES); // temporarily use lead_card to pop cards
        if (c == Card(ACE, DIAMONDS)) hasAce = true;
        if (c == Card(TEN, CLUBS)) hasTen = true;
        if (c == Card(QUEEN, HEARTS)) hasQueen = true;
    }
    ASSERT_TRUE(hasAce);
    ASSERT_TRUE(hasTen);
    ASSERT_TRUE(hasQueen);

    delete john;
}




TEST(test_make_trump_round2_nondealer_pass) {
    Player * john = Player_factory("John", "Simple");

    Card upcard(ACE, DIAMONDS);
    Suit order_up;

    john->add_card(Card(NINE, SPADES));
    john->add_card(Card(TEN, CLUBS));

    // Round 2 non-dealer can pass 
    ASSERT_FALSE(john->make_trump(upcard, false, 1, order_up));

    delete john;
}


TEST(lead_card){
    Player * john = Player_factory("John", "Simple");

    Suit trump = HEARTS;
    john->add_card(Card(KING, SPADES));
    john->add_card(Card(NINE, SPADES));
    john->add_card(Card(ACE, HEARTS));

    ASSERT_EQUAL(john->lead_card(trump), Card(KING, SPADES));
    delete john;

}

TEST(test_lead_all_trump) {
    Player * john = Player_factory("John", "Simple");

    Suit trump = SPADES;

    john->add_card(Card(NINE, SPADES));
    john->add_card(Card(KING, SPADES));
    john->add_card(Card(ACE, SPADES));

    ASSERT_EQUAL(john->lead_card(trump),
                 Card(ACE, SPADES));

    delete john;
}


TEST(play_card){
    Player * john = Player_factory("John", "Simple");

    Card leadCard(JACK, DIAMONDS); // diamonds is lead
    Suit trump = CLUBS;

    john->add_card(Card(KING, DIAMONDS));
    john->add_card(Card(NINE, DIAMONDS));
    john->add_card(Card(ACE, DIAMONDS));

    ASSERT_EQUAL(john->play_card(leadCard, trump),Card(ACE, DIAMONDS));

    // none of the same suit
    Player * chloe = Player_factory("chloe", "Simple");

    chloe->add_card(Card(KING, HEARTS));
    chloe->add_card(Card(NINE, SPADES));
    chloe->add_card(Card(ACE, CLUBS));
    ASSERT_EQUAL(chloe->play_card(leadCard, trump), Card(NINE, SPADES));

    delete john;
    delete chloe;
}
TEST(test_play_card) {
     Player* john = Player_factory("John", "Simple");

    john->add_card(Card(NINE, SPADES));
    john->add_card(Card(TEN, DIAMONDS));
    john->add_card(Card(JACK, SPADES));

    Card led_card(KING, SPADES); // Diamonds not led
    Card played = john->play_card(led_card, HEARTS);

    // follow suit with highest SPADES
    ASSERT_EQUAL(played, Card(JACK, SPADES));

    delete john;
}

TEST(test_make_trump_exactly_two_round1) {
    Player * john = Player_factory("John", "Simple");

    Card upcard(ACE, HEARTS);
    Suit order_up;

    john->add_card(Card(KING, HEARTS));
    john->add_card(Card(QUEEN, HEARTS));

    ASSERT_TRUE(john->make_trump(upcard, false, 1, order_up));
    ASSERT_EQUAL(order_up, HEARTS);

    delete john;
}

TEST(test_make_trump_one_face_round1) {
    Player * john = Player_factory("John", "Simple");

    Card upcard(ACE, HEARTS);
    Suit order_up;

    john->add_card(Card(KING, HEARTS));
    ASSERT_FALSE(john->make_trump(upcard, false, 1, order_up));

    delete john;
}


TEST(test_lead_left_bower_is_trump) {
    Player * john = Player_factory("john", "Simple");

    Suit trump = HEARTS;

    john->add_card(Card(JACK, DIAMONDS)); // left bower
    john->add_card(Card(KING, SPADES));

    ASSERT_EQUAL(john->lead_card(trump), Card(KING, SPADES));

    delete john;
}

// perfers following suit over trump
TEST(test_play_must_follow_suit_over_trump) {
    Player * john = Player_factory("john", "Simple");

    Card led(KING, SPADES);
    Suit trump = HEARTS;

    john->add_card(Card(NINE, SPADES));
    john->add_card(Card(ACE, HEARTS));

    ASSERT_EQUAL(john->play_card(led, trump), Card(NINE, SPADES));

    delete john;
}

TEST(test_screw_the_dealer) {
    Player *dealer = Player_factory("Dealer", "Simple");

    Card upcard(ACE, CLUBS);
    Suit order_up;
    dealer->add_card(Card(NINE, SPADES));
    dealer->add_card(Card(TEN, HEARTS));

    // dealer forced to pick upcard same color
    ASSERT_TRUE(dealer->make_trump(upcard, true, 2, order_up));
    delete dealer;
   
}


// lead_card picks highest card
TEST(test_lead_card) {
    Player* john = Player_factory("John", "Simple");
    john->add_card(Card(NINE, SPADES));
    john->add_card(Card(TEN, SPADES));
    john->add_card(Card(ACE, SPADES));

    Suit trump = CLUBS;
    Card lead = john->lead_card(trump);

    ASSERT_EQUAL(lead, Card(ACE, SPADES));

    delete john;
}



TEST_MAIN()
a