#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "cards.h"
#include "blackjack.h"

void playBlackJack(void){
    blackjack_game_t *pGame = blackjack_start();

    blackjack_deal(pGame, pGame->dealerCards);

    for (int i = 0; i < 2; i++){
        blackjack_deal(pGame, pGame->playerCards);
    }

    int gameAlive = 1;
    int playersMove = 1;
    blackjack_result_t handValue;
    char option;
    while (gameAlive > 0 && playersMove > 0){
        clear();
        printw("Dealer cards:\n");
        printList(pGame->dealerCards);

        printw("\n\nYour cards:\n");
        printList(pGame->playerCards);

        printw("Would you like to [H]it or [S]tand? ");
        option = toupper(getch());

        switch(option){
        case 'H':
            blackjack_deal(pGame, pGame->playerCards);
            break;
        case 'S':
            playersMove = 0;
            break;
        default:
            printw("Didn't quite get that, let's try again");
            break;
        };

        handValue = blackjack_calculate(pGame->playerCards);
        if (handValue.value > 21){
            printw("\n\n");
            printList(pGame->playerCards);
            printw("\nYou're bust!");
            gameAlive = 0;
            getch();
        }
    }

    if (gameAlive > 0){
        // Dealer plays
        // Let's say the dealer stands on soft 17
        blackjack_result_t dealerValue;
        do {
            blackjack_deal(pGame, pGame->dealerCards);

            printw("\n\nDealer cards:\n");
            printList(pGame->dealerCards);

            dealerValue = blackjack_calculate(pGame->dealerCards);
        } while (dealerValue.value < 17);

        if (dealerValue.value > 21){
            printw("\n\nYou win!");
        } else if (handValue.value > dealerValue.value) {
            printw("\n\nYou win!");
        } else {
            printw("\n\nDealer wins!");
        }

        getch();
    }
}

int main()
{
    initscr();
    cbreak();

    printw("Let's play Blackjack!\n\n\n");

    while(true){
        playBlackJack();
    }

    return 0;
}
