#include <cstdlib>
#include <ctime>
#include "AIPlayer.h"
#include "HandEvaluator.h"
int AIPlayer::makeDecision(int computerChips, int minBet, const CustomStack<Card>& computerHand, const Card table[], int tableSize, int playerChips) {//решение о ставке
    if (minBet == -1) return 0;//игрок сдался
    HandResult result = HandEvaluator::findBestHand(computerHand, table, tableSize);//оценка силы руки
    int handStrength = result.rank;
    bool bluff = (std::rand() % 100 < 15);//15% шанс блефа
    int bet = minBet;
    int maxAdditionalBet = minBet + playerChips;//макс ставка
    if (tableSize == 0) {//префлоп
        if (handStrength >= 1 || bluff) {//пара или блеф
            if (handStrength >= 2 || bluff) bet = minBet + std::rand() % 10;//небол ставка
        }
        else {
            if (minBet > 10) return 0;//сброс
        }
    }
    else if (tableSize == 3) {//флоп
        if (handStrength >= 3 || bluff) {//тройка или блеф
            bet = minBet + std::rand() % 20;//сред ставка
        }
        else if (handStrength >= 1) {//пара или лучше
            if (std::rand() % 2 == 0) bet = minBet + std::rand() % 10;//50% повыш
        }
        else {
            if (minBet > computerChips / 3) return 0;//сброс
        }
    }
    else if (tableSize == 4 || tableSize == 5) {//тёрн или ривер
        if (handStrength >= 4 || bluff) {//стрит или блеф
            bet = minBet + std::rand() % 30;//бол ставка
        }
        else if (handStrength >= 2) {//две пары или лучше
            bet = minBet + std::rand() % 15;//сред ставка
        }
        else if (handStrength == 1) {//одна пара
            if (minBet > computerChips / 2) return 0;//сброс
        }
        else {
            return 0;//сброс со слабой рукой
        }
    }
    if (bet < minBet) bet = minBet;//не меньше мин
    if (bet > computerChips) bet = computerChips;//не больше фишек
    if (bet > maxAdditionalBet) bet = maxAdditionalBet;//не больше чем может игрок
    return bet;
}