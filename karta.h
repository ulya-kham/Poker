#pragma once
// karta.h
#ifndef KARTA_H
#define KARTA_H

const int KOL_KART = 52;

struct karta {
    char mast;  // 'S' - пики (_()_), 'H' - черви (^^), 'D' - бубны <>, 'C' - крести X
    int znach;  // 2Ц14 (11=J, 12=Q, 13=K, 14=A)
};

const char* znachToStr(int z);
const char* mastToSym(char m);
void sozdatKolodu(karta koloda[]);
void tasovat(karta koloda[]);

#endif