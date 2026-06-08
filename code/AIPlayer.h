#pragma once
#include "Card.h"
#include "CustomStack.h"
class AIPlayer {//класс для компьютерного противника
public:
    static int makeDecision(int computerChips, int minBet, const CustomStack<Card>& computerHand, 
        const Card table[], int tableSize, int playerChips);//решение о ставке
};