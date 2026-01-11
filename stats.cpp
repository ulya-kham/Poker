#define _CRT_SECURE_NO_WARNINGS
#include "stats.h"
#include "profile.h"  // ? для получения имени файла
#include <iostream>
#include <fstream>
#include <string>
#include "ui.h"
#include <windows.h>
using namespace std;

struct Statistika {
    int igry = 0;
    int pobedy = 0;
    int porazheniya = 0;
    long long vyigrysh = 0;
    long long proigrish = 0;
} stats;

static char imyaTekushchegoIgroka[50] = { 0 };

// Установить текущего игрока (вызывается один раз при старте)
void ustanovitIgrokaDlyaStatistiki(const char* imya) {
    if (imya) {
        strcpy(imyaTekushchegoIgroka, imya);
        zagruzitStatistiku();
    }
}

void zagruzitStatistiku() {
    if (strlen(imyaTekushchegoIgroka) == 0) return;

    char imyaFila[100];
    poluchitImyaFilaStatistiki(imyaTekushchegoIgroka, imyaFila);

    ifstream file(imyaFila);
    if (!file.is_open()) {
        stats = Statistika(); // сброс до нуля
        return;
    }

    string key;
    int value;
    bool ok = true;

    for (int i = 0; i < 5; i++) {
        if (!(file >> key >> value)) {
            ok = false;
            break;
        }
        if (key == "igry:") stats.igry = value;
        else if (key == "pobedy:") stats.pobedy = value;
        else if (key == "porazheniya:") stats.porazheniya = value;
        else if (key == "vyigrysh:") stats.vyigrysh = value;
        else if (key == "proigrish:") stats.proigrish = value;
        else {
            ok = false;
            break;
        }
    }
    file.close();

    if (!ok) {
        stats = Statistika();
    }
}

void sohranitStatistiku() {
    if (strlen(imyaTekushchegoIgroka) == 0) return;

    char imyaFila[100];
    poluchitImyaFilaStatistiki(imyaTekushchegoIgroka, imyaFila);

    ofstream file(imyaFila);
    if (!file.is_open()) {
        cout << "\n\t??  Не удалось сохранить статистику.\n";
        return;
    }
    file << "igry: " << stats.igry << "\n";
    file << "pobedy: " << stats.pobedy << "\n";
    file << "porazheniya: " << stats.porazheniya << "\n";
    file << "vyigrysh: " << stats.vyigrysh << "\n";
    file << "proigrish: " << stats.proigrish << "\n";
    file.close();
}

void obnovitStatistiku(bool pobeda, int summa) {
    if (summa < 0) return;
    if (strlen(imyaTekushchegoIgroka) == 0) return;

    stats.igry++;
    if (pobeda) {
        stats.pobedy++;
        stats.vyigrysh += summa;
    }
    else {
        stats.porazheniya++;
        stats.proigrish += summa;
    }
    sohranitStatistiku();
}

void pokazatStatistiku() {
    system("cls");
    cout << "\n\t=============================================\n";
    cout << "\t                С Т А Т И С Т И К А\n";
    cout << "\t               Игрок: " << imyaTekushchegoIgroka << "\n";
    cout << "\t=============================================\n\n";

    if (stats.igry == 0) {
        cout << "\tВы ещё не сыграли ни одной руки.\n\n";
    }
    else {
        cout << "\tВсего сыграно рук:     " << stats.igry << "\n";
        cout << "\tПобед:                 " << stats.pobedy << "\n";
        cout << "\tПоражений:             " << stats.porazheniya << "\n";
        cout << "\tСуммарный выигрыш:     " << stats.vyigrysh << " фишек\n";
        cout << "\tСуммарный проигрыш:    " << stats.proigrish << " фишек\n";

        double winrate = (stats.igry > 0) ? (stats.pobedy * 100.0 / stats.igry) : 0.0;
        cout << "\tПроцент побед:         " << fixed << winrate << "%\n\n";
    }

    moyaPauza();
}