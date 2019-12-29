#include <stdio.h>
#include <stdlib.h>
#include "blackjack.h"
#include "cards.h"

blackjack_result_t blackjack_calculate(cardlist_t *pList){
    blackjack_result_t result = {
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

blackjack_game_t* blackjack_start(){
    blackjack_game_t *pGame = malloc(sizeof(blackjack_game_t));

    pGame->deck = newDeck();
    pGame->dealerCards = malloc(sizeof(cardlist_t));
    pGame->playerCards = malloc(sizeof(cardlist_t));
    pGame->state = PLAYER_MOVES;

    return pGame;
}

void blackjack_deal(blackjack_game_t *pGame, cardlist_t *pList){
    card_t *pCard = cardList_removeFirst(pGame->deck);
    cardList_addLast(pList, pCard);
}
