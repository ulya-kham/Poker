// computer.cpp
#include <cstdlib>
#include "computer.h"
#include "kombinacii.h"

using namespace std;

// etap: 0=префлоп, 1=флоп, 2=тёрн, 3=ривер.
// fichkiIgroka — текущий баланс игрока (сколько он может добавить)
int reshenieComputera(int fichkiComp, int minStavka, karta rukaKaz[2], karta stol[5], int etap, int fichkiIgroka) {
    if (minStavka == -1)
        return 0; // Игрок сдался

    RezultatKombinacii rez = naitiLuchshuyuKombinaciyu(rukaKaz, stol);
    int uroven = rez.tip;
    bool blef = (rand() % 100 < 15);
    int stavka = minStavka;

    // Максимальная ставка, которую может сделать компьютер:
    // он не может заставить игрока поставить больше, чем тот имеет
    int maxDopStavka = minStavka + fichkiIgroka; // игрок может добавить максимум fichkiIgroka

    if (etap == 0) {
        if (uroven >= 1 || blef) {
            if (uroven >= 2 || blef) {
                stavka = minStavka + rand() % 10;
            }
        }
        else {
            if (minStavka > 10) return 0;
        }
    }
    else if (etap == 1) {
        if (uroven >= 3 || blef) {
            stavka = minStavka + rand() % 20;
        }
        else if (uroven >= 1) {
            if (rand() % 2 == 0) {
                stavka = minStavka + rand() % 10;
            }
        }
        else {
            if (minStavka > fichkiComp / 3) return 0;
        }
    }
    else if (etap == 2 || etap == 3) {
        if (uroven >= 4 || blef) {
            stavka = minStavka + rand() % 30;
        }
        else if (uroven >= 2) {
            stavka = minStavka + rand() % 15;
        }
        else if (uroven == 1) {
            if (minStavka > fichkiComp / 2) return 0;
        }
        else {
            return 0;
        }
    }

    // Ограничения
    if (stavka < minStavka) stavka = minStavka;
    if (stavka > fichkiComp) stavka = fichkiComp;
    if (stavka > maxDopStavka) stavka = maxDopStavka; //  ГЛАВНОЕ ИЗМЕНЕНИЕ

    return stavka;
}