// computer.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "computer.h"
#include "kombinacii.h"
#include <cstdlib>
using namespace std;

int reshenieComputera(int fichkiComp, int minStavka, karta rukaKaz[2], karta stol[5], int etap) {
    // Если игрок сдался — компьютер не ставит
    if (minStavka == -1) return 0;

    // Оцениваем силу руки
    RezultatKombinacii rez = naitiLuchshuyuKombinaciyu(rukaKaz, stol);
    int uroven = rez.tip;

    // Вероятность блефа (5%)
    bool blef = (rand() % 100 < 15);

    // Минимальная ставка — это то, что нужно уравнять
    int stavka = minStavka;

    // Логика по этапам
    if (etap == 0) { // Префлоп
        if (uroven >= 1 || blef) {
            // Есть пара или блеф — можно колл/повысить
            if (uroven >= 2 || blef) {
                stavka = minStavka + rand() % 10;
            }
        }
        else {
            // Слабая рука — фолд при высокой ставке
            if (minStavka > 10) return 0;
        }
    }
    else if (etap == 1) { // Флоп
        if (uroven >= 3 || blef) { // Тройка и выше или блеф
            stavka = minStavka + rand() % 20;
        }
        else if (uroven >= 1) { // Пара или две
            // Коллируем или немного повышаем
            if (rand() % 2 == 0) {
                stavka = minStavka + rand() % 10;
            }
        }
        else {
            // Ничего нет — фолд при ставке > 1/3 фишек
            if (minStavka > fichkiComp / 3) return 0;
        }
    }
    else if (etap == 2 || etap == 3) { // Тёрн / Ривер
        if (uroven >= 4 || blef) { // Стрит и выше или блеф
            stavka = minStavka + rand() % 30;
        }
        else if (uroven >= 2) { // Две пары и выше
            stavka = minStavka + rand() % 15;
        }
        else if (uroven == 1) { // Пара
            if (minStavka > fichkiComp / 2) return 0;
        }
        else {
            return 0; // Фолд
        }
    }

    // Ограничения
    if (stavka < minStavka) stavka = minStavka;
    if (stavka > fichkiComp) stavka = fichkiComp;

    return stavka;
}