// input.cpp
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include "input.h"
#include <iostream>
#include <limits>
using namespace std;

int bezopasnyVvodChisla(int minZnach, int maxZnach) {
    int chislo;
    while (true) {
        cin >> chislo;
        if (cin.fail() || chislo < minZnach || chislo > maxZnach) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\tќшибка! ¬ведите число от " << minZnach << " до " << maxZnach << ": ";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return chislo;
        }
    }
}

// input.cpp
int sdelatStavku(int fichki, int minStavka) {
    while (true) {
        if (minStavka == 0) {
            cout << "\t¬ведите ставку (0 Ч пас): ";
        }
        else {
            cout << "\tћинимальна€ ставка: " << minStavka << " фишек. ¬ведите ставку (0 Ч пас): ";
        }

        int stavka;
        cin >> stavka;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\tќшибка: введите число.\n";
            continue;
        }

        if (stavka == 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return -1; // пас
        }

        if (stavka > fichki) {
            cout << "\n\t” вас только " << fichki << " фишек!\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return stavka; // люба€ ставка от 1 до fichki разрешена
    }
}