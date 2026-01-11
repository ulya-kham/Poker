// kombinacii.h
#ifndef KOMBINACII_H
#define KOMBINACII_H

#include "karta.h"

// Структура для описания комбинации
struct RezultatKombinacii {
    int tip; // 0=старшая, 1=пара, ..., 9=роял-флеш
    int znacheniya[5];
};

// Объявление новых функций
RezultatKombinacii naitiLuchshuyuKombinaciyu(karta ruka[2], karta stol[5]);
bool vseKartiUnikalny(karta rukaIgr[2], karta rukaKaz[2], karta stol[5]);
const char* opisatLuchshuyuRuku(karta ruka[2], karta stol[5]);
int sravnitRuki(karta rukaIgr[2], karta rukaKaz[2], karta stol[5]);

#endif // KOMBINACII_H