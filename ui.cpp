// ui.cpp
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#include <windows.h>        // обязательно вызываем первым!
#include <iostream>
#include <cstring>
#include <string>
#include <limits>
#include "ui.h"
#include "karta.h"

using namespace std;

// Вспомогательные функции из karta.cpp
extern const char* znachToStr(int z);
extern const char* mastToSym(char m);

void moyaPauza() {
    cout << Color::yellow << "\n\tДля продолжения нажмите Enter... " << Color::reset;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очищаем буфер
    cin.get(); // ждём один символ (Enter)
}

// Отрисовка карты
void pokazatKartu(const karta& k, bool zakryta) {
    if (zakryta) {
        cout << "\t" << Color::gray << " +---------+\n";
        cout << "\t|??       |\n";
        cout << "\t|         |\n";
        cout << "\t|   ???   |\n";
        cout << "\t|         |\n";
        cout << "\t|       ??|\n";
        cout << "\t+---------+" << Color::reset << "\n";
        return;
    }

    const char* zn = znachToStr(k.znach);
    const char* m = mastToSym(k.mast);

    cout << "\t+---------+\n";
    if (k.znach == 10) {
        cout << "\t|10       |\n";
    }
    else {
        cout << "\t|" << zn << "        |\n";
    }

    int len = strlen(m);
    int spaces = (9 - len) / 2;
    cout << "\t|";
    for (int i = 0; i < spaces; i++) cout << " ";
    cout << m;
    for (int i = 0; i < 9 - len - spaces; i++) cout << " ";
    cout << "|\n";

    if (k.znach == 10) {
        cout << "\t|       10|\n";
    }
    else {
        cout << "\t|        " << zn << "|\n";
    }
    cout << "\t+---------+\n";
}

void pokazatRuku(karta ruka[], int kolvo) {
    if (kolvo <= 0) return;

    const char* znachStr[5];
    const char* mastStr[5];
    for (int i = 0; i < kolvo; i++) {
        znachStr[i] = znachToStr(ruka[i].znach);
        mastStr[i] = mastToSym(ruka[i].mast);
    }

    for (int i = 0; i < kolvo; i++) cout << "\t+---------+  ";
    cout << "\n";

    for (int i = 0; i < kolvo; i++) {
        if (ruka[i].znach == 10) cout << "\t|10       |  ";
        else cout << "\t|" << znachStr[i] << "        |  ";
    }
    cout << "\n";

    for (int i = 0; i < kolvo; i++) cout << "\t|         |  ";
    cout << "\n";

    for (int i = 0; i < kolvo; i++) {
        const char* m = mastStr[i];
        int len = strlen(m);
        int leftSpaces = (9 - len) / 2;
        int rightSpaces = 9 - len - leftSpaces;
        cout << "\t|";
        for (int j = 0; j < leftSpaces; j++) cout << " ";
        cout << m;
        for (int j = 0; j < rightSpaces; j++) cout << " ";
        cout << "|  ";
    }
    cout << "\n";

    for (int i = 0; i < kolvo; i++) cout << "\t|         |  ";
    cout << "\n";

    for (int i = 0; i < kolvo; i++) {
        if (ruka[i].znach == 10) cout << "\t|       10|  ";
        else cout << "\t|        " << znachStr[i] << "|  ";
    }
    cout << "\n";

    for (int i = 0; i < kolvo; i++) cout << "\t+---------+  ";
    cout << "\n\n";
}

void pokazatStol(karta stol[], int otkr) {
    cout << "\t===============\n\n";

    for (int i = 0; i < 5; i++) cout << "\t+---------+  ";
    cout << "\n";

    for (int i = 0; i < otkr; i++) {
        const char* zn = znachToStr(stol[i].znach);
        if (stol[i].znach == 10) cout << "\t|10       |  ";
        else cout << "\t|" << zn << "        |  ";
    }
    for (int i = otkr; i < 5; i++) cout << "\t|??       |  ";
    cout << "\n";

    for (int i = 0; i < 5; i++) cout << "\t|         |  ";
    cout << "\n";

    for (int i = 0; i < otkr; i++) {
        const char* m = mastToSym(stol[i].mast);
        int len = strlen(m);
        int spaces = (9 - len) / 2;
        cout << "\t|";
        for (int j = 0; j < spaces; j++) cout << " ";
        cout << m;
        for (int j = 0; j < 9 - len - spaces; j++) cout << " ";
        cout << "|  ";
    }
    for (int i = otkr; i < 5; i++) cout << "\t|   ???   |  ";
    cout << "\n";

    for (int i = 0; i < 5; i++) cout << "\t|         |  ";
    cout << "\n";

    for (int i = 0; i < otkr; i++) {
        const char* zn = znachToStr(stol[i].znach);
        if (stol[i].znach == 10) cout << "\t|       10|  ";
        else cout << "\t|        " << zn << "|  ";
    }
    for (int i = otkr; i < 5; i++) cout << "\t|       ??|  ";
    cout << "\n";

    for (int i = 0; i < 5; i++) cout << "\t+---------+  ";
    cout << "\n\n";
}

void pokazatPravila() {
    system("cls");
    cout << "\n\t   " << Color::green << "==========================================================\n";
    cout << "\t                       ПРАВИЛА ИГРЫ\n";
    cout << "\t   ==========================================================" << Color::reset << "\n";
    cout << "\n";

    cout << "\t   " << Color::yellow << "Техасский Холдем" << Color::reset << " — самая популярная разновидность покера.\n\n";

    cout << "\t   У вас на руках 2 " << Color::gray << "закрытые карты." << Color::reset << "\n";
    cout << "\t   На стол выкладываются 5 общих карт \n";
    cout << "\t   (сначала три (флоп), затем ещё одна (тёрн) и последняя (ривер)).\n";
    cout << "\t   Ваша цель — собрать лучшую комбинацию из 5 карт,\n";
    cout << "\t   используя любые из своих и общих.\n\n";

    cout << "\t   Ставки делаются по кругу. Вы можете:\n";
    cout << "\t   [0] — сдаться (пас),\n";
    cout << "\t   [x] — уравнять ставку (колл),\n";
    cout << "\t   [>x] — повысить (рейз),\n";
    cout << "\t   [ALL-IN] — поставить все фишки.\n\n";

    cout << "\t   Комбинации (от сильной к слабой):\n";
    cout << "\t   1. Роял-флеш (10-J-Q-K-A в одной масти) - это сильнейшая!\n";
    cout << "\t   2. Стрит-флеш (стрит в одной масти)\n";
    cout << "\t   3. Каре (четыре одинаковых)\n";
    cout << "\t   4. Фулл-хаус (тройка + пара)\n";
    cout << "\t   5. Флеш (одна масть)\n";
    cout << "\t   6. Стрит (5 подряд)\n";
    cout << "\t   7. Тройка\n";
    cout << "\t   8. Две пары\n";
    cout << "\t   9. Пара\n";
    cout << "\t  10. Старшая карта\n\n";
    cout << "\t " << Color::green << "Удачи за игровым столом!" << Color::reset << "\n";
    moyaPauza();
}

void konetsIgry() {
    system("cls");
    cout << "\n\t" << Color::red << "==================================================" << Color::reset << "\n";
    cout << "\t              " << Color::green << "СПАСИБО ЗА ИГРУ!" << Color::reset << "\n";
    cout << "\t" << Color::red << "==================================================" << Color::reset << "\n\n";

    cout << "\t   До новых встреч за покерным столом!\n\n\n\n\n";
}