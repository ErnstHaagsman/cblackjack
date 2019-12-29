#ifndef CARDS_H
#define CARDS_H

// Types

typedef enum {
    SPADES,
    DIAMONDS,
    HEARTS,
    CLUBS,
} Suit;

struct Card {
    int rank;
    Suit suit;
};

typedef struct Card card_t;

struct CardNode {
    card_t* card;
    struct CardNode* next;
};

typedef struct CardNode cardnode_t;

struct CardList {
    cardnode_t* first;
    cardnode_t* last;
    int length;
};

typedef struct CardList cardlist_t;

// CardList methods

void cardList_addFirst (cardlist_t *pList, card_t *pCard);
void cardList_addLast (cardlist_t *pList, card_t *pCard);
card_t* cardList_removeIndex(cardlist_t *pList, int index);
card_t* cardList_removeFirst(cardlist_t *pList);
cardlist_t* newDeck(void);

// Utility functions

char* printSuit(Suit suit);
char* printCard(card_t* pCard);

#endif // CARDS_H
