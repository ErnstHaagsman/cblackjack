#include <stdio.h>
#include <stdlib.h>
#include "cards.h"

void cardList_addFirst (cardlist_t *pList, card_t *pCard){
    cardnode_t *pNode = malloc(sizeof(cardnode_t));

    pNode->card = pCard;
    pNode->next = pList->first;

    pList->length = pList->length + 1;

    if (pNode->next == NULL){
        pList->last = pNode;
    }

    pList->first = pNode;
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

card_t* cardList_removeIndex(cardlist_t *pList, int index){
    int i = 0;
    cardnode_t *current = pList->first;

    if (index == 0){
        pList->first = current->next;
        pList->length = pList->length - 1;
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
        pList->last = previous;
    }

    pList->length = pList->length - 1;
    return current->card;
}

card_t* cardList_removeFirst(cardlist_t *pList){
    return cardList_removeIndex(pList, 0);
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

void printList(cardlist_t *pDeck){
    cardnode_t *pCurrent = pDeck->first;
    card_t *pCard;
    do {
        pCard = pCurrent->card;
        printw("%s\n", printCard(pCard));
        pCurrent = pCurrent->next;
    } while(pCurrent != NULL);
}
