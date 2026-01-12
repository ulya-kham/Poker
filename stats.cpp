// stats.cpp
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include "ui.h"
#include <windows.h>
#include "stats.h"
#include "profile.h"

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
        cout << "\n\t" << Color::red << " Не удалось сохранить статистику." << Color::reset << "\n";
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
    cout << "\n\t" << Color::cyan << "=============================================\n";
    cout << "\t\t    С Т А Т И С Т И К А\n";
    cout << "\t\t" << Color::green << "      Игрока \"" << imyaTekushchegoIgroka << "\" " << Color::reset << "\n";
    cout << "\t" << Color::cyan << "=============================================" << Color::reset << "\n\n";

    if (stats.igry == 0) {
        cout << "\t" << Color::yellow << "Вы ещё не сыграли ни одной руки!" << Color::reset << "\n\n";
    }
    else {
        cout << "\tВсего сыграно рук:     " << stats.igry << "\n";
        cout << "\t" << Color::green << "Побед:                 " << stats.pobedy << "\n";
        cout << "\t" << Color::red << "Поражений:             " << stats.porazheniya << "\n";
        cout << "\t" << Color::yellow << "Суммарный выигрыш:     " << stats.vyigrysh << " фишек\n";
        cout << "\t" << Color::red << "Суммарный проигрыш:    " << stats.proigrish << " фишек" << Color::reset << "\n";

        double winrate = (stats.igry > 0) ? (stats.pobedy * 100.0 / stats.igry) : 0.0;
        cout << "\tПроцент побед:         " << fixed << winrate << "%\n\n";
    }

    moyaPauza();
}

// Достижения: загружает из save_ИМЯ.txt
void pokazatDostizheniyaIzFila(const char* imyaIgroka) {
    char imyaFila[100];
    poluchitImyaFilaSohraneniya(imyaIgroka, imyaFila);

    ifstream file(imyaFila);
    if (!file.is_open()) {
        system("cls");
        cout << Color::blue << "\n\tДостижений" << Color::yellow << " пока нет. Сыграйте хотя бы одну руку!" << Color::reset << "\n";
        moyaPauza();
        return;
    }

    string line;
    getline(file, line); // дата
    getline(file, line); // пустая

    for (int i = 0; i < 4; i++) {
        if (!getline(file, line)) {
            file.close();
            system("cls");
            cout << Color::yellow << "\n\tДостижений пока нет." << Color::reset << "\n";
            moyaPauza();
            return;
        }
    }
    getline(file, line); // пустая строка перед достижениями

    struct Dostizheniya {
        bool novichok, udachlivyi, bankrotSpasitel, korolKombinaciy, millioner;
        int podryadPobed;
    } d;

    file >> d.novichok >> d.udachlivyi >> d.bankrotSpasitel
        >> d.korolKombinaciy >> d.millioner >> d.podryadPobed;
    file.close();

    system("cls");
    cout << "\n\t" << Color::blue << "=============================================\n";
    cout << "\t\t\tДОСТИЖЕНИЯ\n";
    cout << "\t=============================================" << Color::reset << "\n\n";


    // Обыкновенные
    cout << Color::cyan << "\n\tОБЫКНОВЕННЫЕ:\n" << Color::reset;
    cout << "\t  [ " << (d.novichok ? "+" : " ") << " ] Новичок — сыграть первую руку\n";
    cout << "\t  [ " << (d.udachlivyi ? "+" : " ") << " ] Удачливый — 5 побед подряд\n";
    cout << "\t  [ " << (d.bankrotSpasitel ? "+" : " ") << " ] Банкрот-спаситель — погасить долг\n";

    // Нестандартные
    cout << Color::green << "\n\tНЕСТАНДАРТНЫЕ:\n" << Color::reset;
    cout << "\t  [ " << (d.korolKombinaciy ? "+" : " ") << " ] Король комбинаций — каре или выше\n";

    // Редкие
    cout << Color::blue << "\n\tРЕДКИЕ:\n" << Color::reset;
    cout << "\t  [ " << (d.millioner ? "+" : " ") << " ] Миллионер — накопить 1000+ фишек\n";

    moyaPauza();
}