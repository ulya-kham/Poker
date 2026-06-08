#include "Card.h"
#include <iostream>
Card::Card(char s, int r) : suit(s), rank(r) {}//конструктор
char Card::getSuit() const { return suit; }//геттер масти
int Card::getRank() const { return rank; }//геттер знач
const char* Card::rankToString() const {//знач в строку
    switch (rank) {
    case 2: return "2";
    case 3: return "3";
    case 4: return "4";
    case 5: return "5";
    case 6: return "6";
    case 7: return "7";
    case 8: return "8";
    case 9: return "9";
    case 10: return "10";
    case 11: return "J";
    case 12: return "Q";
    case 13: return "K";
    case 14: return "A";
    default: return "?";
    }
}
const char* Card::suitToSymbol() const {//масть в символ
    switch (suit) {
    case 'S': return "♠";//пики
    case 'H': return "♥";//черви
    case 'D': return "♦";//бубны
    case 'C': return "♣";//трефы
    default:  return "?";
    }
}
void Card::print() const {//вывод карты
    std::cout << rankToString() << suitToSymbol();
}
bool Card::operator==(const Card& other) const {//оператор равенства
    return (suit == other.suit && rank == other.rank);
}
bool Card::operator!=(const Card& other) const {//оператор неравенства
    return !(*this == other);
}