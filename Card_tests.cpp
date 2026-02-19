#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

TEST(test_left_bower_detection) {
    Card left(JACK, DIAMONDS); // same color as hearts
    ASSERT_TRUE(left.is_left_bower(HEARTS));
    ASSERT_TRUE(left.is_trump(HEARTS));
}

TEST(test_face_or_ace_true) {
    Card queen(QUEEN, SPADES);
    Card king(KING, SPADES);
    Card ace(ACE, SPADES);

    ASSERT_TRUE(queen.is_face_or_ace());
    ASSERT_TRUE(king.is_face_or_ace());
    ASSERT_TRUE(ace.is_face_or_ace());
}

TEST(test_suit_next_red) {
    ASSERT_EQUAL(DIAMONDS, Suit_next(HEARTS));
    ASSERT_EQUAL(HEARTS, Suit_next(DIAMONDS));
}

TEST(test_suit_next_black) {
    ASSERT_EQUAL(CLUBS, Suit_next(SPADES));
    ASSERT_EQUAL(SPADES, Suit_next(CLUBS));
}

TEST(test_card_comparison_rank) {
    Card low(NINE, SPADES);
    Card high(ACE, SPADES);

    ASSERT_TRUE(low < high);
    ASSERT_TRUE(high > low);
}

TEST(test_left_bower_beats_ace_trump_card) {
    Card left(JACK, DIAMONDS);
    Card aceTrump(ACE, HEARTS);

    ASSERT_TRUE(Card_less(aceTrump, left, HEARTS));
}

TEST(test_right_bower) {
    Card right(JACK, HEARTS);

    ASSERT_TRUE(right.is_right_bower(HEARTS));
    ASSERT_TRUE(right.is_trump(HEARTS));
}

TEST(test_left_bower_get_suit) {
    Card left(JACK, DIAMONDS);

    ASSERT_EQUAL(HEARTS, left.get_suit(HEARTS));
}

TEST(test_trump_false) {
    Card card(NINE, SPADES);
    ASSERT_FALSE(card.is_trump(HEARTS));
}

TEST(test_face_or_ace_false) {
    Card nine(NINE, SPADES);
    ASSERT_FALSE(nine.is_face_or_ace());
}

TEST(test_card_equality) {
    Card c1(ACE, HEARTS);
    Card c2(ACE, HEARTS);
    Card c3(KING, HEARTS);

    ASSERT_TRUE(c1 == c2);
    ASSERT_TRUE(c1 != c3);
}

TEST(test_right_beats_left) {
    Card right(JACK, HEARTS);
    Card left(JACK, DIAMONDS);

    ASSERT_TRUE(Card_less(left, right, HEARTS));
}

TEST(test_trump_beats_non_trump) {
    Card trumpCard(NINE, HEARTS);
    Card nonTrump(ACE, SPADES);

    ASSERT_TRUE(Card_less(nonTrump, trumpCard, HEARTS));
}

// fix
TEST(test_card_less) {
    Card right(JACK, HEARTS);
    Card left(JACK, DIAMONDS);
    Card aceTrump(ACE, HEARTS);
    Card queenTrump(QUEEN, HEARTS);

    // Two trump cards (non-bowers)
    ASSERT_TRUE(Card_less(queenTrump, aceTrump, HEARTS));
    ASSERT_FALSE(Card_less(aceTrump, queenTrump, HEARTS));

    ASSERT_TRUE(Card_less(left, right, HEARTS));
    ASSERT_FALSE(Card_less(left, aceTrump, HEARTS));


}

TEST_MAIN()
