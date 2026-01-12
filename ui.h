// ui.h
#pragma once

#include <iostream>
#include "karta.h"


// ÷вета через ANSI-коды (работают в современных консол€х)
namespace Color {
    inline const char* reset = "\033[0m";           // сброс цвета
    inline const char* red = "\033[31m";            // красный
    inline const char* green = "\033[32m";          // зелЄный
    inline const char* yellow = "\033[33m";         // жЄлтый
    inline const char* blue = "\033[34m";           // синий
    inline const char* cyan = "\033[36m";           // циановый
    inline const char* white = "\033[37m";          // белый
    inline const char* gray = "\033[90m";           // серый (€ркий чЄрный)
};


void moyaPauza();
int pokazatGlavnoeMenyu();
void pokazatPravila();
void konetsIgry();
void pokazatRuku(karta ruka[], int kol);
void pokazatStol(karta stol[], int otkryto);
