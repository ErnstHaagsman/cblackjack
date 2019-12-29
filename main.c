#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "cards.h"

char* printSuit(Suit suit){
    switch (suit){
        case 0:
            return "Spades";
            break;
        case 1:
            return "Diamonds";
            break;
        case 2:
            return "Hearts";
            break;
        case 3:
            return "Clubs";
            break;
        default:
            return "Unknown";
            break;
    };
}

char* printCard(card_t* pCard){
    static char text[17];
    static char rank[5];

    switch(pCard->rank){
        case 11:
            strcpy(rank, "Jack");
            break;
        case 12:
            strcpy(rank, "Queen");
            break;
        case 13:
            strcpy(rank, "King");
            break;
        case 14:
            strcpy(rank, "Ace");
            break;
        default:
            sprintf(rank, "%d", pCard->rank);
            break;
    };

    sprintf(text, "%s of %s", rank, printSuit(pCard->suit));
    return text;
}

void cardList_addFirst (cardlist_t *list, card_t *card){
    cardnode_t *pNode = malloc(sizeof(cardnode_t));

    pNode->card = card;
    pNode->next = list->first;

    list->length = list->length + 1;

    if (pNode->next == NULL){
        list->last = pNode;
    }

    list->first = pNode;
}

void cardList_addLast (cardlist_t *pList, card_t *pCard){
    cardnode_t *pNode = malloc(sizeof(cardnode_t));

    pNode->card = pCard;
    pNode->next = NULL;

    cardnode_t *pPrevious = pList->last;

    if(pPrevious == NULL){
        // Adding to an empty deck
        pList->first = pNode;
    } else {
        pPrevious->next = pNode;
    }

    pList->last = pNode;
}

card_t* cardList_removeIndex(cardlist_t *list, int index){
    int i = 0;
    cardnode_t *current = list->first;

    if (index == 0){
        list->first = current->next;
        list->length = list->length - 1;
        return current->card;
    }

    cardnode_t *previous;

    while (i < index){
        if(current->next == NULL){
            return NULL;
        }

        previous = current;
        current = current->next;
        i++;
    }

    previous->next = current->next;

    if (previous->next == NULL){
        list->last = previous;
    }

    list->length = list->length - 1;
    return current->card;
}

card_t* cardList_removeFirst(cardlist_t *pList){
    return cardList_removeIndex(pList, 0);
}

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

cardlist_t* newDeck(void){
    // First, create a cardlist with all cards, then
    // randomly take out cards to build a new deck.

    cardlist_t ordered = {NULL, 0};

    for (int i = 0; i<4; i++){
        for (int j = 2; j < 15; j++){
            card_t *pCard = malloc(sizeof(card_t));
            pCard->rank = j;
            pCard->suit = i;
            cardList_addFirst(&ordered, pCard);
        }
    }

    cardlist_t *deck = malloc(sizeof(cardlist_t));
    int index;
    for(int i = 0; i < 52; i++){
        // Length 1 means we need to ask for element 0
        index = ordered.length == 1 ? 0 : rand() % ordered.length;

        card_t *pCardToAdd = cardList_removeIndex(&ordered, index);
        cardList_addFirst(deck, pCardToAdd);
    }

    return deck;
}

void printList(cardlist_t *pDeck){
    cardnode_t *pCurrent = pDeck->first;
    card_t *pCard;
    do {
        pCard = pCurrent->card;
        printw("%s\n", printCard(pCard));
        pCurrent = pCurrent->next;
    } while(pCurrent != NULL);
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
