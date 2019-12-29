#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <ctype.h>
#include "cards.h"
#include "blackjack.h"

typedef struct {
    WINDOW *titleBar;
    WINDOW *dealerWindow;
    WINDOW *playerWindow;
    WINDOW *message;
} gamescreen_t;

void init_screen(gamescreen_t *screen){
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    screen->titleBar = newwin(1,10,0,0);
    screen->dealerWindow = newwin(7, 20, 3, 10);
    screen->playerWindow = newwin(7, 20, 11, 10);
    screen->message = newwin(1, xMax, yMax - 1, 0);
    refresh();
}

void wPrintList(WINDOW *win, cardlist_t *pDeck){
    cardnode_t *pCurrent = pDeck->first;
    card_t *pCard;
    do {
        pCard = pCurrent->card;
        wprintw(win, "%s\n", printCard(pCard));
        pCurrent = pCurrent->next;
    } while(pCurrent != NULL);
}

void animateCard(WINDOW *win, card_t *pCard){
    char *card = printCard(pCard);

    for(int i =0; i<3; i++){
        wprintw(win, ".");
        wrefresh(win);
        usleep(500 * 1000);
    }

    wprintw(win, "\b\b\b%s\n", card);
}

void drawGame(gamescreen_t *screen, blackjack_game_t *pGame, char *message){
    wclear(screen->dealerWindow);
    wprintw(screen->dealerWindow, "Dealer cards:\n\n");
    wPrintList(screen->dealerWindow, pGame->dealerCards);
    wrefresh(screen->dealerWindow);

    wclear(screen->playerWindow);
    wprintw(screen->playerWindow, "Your cards:\n\n");
    wPrintList(screen->playerWindow, pGame->playerCards);
    wrefresh(screen->playerWindow);

    wclear(screen->message);
    wprintw(screen->message, "%s", message);
    wrefresh(screen->message);
}

void playBlackJack(gamescreen_t *screen){
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
        drawGame(screen, pGame, "Would you like to [H]it or [S]tand?");
        option = toupper(getch());

        switch(option){
        case 'H':
            blackjack_deal(pGame, pGame->playerCards);
            animateCard(screen->playerWindow, pGame->playerCards->last->card);
            break;
        case 'S':
            playersMove = 0;
            break;
        default:
            break;
        };

        handValue = blackjack_calculate(pGame->playerCards);
        if (handValue.value > 21){
            drawGame(screen, pGame, "You're bust!");
            gameAlive = 0;
            getch();
        }
    }

    if (gameAlive > 0){
        // Dealer plays
        // Let's say the dealer stands on soft 17
        blackjack_result_t dealerValue;
        drawGame(screen, pGame, "Dealer plays");
        do {
            blackjack_deal(pGame, pGame->dealerCards);

            animateCard(screen->dealerWindow, pGame->dealerCards->last->card);

            dealerValue = blackjack_calculate(pGame->dealerCards);
        } while (dealerValue.value < 17);

        if (dealerValue.value > 21){
            drawGame(screen, pGame, "You win!");
        } else if (handValue.value > dealerValue.value) {
            drawGame(screen, pGame, "You win!");
        } else {
            drawGame(screen, pGame, "Dealer wins!");
        }

        getch();
    }
}

int main()
{
    initscr();
    cbreak();
    noecho();

    gamescreen_t *screen = malloc(sizeof(gamescreen_t));

    init_screen(screen);
    wprintw(screen->titleBar, "Blackjack");
    wrefresh(screen->titleBar);

    while(true){
        playBlackJack(screen);
    }

    return 0;
}
