// main.cpp
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#include <iostream>
#include <windows.h>
#include <ctime>
#include <limits>
#include "ui.h"
#include "game.h"
#include "profile.h"
#include "stats.h"

using namespace std;


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(0)));

    // ВЫБОР ПРОФИЛЯ
    const char* tekushchiyIgrOk = vybratSushchestvuyushchiyProfil();

    // ИНИЦИАЛИЗАЦИЯ СТАТИСТИКИ ДЛЯ ЭТОГО ИГРОКА
    ustanovitIgrokaDlyaStatistiki(tekushchiyIgrOk);

    while (true) {
        system("cls");
        cout << "\n\t" << Color::yellow << "=============================================" << Color::reset << "\n";
        cout << "\t           " << Color::yellow << "ПОКЕР: ТЕХАССКИЙ ХОЛДЕМ " << Color::reset << "\n";
        cout << "\t            Добро пожаловать, "<< Color::cyan << tekushchiyIgrOk << Color::reset << "!\n";
        cout << "\t" << Color::yellow << "=============================================" << Color::reset << "\n\n";
        cout << "\t1. Начать новую игру\n";
        cout << "\t2. " << Color::blue << "Достижения" << Color::reset << "\n";
        cout << "\t3. " << Color::green << "Правила игры" << Color::reset << "\n";
        cout << "\t4. " << Color::cyan << "Статистика" << Color::reset << "\n";
        cout << "\t5. " << Color::red << "Выход" << Color::reset << "\n";
        cout << "\n\tВаш выбор: ";

        int vybor;
        while (!(cin >> vybor) || vybor < 1 || vybor > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t" << Color::red << "Неверный выбор. " << Color::yellow << "Введите число от 1 до 5: " << Color::reset << "";
        }

        switch (vybor) {
        case 1:
            zapustitIgru(false, tekushchiyIgrOk); // передаём имя
            break;

        case 2:
            pokazatDostizheniyaIzFila(tekushchiyIgrOk);
            break;

        case 3:
            pokazatPravila();
            break;

        case 4:
            pokazatStatistiku();
            break;

        case 5:
            konetsIgry();
            return 0;
        }
    }
}