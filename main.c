#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>


// Deck array
const char SUITS[][9] = {"Hearts", "Diamonds", "Spades", "Clubs"};
const char CARDS[][10] = {"Ace", "Two", "Three", "Four", "Five", "Six",
                            "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void checkRound(int playerBalances[][3],  int cardArray[][10], int numPlayers){
    for (int i = 0; i < numPlayers; i++){
        if ((cardArray[i][0] <= 21 && cardArray[i][0]> cardArray[numPlayers][0]) || (cardArray[i][0]<=21 && cardArray[numPlayers][0] > 21)) {
            playerBalances[i][2] = 1;
        } else if (cardArray[i][0] > 21 || cardArray[i][0] < cardArray[numPlayers][0]){
            playerBalances[i][2] = -1;
        }
        else if ( cardArray[i][0] == cardArray[numPlayers][0]){
            playerBalances[i][2] = 0;
        }
        playerBalances[i][0] = playerBalances[i][0] + playerBalances[i][1] * playerBalances[i][2];
    }
}

void sumValues(int cardArray[][10], int playerNo, int numCards){
    int cardTotal = 0;
    for(int i = 1; i<=numCards; i++ ){
        int cardValue = cardArray[playerNo][i] % 13;
        switch (cardValue) {
            case 1:
                cardTotal += 11;
                break;
            case 2:
                cardTotal += 2;
                break;
            case 3:
                cardTotal += 3;
                break;
            case 4:
                cardTotal += 4;
                break;
            case 5:
                cardTotal += 5;
                break;
            case 6:
                cardTotal += 6;
                break;
            case 7:
                cardTotal += 7;
                break;
            case 8:
                cardTotal += 8;
                break;
            case 9:
                cardTotal += 9;
                break;
            case 10:
            case 11:
            case 12:
            case  0:
                cardTotal += 10;
                break;
        }
    }

    for (int i = 1; i <= numCards; i++) {
        if(cardTotal <=21){
            break;
        } else if (cardArray[playerNo][i] %13 == 1){
            cardTotal -= 10;
        }
    }
    cardArray[playerNo][0] = cardTotal;
}
int dealCard(int *array, int *cardCounter){
    *cardCounter += 1;
    return array[*cardCounter -1];
}

void shuffle(int *array, int n){
    srand(time(NULL));
    for(int i = n -1 ; i > 0; i-- ){
        int j = rand() % (i+1);
        swap(&array[i], &array[j]);
    }
}

void getDeck(char deck[52][20]){
    int k = 0;
    for (int i = 0 ; i < 4 ; i++){
        for (int j = 0 ; j < 13 ; j++){
            strcpy(deck[k],CARDS[j]);
            strcat(deck[k], " of ");
            strcat(deck[k], SUITS[i]);
            k++;
        }
    }
}

void makeBets(int playerBalances[][3], int length){
    for (int i = 0; i < length; i++){
        if (playerBalances[i][0] > 0){
            int bet;
            while(1){
                printf("Player %d enter a bet below %d: ", i + 1, playerBalances[i][0]);
                scanf("%d", &bet);
                if (bet >0 && bet <= playerBalances[i][0]){
                    break;
                }
            }
            playerBalances[i][1] = bet;
        } else{ playerBalances[i][1] = 0; }
    }
}


void playLoop(int playerBalances[][3], int numPlayers){
    int playerCards[numPlayers+1][10];
    char cardNames[52][20];
    int dealtCard = 0;

    getDeck(cardNames);
    // Initialize cards
        int cards[52];
        for (int i = 0; i < 52; i++){
            cards[i] = i+1;
        }
    shuffle(cards, 52);
    // Player turn
    for (int i = 0; i < numPlayers; i++){
        if (playerBalances[i][0] <= 0){
            continue;
        }
        int currentCard = 3;
        playerCards[i][1] = dealCard(cards, &dealtCard);
        playerCards[i][2] = dealCard(cards,&dealtCard);
        sumValues(playerCards, i, 2);
        printf("Player %d\n", i +1);
        printf("%s \n%s \nTotal: %d \n",cardNames[playerCards[i][1]-1], cardNames[playerCards[i][2]-1], playerCards[i][0]);
        while(1){
            char decision[4];

            printf("Hit or Stay?: ");
            scanf("%s", decision);
            for (int j = 0 ; decision[j];j++){
                decision[j] = tolower(decision[j]);
            }
            if (strcmp(decision,"hit") == 0){
                playerCards[i][currentCard] = dealCard(cards,&dealtCard);
                printf("%s\n", cardNames[playerCards[i][currentCard]-1]);
                sumValues(playerCards, i, currentCard++);
                printf("Total: %d\n", playerCards[i][0]);
                if (playerCards[i][0] > 21){
                    printf("Bust!\n");
                    playerBalances[i][2] = 0;
                    break;
                }
            }
            else if (strcmp(decision,"stay") ==0)
                break;
        }
        printf("\n");
    }
    // Dealer Turn
    int dealerNumCards = 3;
    playerCards[numPlayers][1] = dealCard(cards, &dealtCard);
    playerCards[numPlayers][2] = dealCard(cards, &dealtCard);
    sumValues(playerCards, numPlayers, 2);
    while (playerCards[numPlayers][0] <= 16){
        playerCards[numPlayers][dealerNumCards] = dealCard(cards, &dealtCard);
        sumValues(playerCards, numPlayers, dealerNumCards);
    }
    printf("Dealer has %d\n", playerCards[numPlayers][0]);
    checkRound(playerBalances,playerCards, numPlayers);

    for (int i = 0; i < numPlayers; i++){
        printf("Player %d's balance is: %d\n", i+1,playerBalances[i][0]);
    }
}



void bank( int playerBalances[][3], int length){
    for (int i = 0; i < length; i++){
        playerBalances[i][0] = 200;
    }

}



int main() {
    char deck[52][20];
        getDeck(deck);
    int numPlayers;
        printf("Enter a number of players: ");
        scanf("%d", &numPlayers);
        int playerBalances[numPlayers+1][3];

    bank(playerBalances, numPlayers);
    makeBets(playerBalances, numPlayers);
    playLoop(playerBalances, numPlayers);
    while(1) {
        char decision;
        printf("Keep Playing (Y/N)?: ");
        scanf(" %c", &decision);

        while(1){
            if (decision == 'Y' || decision =='N'){
            break;}
            }

        if (decision == 'Y'){
            makeBets(playerBalances, numPlayers);
            playLoop(playerBalances, numPlayers);
        } else {
            printf("\nThanks for playing!\nTotals are: \n");
            for (int i = 0; i < numPlayers; i++){
                printf("Player %d : %d\n", i +1, playerBalances[i][0]);
            }
            break;
        }
    }
    return 0;
}
