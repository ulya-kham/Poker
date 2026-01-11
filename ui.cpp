#define _CRT_SECURE_NO_WARNINGS
#include "ui.h"
#include "karta.h"
#include <iostream>
#include <cstring>
using namespace std;

// Вспомогательные функции из karta.cpp
extern const char* znachToStr(int z);
extern const char* mastToSym(char m);

void pokazatKartu(const karta& k, bool zakryta) {
    if (zakryta) {
        cout << "\t+---------+\n";
        cout << "\t|??       |\n";
        cout << "\t|         |\n";
        cout << "\t|   ???   |\n";
        cout << "\t|         |\n";
        cout << "\t|       ??|\n";
        cout << "\t+---------+\n";
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

// Остальные функции (меню, правила, конец игры) — без изменений
// Просто оставь их как есть в своём файле


// Главное меню
int pokazatGlavnoeMenyu() {
    system("cls");
    cout << "\n\t=============================================\n";
    cout << "\t          ПОКЕР: ТЕХАССКИЙ ХОЛДЕМ  \n";
    cout << "\t=============================================\n\n";
    cout << "\t1. Начать новую игру\n";
    cout << "\t2. Загрузить сохранение\n";
    cout << "\t3. Достижения\n";
    cout << "\t4. Правила игры\n";
    cout << "\t5. Выход\n";
    cout << "\n\tВаш выбор: ";

    int vybor;
    while (!(cin >> vybor) || vybor < 1 || vybor > 5) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n\tНеверный выбор. Введите число от 1 до 5: ";
    }
    return vybor;
}

// Правила
void pokazatPravila() {
    system("cls");
    cout << "\n\t=============================================\n";
    cout << "\t               ПРАВИЛА ИГРЫ\n";
    cout << "\t=============================================\n\n";
    cout << "\tТехасский Холдем — самая популярная разновидность покера.\n";
    cout << "\tУ каждого игрока 2 закрытые карты. На стол выкладываются 5 общих.\n";
    cout << "\tЦель — собрать лучшую комбинацию из 5 карт (используя свои и общие).\n";
    cout << "\tСтавки делаются по кругу. Можно: пасовать, уравнять, повысить или пойти ALL-IN.\n";
    cout << "\n\tКомбинации (от слабой к сильной):\n";
    cout << "\t- Старшая карта\n\t- Пара\n\t- Две пары\n\t- Тройка\n\t- Стрит\n\t- Флеш\n\t- Фулл-хаус\n\t- Каре\n\t- Стрит-флеш\n\t- Роял-флеш\n";
    cout << "\n\tУдачи за игровым столом!\n";
    system("pause");
}

// Конец игры
void konetsIgry() {
    system("cls");
    cout << "\n\tСпасибо за игру!\n";
    cout << "\tДо новых встреч за покерным столом!\n\n";
    system("pause");
}