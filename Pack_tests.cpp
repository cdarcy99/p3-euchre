#include "Pack.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

TEST(test_pack_reset) {
    Pack pack;
    pack.deal_one();
    pack.deal_one();
    pack.reset();
    Card first = pack.deal_one();
    ASSERT_TRUE(first == Card(NINE, SPADES)); // after reset, first card should be back
}

TEST(test_pack_shuffle_changes_order) {
    Pack pack1;
    Pack pack2 = pack1; // copy original
    pack1.shuffle();
    
    bool different = false;
    for(int i = 0; i < 24; i++) {
        if(pack1.deal_one() != pack2.deal_one()) {
            different = true;
        }
    }
    ASSERT_TRUE(different); // shuffle should alter order
}

TEST(test_pack_default_full_order) {
    Pack pack;

    Card first = pack.deal_one();
    ASSERT_EQUAL(Card(NINE, SPADES), first);

    Card current = first;
    for(int i = 1; i < 24; i++) {
        current = pack.deal_one();
    }

    // Last card should be ACE of DIAMONDS
    ASSERT_EQUAL(Card(ACE, DIAMONDS), current);

    ASSERT_TRUE(pack.empty());
}

TEST(test_pack_deal_order) {
    Pack pack;

    Card first = pack.deal_one();
    Card second = pack.deal_one();

    ASSERT_EQUAL(Card(NINE, SPADES), first);
    ASSERT_EQUAL(Card(TEN, SPADES), second);
}

TEST(test_pack_empty) {
    Pack pack;

    ASSERT_FALSE(pack.empty());

    for(int i = 0; i < 24; i++) {
        pack.deal_one();
    }

    ASSERT_TRUE(pack.empty());
}


TEST_MAIN()
