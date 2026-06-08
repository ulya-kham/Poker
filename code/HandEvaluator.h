#pragma once
#include "Card.h"
#include "CustomStack.h"
class HandResult {//результат оценки комбинации
public:
    int rank;//тип комбинации (0-9)
    int values[5];//значения карт для кикеров
    HandResult();//конструктор
};
class HandEvaluator {//оценщик комбинаций
public:
    static HandResult findBestHand(const CustomStack<Card>& hand, const Card table[], int tableSize);//поиск лучшей комбинации
    static bool areCardsUnique(const CustomStack<Card>& playerHand, const CustomStack<Card>& computerHand, const Card table[], int tableSize);//проверка уникальности карт
    static const char* describeBestHand(const CustomStack<Card>& hand, const Card table[], int tableSize);//описание руки строкой
    static int compareHands(const CustomStack<Card>& playerHand, const CustomStack<Card>& computerHand, const Card table[], int tableSize);//сравнение рук
private:
    static HandResult evaluateHand(Card combo[5]);//оценка комбинации из 5 карт
};