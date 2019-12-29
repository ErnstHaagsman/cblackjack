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

#endif // CARDS_H
