#include "Deck.h"
#include <cstdlib>
#include <ctime>
#include <stdexcept>
Deck::Deck() : currentIndex(0) {//констр созд и перемеш колоду
    create();
    shuffle();
}
void Deck::create() {//созд станд колоды из 52 карт
    char suits[4] = { 'S', 'H', 'D', 'C' };//масти
    int idx = 0;
    for (int s = 0; s < 4; s++) {//цикл по мастям
        for (int r = 2; r <= 14; r++) {//цикл по знач
            cards[idx] = Card(suits[s], r);
            idx++;
        }
    }
    currentIndex = 0;//сброс инд
}
void Deck::shuffle() {//перемеш алгоритмом Фишера-Йетса
    std::srand(static_cast<unsigned>(std::time(nullptr)));//иниц генератора
    for (int i = DECK_SIZE - 1; i > 0; i--) {//от послед к первой
        int j = std::rand() % (i + 1);//случ поз
        Card tmp = cards[i];//обмен карт
        cards[i] = cards[j];
        cards[j] = tmp;
    }
    currentIndex = 0;//сброс инд
}
Card Deck::dealCard() {//разд одной карты
    if (currentIndex >= DECK_SIZE) {//провер на пустоту
        throw std::runtime_error("Невозможно раздать карту: колода пуста");
    }
    return cards[currentIndex++];//возвр карты и увелич инд
}
bool Deck::isEmpty() const {//провер пуста ли колода
    return currentIndex >= DECK_SIZE;
}
void Deck::reset() {//сброс и перемеш колоды
    create();
    shuffle();
}
int Deck::getRemainingCards() const {//колво оставш карт
    return DECK_SIZE - currentIndex;
}