#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <vector>
#include <cctype>

int drawCard(std::vector<int>& listOfCards) {
    std::random_device rd;  // Seed (non-deterministic)
    std::mt19937 gen(rd()); // Mersenne Twister engine
    // Define the range [1, 100]
    std::uniform_int_distribution<> dist(1, listOfCards.size());
    int idx = dist(gen);
    int value = listOfCards[idx];
    listOfCards.erase(listOfCards.begin() + idx);
    return value;
}

void aiTurn(std::vector<int>& listOfCards, int& botTrueValue, int& botVisibleValue) {
    while (botTrueValue < 18) {
        int cardDrawn = drawCard(listOfCards);
        if (cardDrawn == 11 && botTrueValue + 11 > 21){
            cardDrawn = 1;
            std::cout << "Card drawn was an ace, value set to 1 otherwise dealer would be busted.\n";
        }
        botTrueValue += cardDrawn;
        botVisibleValue += cardDrawn;
        std::cout << "Dealer drew " << cardDrawn << ". Dealer's visible hand is now " << botVisibleValue << "!\n";
    
    }
}

void gameLoop(double& money, int& gameWin) {
    int playerValue = 0;
    int dealerVisibleValue = 0;
    int dealerTrueValue = 0;
    int cardDrawn = 0;
    bool stand = false;
    std::vector<int> cardsInDeck = {
        1,1,1,1,
        2,2,2,2,
        3,3,3,3,
        4,4,4,4,
        5,5,5,5,
        6,6,6,6,
        7,7,7,7,
        8,8,8,8,
        9,9,9,9,
        10,10,10,10,
        10,10,10,10,
        10,10,10,10,
        11,11,11,11
    };


    std::cout << "=== BLACKJACK ===\n\n";
    std::cout << "Drawing 2 cards for player\n\n";
    cardDrawn = drawCard(cardsInDeck);
    playerValue += cardDrawn;
    std::cout << "Drew a " << cardDrawn << "\n";
    cardDrawn = drawCard(cardsInDeck);
    playerValue += cardDrawn;
    std::cout << "Drew a " << cardDrawn << "\n";
    if(playerValue == 21) {
        std::cout << "Blackjack! Player value is 21!";
        gameWin = 3;
        return;
    }
    std::cout << "Total Value is " << playerValue << "!\nDealer drawing...";
    cardDrawn = drawCard(cardsInDeck);
    dealerTrueValue += cardDrawn;
    dealerVisibleValue += cardDrawn;
    std::cout << "Dealer draws 2 cards including a " << cardDrawn << "!\nDealer visible value is " << dealerVisibleValue << "!\n";
    cardDrawn = drawCard(cardsInDeck);
    dealerTrueValue += cardDrawn;
    
    while (playerValue < 21 && stand == false){
        if (playerValue == 21) {
            std::cout << "21 reached!\n Dealers turn:\n\n";
            stand = true;
            continue;
        } else if(playerValue < 21) {
            std::cout << "Player value is " << playerValue << ". Hit or Stand?\n";
            std::string choice;
            while (choice != "hit" && choice != "stand") {
                std::cin >> choice;
                std::transform(choice.begin(), choice.end(), choice.begin(),
                [](unsigned char c){ return std::tolower(c); });
            }
            if (choice == "hit") {
                cardDrawn = drawCard(cardsInDeck);
                if (cardDrawn == 11 && playerValue + 11 > 21){
                    cardDrawn = 1;
                    std::cout << "Card drawn was an ace, value set to 1 otherwise player would be busted.\n";
                }
                playerValue += cardDrawn;
                std::cout << "Player drawn " << cardDrawn << "!\n";
            } else if (choice == "stand") {
                stand = true;
                continue;
            }
        
               
        } else{
            std::cout << "Player value went over! Player value is " << playerValue << "!\n";
        }
    }
    if (stand == true || playerValue == 21) {
        aiTurn(cardsInDeck, dealerTrueValue, dealerVisibleValue);
        if(dealerTrueValue <= 21) {
            std::cout << "Dealer stands with a true value of " << dealerTrueValue << "!\n";
            if(playerValue>dealerTrueValue){
                std::cout << "Player value is greater than dealer value! Dealer value was " << dealerTrueValue << "!\n";
                gameWin = 2;
                return;
            } else if(playerValue==dealerTrueValue) {
                std::cout << "Player value is equal to dealer value! Dealer value was " << dealerTrueValue << "!\n";
                gameWin = 0;
                return;
            } else {
                std::cout << "Player value is less than dealer value! Dealer value was " << dealerTrueValue << "!\n";
                gameWin = 1;
                return;
            }
        } else {
            std::cout << "Dealer went over with a hand of " << dealerTrueValue << "! You win!\n";
            gameWin = 2;
            return;
        }
    } else if (playerValue > 21) {
        std::cout << "Busted! Player value is " << playerValue << "!\n";
        gameWin = 1;
        return;
    }
    

}

int main() {
    double money = 1000;
    std::cout << "Welcome to the Cadino! LETS GO GAMBLING!\n";
    std::cout << "Your goal is to reach 50000 by gambling!\n\n";
    while (money < 50000 && money > 0) {
        double playerBet = 0;
        int gameCon = 0;
        std::cout << "You have " << money << " credits. How much do you want to bet?\n";
        std::cin >> playerBet;
        if (playerBet < 250 && money > 250) {
            std::cout << "Cmon! You have more money! BET MORE!!11!!1\n\n";
            continue;
        }
        if (playerBet > money) {
            std::cout << "You can't bet more than you have!";
            continue;
        }
        money -= playerBet;
        gameLoop(playerBet, gameCon);
        if(gameCon == 0) {
            std::cout << "You lost nothing from that round!\n\n";
            money += playerBet;
            continue;
        } else if (gameCon == 1) {
            std::cout << "You lost that round and lost the money you bet!\n\n";
            continue;
        } else if(gameCon == 2) {
            std::cout << "You won that round and doubled your bet!\n\n";
            playerBet *= 2;
            money += playerBet;
            continue;
        } else if(gameCon == 3) {
            std::cout << "You tripled your bet from that round!\n\n";
            playerBet *= 3;
            money += playerBet;
        }
    }
    if (money <= 0) {
        std::cout << "You lost all your money and left poor!";
    } else if(money > 50000) {
        std::cout << "You left with 50x the money you entered with!";
    }
    return 0;
}
