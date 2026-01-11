// main.cpp
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <iostream>
#include <windows.h>
#include <ctime>
#include <fstream>
#include <string>
#include <limits> // ← ДОБАВЛЕНО для numeric_limits
#include "ui.h"
#include "game.h"


using namespace std;

struct Dostizheniya {
    bool novichok = false;
    bool udachlivyi = false;
    bool bankrotSpasitel = false;
    bool korolKombinaciy = false;
    bool millioner = false;
    int podryadPobed = 0;
};

void pokazatDostizheniyaIzFila() {
    ifstream file("poker_save.txt");
    if (!file.is_open()) {
        cout << "\n\tДостижений пока нет.\n";
        return;
    }

    string line;
    for (int i = 0; i < 5; i++) {
        if (!getline(file, line)) {
            file.close();
            cout << "\n\tДостижений пока нет.\n";
            return;
        }
    }

    Dostizheniya d;
    file >> d.novichok >> d.udachlivyi >> d.bankrotSpasitel
        >> d.korolKombinaciy >> d.millioner >> d.podryadPobed;
    file.close();

    cout << "\n\t🏆 ВАШИ ДОСТИЖЕНИЯ:\n";
    cout << "\t[ " << (d.novichok ? "✓" : " ") << " ] Новичок — сыграть первую руку\n";
    cout << "\t[ " << (d.udachlivyi ? "✓" : " ") << " ] Удачливый — 5 побед подряд\n";
    cout << "\t[ " << (d.bankrotSpasitel ? "✓" : " ") << " ] Банкрот-спаситель — погасить долг 50 фишек\n";
    cout << "\t[ " << (d.korolKombinaciy ? "✓" : " ") << " ] Король комбинаций — собрать каре или выше\n";
    cout << "\t[ " << (d.millioner ? "✓" : " ") << " ] Миллионер — накопить 1000+ фишек\n";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(0)));

    while (true) {
        system("cls");
        cout << "\n\t=============================================\n";
        cout << "\t        🃏 ПОКЕР: ТЕХАССКИЙ ХОЛДЕМ 🃏\n";
        cout << "\t=============================================\n\n";
        cout << "\t1. Начать новую игру\n";
        cout << "\t2. Загрузить сохранение\n";
        cout << "\t3. Достижения\n";
        cout << "\t4. Правила игры\n";
        cout << "\t5. Выход\n";
        cout << "\n\tВаш выбор: ";

        int vybor;
        // Безопасный ввод
        while (!(cin >> vybor) || vybor < 1 || vybor > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\tНеверный выбор. Введите число от 1 до 5: ";
        }

        switch (vybor) {
        case 1:
            system("cls");
            cout << "\n\tЗапуск новой игры...\n";
            system("pause");
            zapustitIgru(false);
            break;

        case 2: {
            ifstream test("poker_save.txt");
            if (!test.good()) {
                cout << "\n\tСохранение не найдено!\n";
                system("pause");
                break;
            }
            test.close();

            system("cls");
            cout << "\n\tЗагрузка сохранённой игры...\n";
            system("pause");
            zapustitIgru(true);
            break;
        }

        case 3:
            pokazatDostizheniyaIzFila();
            system("pause");
            break;

        case 4:
            pokazatPravila();
            break;

        case 5:
            konetsIgry();
            return 0;
        }
    }
}