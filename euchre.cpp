#include <cassert>
#include <iostream>
#include <array>
#include <string>
#include "Card.hpp"
#include "Pack.hpp"
#include "Player.hpp"
#include <fstream>
#include <vector>

using namespace std;

class Game {
public:
    Game(vector<Player*> players_in, Pack pack_in, bool shuffle_in, int handsTotal_in)
        : players(players_in), pack(pack_in), doShuffle(shuffle_in),
          handsTotal(handsTotal_in), team0Score(0), team1Score(0),
          dealerIndex(0), currentHand(0) {}

    void play() {
        while (team0Score < handsTotal && team1Score < handsTotal) {
            play_hand();
            dealerIndex = (dealerIndex + 1) % 4;
            currentHand++;
        }
        if (team0Score >= handsTotal)
            cout << players[0]->get_name() << " and " 
        << players[2]->get_name() << " win!" << endl;
        else
            cout << players[1]->get_name() << " and " 
            << players[3]->get_name() << " win!" << endl;
    }

private:
    vector<Player*> players;
    Pack pack;
    bool doShuffle;
    int handsTotal;
    int team0Score;
    int team1Score;
    int dealerIndex;
    int currentHand;

    void deal() {
        int current = (dealerIndex + 1) % 4;
        for (int i = 0; i < 4; i++) {
            int count = (i % 2 == 0) ? 3 : 2;
            for (int j = 0; j < count; j++)
                players[current]->add_card(pack.deal_one());
            current = (current + 1) % 4;
        }
        current = (dealerIndex + 1) % 4;
        for (int i = 0; i < 4; i++) {
            int count = (i % 2 == 1) ? 3 : 2;
            for (int j = 0; j < count; j++)
                players[current]->add_card(pack.deal_one());
            current = (current + 1) % 4;
        }
    }

    // Returns makerIndex, or -1 if no one ordered up
    int make_trump(const Card &upcard, Suit &trump) {
        // Round 1
        for (int i = 1; i <= 4; i++) {
            int index = (dealerIndex + i) % 4;
            bool picked = players[index]->
            make_trump(upcard, index == dealerIndex, 1, trump);
            if (picked) {
                cout << players[index]->get_name() << " orders up " << trump << endl;
                cout << endl;
                players[dealerIndex]->add_and_discard(upcard);
                return index;
            }
            cout << players[index]->get_name() << " passes" << endl;
        }
        // Round 2
        for (int i = 1; i <= 4; i++) {
            int index = (dealerIndex + i) % 4;
            bool picked = players[index]->
            make_trump(upcard, index == dealerIndex, 2, trump);
            if (picked) {
                cout << players[index]->get_name() << " orders up " << trump << endl;
                cout << endl;
                return index;
            }
            cout << players[index]->get_name() << " passes" << endl;
        }
        return -1;
    }

    // Plays one trick, returns the winner's player index
    int play_trick(int leader, Suit trump) {
        Card ledCard = players[leader]->lead_card(trump);
        cout << ledCard << " led by " << players[leader]->
        get_name() << endl;

        Card winningCard = ledCard;
        int winningPlayer = leader;

        for (int i = 1; i < 4; i++) {
            int currentPlayer = (leader + i) % 4;
            Card played = players[currentPlayer]->play_card(ledCard, trump);
            cout << played << " played by " 
            << players[currentPlayer]->get_name() << endl;
            if (Card_less(winningCard, played, ledCard, trump)) {
                winningCard = played;
                winningPlayer = currentPlayer;
            }
        }

        cout << players[winningPlayer]->get_name() << " takes the trick" << endl;
        cout << endl;
        return winningPlayer;
    }

    void score_hand(int makerIndex, int team0Tricks, int team1Tricks) {
        bool makerTeam0 = (makerIndex % 2 == 0);

        if (makerTeam0) {
            if (team0Tricks == 5) {
                cout << players[0]->get_name() << " and " << players[2]->get_name()
                     << " win the hand" << endl;
                cout << "march!" << endl;
                team0Score += 2;
            } else if (team0Tricks >= 3) {
                cout << players[0]->get_name() << " and " << players[2]->get_name()
                     << " win the hand" << endl;
                team0Score += 1;
            } else {
                cout << players[1]->get_name() << " and " << players[3]->get_name()
                     << " win the hand" << endl;
                cout << "euchred!" << endl;
                team1Score += 2;
            }
        } else {
            if (team1Tricks == 5) {
                cout << players[1]->get_name() << " and " << players[3]->get_name()
                     << " win the hand" << endl;
                cout << "march!" << endl;
                team1Score += 2;
            } else if (team1Tricks >= 3) {
                cout << players[1]->get_name() << " and " << players[3]->get_name()
                     << " win the hand" << endl;
                team1Score += 1;
            } else {
                cout << players[0]->get_name() << " and " << players[2]->get_name()
                     << " win the hand" << endl;
                cout << "euchred!" << endl;
                team0Score += 2;
            }
        }

        cout << players[0]->get_name() << " and " << players[2]->get_name()
             << " have " << team0Score << " points" << endl;
        cout << players[1]->get_name() << " and " << players[3]->get_name()
             << " have " << team1Score << " points" << endl;
        cout << endl;
    }

    void play_hand() {
        if (doShuffle) pack.shuffle();
        else pack.reset();

        cout << "Hand " << currentHand << endl;
        cout << players[dealerIndex]->get_name() << " deals" << endl;

        deal();

        Card upcard = pack.deal_one();
        cout << upcard << " turned up" << endl;

        Suit trump;
        int makerIndex = make_trump(upcard, trump);

        if (makerIndex == -1) return;

        int team0Tricks = 0;
        int team1Tricks = 0;
        int leader = (dealerIndex + 1) % 4;

        for (int trick = 0; trick < 5; trick++) {
            int winner = play_trick(leader, trump);
            if (winner % 2 == 0) team0Tricks++;
            else team1Tricks++;
            leader = winner;
        }

        score_hand(makerIndex, team0Tricks, team1Tricks);
    }
};

int main(int argc, char **argv) {
    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }

    string packFile = argv[1];
    bool doShuffle = (string(argv[2]) == "shuffle");
    int handsTotal = stoi(argv[3]);

    // Print args with trailing space after each
    cout << argv[0] << " " << packFile << " ";
    if (doShuffle) cout << "shuffle " << handsTotal << " ";
    else cout << "noshuffle " << handsTotal << " ";

    vector<Player*> players;
    for (int i = 4; i < 12; i += 2) {
        string name = argv[i];
        string strategy = argv[i + 1];
        cout << name << " " << strategy << " ";
        players.push_back(Player_factory(name, strategy));
    }
    cout << endl;

    ifstream packInput(packFile);
    if (!packInput.is_open()) {
        cout << "Error opening " << packFile << endl;
        return 1;
    }
    Pack pack(packInput);

    Game game(players, pack, doShuffle, handsTotal);
    game.play();

    for (Player* p : players)
        delete p;

    return 0;
}
