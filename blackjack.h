#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "cards.h"

typedef enum {
    PLAYER_MOVES,
    DEALER_MOVES,
    FINISHED
} GameState;

typedef struct {
    cardlist_t *deck;
    cardlist_t *dealerCards;
    cardlist_t *playerCards;
    GameState state;
} blackjack_game_t;

typedef struct {
    int value;
    int aces;
} blackjack_result_t;

// Blackjack methods
blackjack_result_t blackjack_calculate(cardlist_t *pList);
blackjack_game_t* blackjack_start(void);
void blackjack_deal(blackjack_game_t *pGame, cardlist_t *pList);

#endif // BLACKJACK_H
