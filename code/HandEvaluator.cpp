#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include "HandEvaluator.h"
#include "ConsoleUI.h"
using std::sort;
using std::memcmp;
HandResult::HandResult() : rank(0) {//конструктор
    for (int i = 0; i < 5; i++) values[i] = 0;
}
static bool compareCardsByRank(const Card& a, const Card& b) {//сорт карт по убыв
    return a.getRank() > b.getRank();
}
HandResult HandEvaluator::evaluateHand(Card combo[5]) {//оценка комб из 5 карт
    Card sorted[5];
    for (int i = 0; i < 5; i++) sorted[i] = combo[i];
    sort(sorted, sorted + 5, compareCardsByRank);//сорт по убыв
    bool isFlush = true;//провер флеша
    for (int i = 1; i < 5; i++) {
        if (sorted[i].getSuit() != sorted[0].getSuit()) {
            isFlush = false;
            break;
        }
    }
    bool isStraight = true;//провер стрита
    for (int i = 0; i < 4; i++) {
        if (sorted[i].getRank() - sorted[i + 1].getRank() != 1) {
            isStraight = false;
            break;
        }
    }
    bool isLowStraight = false;//провер низкого стрита (A-2-3-4-5)
    if (!isStraight && sorted[0].getRank() == 14 && sorted[1].getRank() == 5 &&
        sorted[2].getRank() == 4 && sorted[3].getRank() == 3 && sorted[4].getRank() == 2) {
        isLowStraight = true;
        isStraight = true;
    }
    int frequency[15] = { 0 };//подсчёт частоты знач
    for (int i = 0; i < 5; i++) {
        if (sorted[i].getRank() >= 2 && sorted[i].getRank() <= 14) {
            frequency[sorted[i].getRank()]++;
        }
    }
    HandResult result;
    memset(result.values, 0, sizeof(result.values));
    if (isFlush && isStraight && !isLowStraight && sorted[0].getRank() == 14) {//роял-флеш
        result.rank = 9;
        result.values[0] = 14;
        return result;
    }
    if (isFlush && isStraight) {//стрит-флеш
        result.rank = 8;
        result.values[0] = isLowStraight ? 5 : sorted[0].getRank();
        return result;
    }
    for (int z = 14; z >= 2; z--) {//каре
        if (frequency[z] == 4) {
            result.rank = 7;
            result.values[0] = z;
            for (int i = 0; i < 5; i++) {
                if (sorted[i].getRank() != z) {
                    result.values[1] = sorted[i].getRank();
                    break;
                }
            }
            return result;
        }
    }
    int set = 0, pair = 0;//фулл-хаус
    for (int z = 14; z >= 2; z--) {
        if (frequency[z] == 3) set = z;
        else if (frequency[z] == 2) pair = z;
    }
    if (set && pair) {
        result.rank = 6;
        result.values[0] = set;
        result.values[1] = pair;
        return result;
    }
    if (isFlush) {//флеш
        result.rank = 5;
        for (int i = 0; i < 5; i++) result.values[i] = sorted[i].getRank();
        return result;
    }
    if (isStraight) {//стрит
        result.rank = 4;
        result.values[0] = isLowStraight ? 5 : sorted[0].getRank();
        return result;
    }
    if (set) {//тройка
        result.rank = 3;
        result.values[0] = set;
        int idx = 1;
        for (int i = 0; i < 5; i++) {
            if (sorted[i].getRank() != set) result.values[idx++] = sorted[i].getRank();
        }
        return result;
    }
    int p1 = 0, p2 = 0, kicker = 0;//две пары или одна пара
    for (int z = 14; z >= 2; z--) {
        if (frequency[z] == 2) {
            if (!p1) p1 = z;
            else p2 = z;
        }
        else if (frequency[z] == 1) kicker = z;
    }
    if (p1 && p2) {//две пары
        result.rank = 2;
        result.values[0] = p1;
        result.values[1] = p2;
        result.values[2] = kicker;
        return result;
    }
    if (p1) {//одна пара
        result.rank = 1;
        result.values[0] = p1;
        int idx = 1;
        for (int i = 0; i < 5; i++) {
            if (sorted[i].getRank() != p1) result.values[idx++] = sorted[i].getRank();
        }
        return result;
    }
    result.rank = 0;//старшая карта
    for (int i = 0; i < 5; i++) result.values[i] = sorted[i].getRank();
    return result;
}
HandResult HandEvaluator::findBestHand(const CustomStack<Card>& hand, const Card table[], int tableSize) {//поиск лучшей комб
    Card allCards[7];//все доступные карты
    int n = 0;
    for (int i = 0; i < hand.getSize(); i++) {//добавл карты из руки
        if (hand[i].getRank() != 0) allCards[n++] = hand[i];
    }
    for (int i = 0; i < tableSize; i++) {//добавл карты со стола
        if (table[i].getRank() != 0) allCards[n++] = table[i];
    }
    HandResult best;
    bool found = false;
    for (int a = 0; a < n; a++) {//перебор всех комб из 5 карт
        if (allCards[a].getRank() == 0) continue;
        for (int b = a + 1; b < n; b++) {
            if (allCards[b].getRank() == 0) continue;
            for (int c = b + 1; c < n; c++) {
                if (allCards[c].getRank() == 0) continue;
                for (int d = c + 1; d < n; d++) {
                    if (allCards[d].getRank() == 0) continue;
                    for (int e = d + 1; e < n; e++) {
                        if (allCards[e].getRank() == 0) continue;
                        Card combo[5] = { allCards[a], allCards[b], allCards[c], allCards[d], allCards[e] };
                        HandResult current = evaluateHand(combo);
                        if (!found || current.rank > best.rank ||//сравн с лучшим рез
                            (current.rank == best.rank && memcmp(current.values, best.values, sizeof(current.values)) > 0)) {
                            best = current;
                            found = true;
                        }
                    }
                }
            }
        }
    }
    return best;
}
const char* HandEvaluator::describeBestHand(const CustomStack<Card>& hand, const Card table[], int tableSize) {//описание руки строкой
    Card allCards[7];
    int n = 0;
    for (int i = 0; i < hand.getSize(); i++) {
        if (hand[i].getRank() != 0) allCards[n++] = hand[i];
    }
    for (int i = 0; i < tableSize; i++) {
        if (table[i].getRank() != 0) allCards[n++] = table[i];
    }
    static char buffer[100];
    if (n < 5) {//если карт меньше 5
        int maxCard = 2;
        for (int i = 0; i < n; i++) {
            if (allCards[i].getRank() > maxCard) maxCard = allCards[i].getRank();
        }
        sprintf_s(buffer, sizeof(buffer), "Старшая карта (%s)", Card(' ', maxCard).rankToString());
        return buffer;
    }
    HandResult best = findBestHand(hand, table, tableSize);
    const char* rankNames[] = { "Старшая карта", "Пара", "Две пары", "Тройка", "Стрит", "Флеш", "Фулл-хаус", "Каре", "Стрит-флеш", "Роял-флеш" };
    if (best.rank == 0) sprintf_s(buffer, sizeof(buffer), "%s (%s)", rankNames[best.rank], Card(' ', best.values[0]).rankToString());
    else if (best.rank == 1) sprintf_s(buffer, sizeof(buffer), "%s (%s)", rankNames[best.rank], Card(' ', best.values[0]).rankToString());
    else if (best.rank == 2) sprintf_s(buffer, sizeof(buffer), "%s (%s и %s)", rankNames[best.rank], Card(' ', best.values[0]).rankToString(), Card(' ', best.values[1]).rankToString());
    else if (best.rank == 3) sprintf_s(buffer, sizeof(buffer), "%s (%s)", rankNames[best.rank], Card(' ', best.values[0]).rankToString());
    else if (best.rank == 4 || best.rank == 8) sprintf_s(buffer, sizeof(buffer), "%s до %s", rankNames[best.rank], Card(' ', best.values[0]).rankToString());
    else if (best.rank == 6) sprintf_s(buffer, sizeof(buffer), "%s (%s и %s)", rankNames[best.rank], Card(' ', best.values[0]).rankToString(), Card(' ', best.values[1]).rankToString());
    else if (best.rank == 7) sprintf_s(buffer, sizeof(buffer), "%s (%s)", rankNames[best.rank], Card(' ', best.values[0]).rankToString());
    else strcpy_s(buffer, sizeof(buffer), rankNames[best.rank]);
    return buffer;
}
bool HandEvaluator::areCardsUnique(const CustomStack<Card>& playerHand, const CustomStack<Card>& computerHand, const Card table[], int tableSize) {//провер уникальности карт
    Card allCards[9];
    int n = 0;
    for (int i = 0; i < playerHand.getSize(); i++) allCards[n++] = playerHand[i];
    for (int i = 0; i < computerHand.getSize(); i++) allCards[n++] = computerHand[i];
    for (int i = 0; i < tableSize; i++) {
        if (table[i].getRank() != 0) allCards[n++] = table[i];
    }
    for (int i = 0; i < n; i++) {//провер на дубликаты
        for (int j = i + 1; j < n; j++) {
            if (allCards[i].getSuit() == allCards[j].getSuit() && allCards[i].getRank() == allCards[j].getRank()) return false;
        }
    }
    return true;
}
int HandEvaluator::compareHands(const CustomStack<Card>& playerHand, const CustomStack<Card>& computerHand, const Card table[], int tableSize) {//сравн рук
    if (!areCardsUnique(playerHand, computerHand, table, tableSize)) {
        std::cout << "\n\t" << Color::brightRed << "ОШИБКА: одинаковые карты!" << Color::reset << "\n";
        return 0;
    }
    HandResult player = findBestHand(playerHand, table, tableSize);
    HandResult computer = findBestHand(computerHand, table, tableSize);
    std::cout << "\n\t" << Color::brightCyan << "Вы играете: " << Color::brightWhite << describeBestHand(playerHand, table, tableSize) << Color::reset << "\n";
    std::cout << "\t" << Color::coral << "Компьютер играет: " << Color::brightWhite << describeBestHand(computerHand, table, tableSize) << Color::reset << "\n";
    if (player.rank > computer.rank) {
        std::cout << "\n\t" << Color::brightGreen << "Вы выиграли! Ваша комбинация сильнее." << Color::reset << "\n";
        return 1;
    }
    else if (player.rank < computer.rank) {
        std::cout << "\n\t" << Color::white << "Компьютер выиграл! Его комбинация сильнее." << Color::reset << "\n";
        return -1;
    }
    else {
        int cmp = memcmp(player.values, computer.values, sizeof(player.values));
        if (cmp > 0) {
            std::cout << "\n\t" << Color::brightGreen << "Вы выиграли! У вас лучше кикер." << Color::reset << "\n";
            return 1;
        }
        else if (cmp < 0) {
            std::cout << "\n\t" << Color::white << "Компьютер выиграл! У него лучше кикер." << Color::reset << "\n";
            return -1;
        }
        else {
            std::cout << "\n\t" << Color::brightYellow << "= Полная ничья! Банк разделён поровну." << Color::reset << "\n";
            return 0;
        }
    }
}