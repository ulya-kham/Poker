#pragma once
class Card {//карты
private:
    char suit;//масть 
    int rank;//значение 
public:
    Card(char s = ' ', int r = 0);//конструкт
    char getSuit() const;//геттер масти
    int getRank() const;//геттер значения
    const char* rankToString() const;//значение в строку
    const char* suitToSymbol() const;//масть в символ
    void print() const;//вывод карты
    bool operator==(const Card& other) const;//оператор равенства
    bool operator!=(const Card& other) const;//оператор неравенства
};