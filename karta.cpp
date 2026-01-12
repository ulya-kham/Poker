// karta.cpp
#include <cstdlib>
#include <cstring>
#include "karta.h"


const char* znachToStr(int z) {
    switch (z) {
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

const char* mastToSym(char m) {
    switch (m) {
    case 'H': return "(^^)";
    case 'D': return "<>";
    case 'S': return "_()_";
    case 'C': return "X";
    default: return "?";
    }
}

void sozdatKolodu(karta koloda[]) {
    char masti[4] = { 'S', 'H', 'D', 'C' };
    int idx = 0;
    for (int m = 0; m < 4; m++) {
        for (int z = 2; z <= 14; z++) {
            koloda[idx].mast = masti[m];
            koloda[idx].znach = z;
            idx++;
        }
    }
}

void tasovat(karta koloda[]) {
    for (int i = KOL_KART - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        karta tmp = koloda[i];
        koloda[i] = koloda[j];
        koloda[j] = tmp;
    }
}