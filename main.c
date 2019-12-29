#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "cards.h"

typedef struct {
    int value;
    int aces;
} blackjack_t;

blackjack_t cardList_blackjack(cardlist_t *pList){
    blackjack_t result = {
        .value = 0,
        .aces = 0
    };

    cardnode_t *pNode = pList->first;
    card_t *pCard;

    while (pNode != NULL){
        pCard = pNode->card;
        if (pCard->rank <= 10){
            result.value += pCard->rank;
        } else if (pCard->rank < 14) {
            // All face cards are worth 10 points
            result.value += 10;
        } else {
            // An ace is 11 for now
            result.value += 11;
            result.aces++;
        }
        pNode = pNode->next;
    }

    // Reduce aces to 1 point, if needed
    while (result.value > 21 && result.aces > 0){
        result.value -= 10;
        result.aces--;
    }

    return result;
}

void playBlackJack(void){
    cardlist_t *pDeck = newDeck();

    cardlist_t dealerHand = { NULL, 0 };
    card_t *pFirstDealerCard = cardList_removeFirst(pDeck);
    cardList_addLast(&dealerHand, pFirstDealerCard);

    printw("Dealer open card: %s", printCard(pFirstDealerCard));

    cardlist_t playerHand = {NULL, 0};

    card_t *pCard;
    for (int i = 0; i < 2; i++){
        pCard = cardList_removeFirst(pDeck);
        cardList_addLast(&playerHand, pCard);
    }

    int gameAlive = 1;
    int playersMove = 1;
    blackjack_t handValue;
    char option;
    while (gameAlive > 0 && playersMove > 0){
        printw("\n\nYour cards:\n");
        printList(&playerHand);

        printw("Would you like to [H]it or [S]tand? ");
        option = toupper(getch());

        switch(option){
        case 'H':
            pCard = cardList_removeFirst(pDeck);
            cardList_addLast(&playerHand, pCard);
            break;
        case 'S':
            playersMove = 0;
            break;
        default:
            printw("Didn't quite get that, let's try again");
            break;
        };

        handValue = cardList_blackjack(&playerHand);
        if (handValue.value > 21){
            printw("\n\n");
            printList(&playerHand);
            printw("\nYou're bust!");
            gameAlive = 0;
            getch();
        }
    }

    if (gameAlive > 0){
        // Dealer plays
        // Let's say the dealer stands on soft 17
        blackjack_t dealerValue;
        do {
            pCard = cardList_removeFirst(pDeck);
            cardList_addLast(&dealerHand, pCard);

            printw("\n\nDealer cards:\n");
            printList(&dealerHand);

            dealerValue = cardList_blackjack(&dealerHand);
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
