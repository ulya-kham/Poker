#pragma once
#include "Card.h"
constexpr int DECK_SIZE = 52;//размер колоды
class Deck {//класс колоды
private:
    Card cards[DECK_SIZE];//массив карт
    int currentIndex;//индекс след карты
public:
    Deck();//конструктор
    void create();//создание колоды
    void shuffle();//перемешивание
    Card dealCard();//раздача карты
    bool isEmpty() const;//проверка на пустоту
    void reset();//сброс колоды
    int getRemainingCards() const;//колво оставшихся
};