// profile.cpp
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <windows.h>
#include "profile.h"
#include "ui.h"

using namespace std;

const char* IMYA_FILA_SPISKA = "poker_profiles.list";

// Вспомогательная функция: обрезать пробелы
void obrezatProbely(char* str) {
    int len = strlen(str);
    if (len == 0) return;

    int start = 0;
    while (start < len && str[start] == ' ') start++;
    int end = len - 1;
    while (end >= 0 && str[end] == ' ') end--;

    if (start > end) {
        str[0] = '\0';
        return;
    }

    for (int i = start; i <= end; i++) {
        str[i - start] = str[i];
    }
    str[end - start + 1] = '\0';
}

// Получить список имён из файла
int poluchitSpisokImyon(char spisok[][50], int maxProfilей) {
    ifstream file(IMYA_FILA_SPISKA);
    int kolvo = 0;
    if (!file.is_open()) return 0;

    while (kolvo < maxProfilей && file.getline(spisok[kolvo], 50)) {
        obrezatProbely(spisok[kolvo]);
        if (strlen(spisok[kolvo]) > 0) {
            kolvo++;
        }
    }
    file.close();
    return kolvo;
}

// Добавить имя в список (если ещё не существует)
bool dobavitImyaVSpisok(const char* imya) {
    if (!imya || strlen(imya) == 0) return false;

    // Проверим, нет ли уже такого имени
    char spisok[20][50];
    int kolvo = poluchitSpisokImyon(spisok, 20);
    for (int i = 0; i < kolvo; i++) {
        if (strcmp(spisok[i], imya) == 0) {
            return true; // уже есть
        }
    }

    // Добавляем
    ofstream file(IMYA_FILA_SPISKA, ios::app);
    if (!file.is_open()) return false;
    file << imya << "\n";
    file.close();
    return true;
}

// Создать новый профиль
const char* sozdatNovyiProfil() {
    static char imya[50];
    system("cls");
    cout << "\n\t" << Color::cyan << "=============================================\n";
    cout << "\t      СОЗДАНИЕ НОВОГО ПРОФИЛЯ\n";
    cout << "\t=============================================" << Color::reset << "\n\n";
    cout << "\tВведите ваше имя (длинной до 49 символов):\n\t";

    cin.getline(imya, 50);
    obrezatProbely(imya);

    if (strlen(imya) == 0) {
        strcpy(imya, "Игрок");
    }

    // Ограничим длину до 30 символов (чтобы имена файлов не ломались)
    if (strlen(imya) > 30) {
        imya[30] = '\0';
    }

    dobavitImyaVSpisok(imya);

    // Пасхалка для Насти
    if (strcmp(imya, "Настя") == 0 || strcmp(imya, "Морковка") == 0) {
        cout << "\n\tПривет, " << Color::yellow << "Морковка" << Color::reset << "! Удачи на парах!\n";
        cout << "\tПусть билеты будут лёгкими, а кавалеры — щедрыми!\n";
        moyaPauza();
    }
    else {
        cout << "\n\t" << Color::green << "Профиль \"" << imya << "\" успешно создан!" << Color::reset << "\n";
        moyaPauza();
    }

    return imya;
}

// Выбрать существующий профиль
const char* vybratSushchestvuyushchiyProfil() {
    char spisok[20][50];
    int kolvo = poluchitSpisokImyon(spisok, 20);

    if (kolvo == 0) {
        return sozdatNovyiProfil();
    }

    system("cls");
    cout << "\n\t" << Color::cyan << "=============================================\n";
    cout << "\t\t\tВЫБЕРИТЕ ИГРОКА\n";
    cout << "\t=============================================" << Color::reset << "\n\n";

    for (int i = 0; i < kolvo; i++) {
       cout << "\t" << (i + 1) << ". " << Color::cyan << spisok[i] << "" << Color::reset << "\n";
    }

    cout << "\n\t" << (kolvo + 1) << ". " << Color::green << "Создать новый профиль" << Color::reset << "\n";
    cout << "\t" << (kolvo + 2) << ". " << Color::gray << "Выйти" << Color::reset << "\n";
    cout << "\n\tВаш выбор: ";

    int vybor;
    while (!(cin >> vybor) || vybor < 1 || vybor > kolvo + 2) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n\t" << Color::red << "Неверный выбор. " << Color::yellow << "Попробуйте снова: " << Color::reset << "";
    }

    cin.ignore(); // очистить буфер после cin >>

    if (vybor == kolvo + 2) {
        exit(0); // выход
    }
    if (vybor == kolvo + 1) {
        return sozdatNovyiProfil();
    }

    static char vybrannoeImya[50];
    strcpy(vybrannoeImya, spisok[vybor - 1]);
    return vybrannoeImya;
}

// Получить имя файла сохранения
void poluchitImyaFilaSohraneniya(const char* imyaIgroka, char rezultat[100]) {
    strcpy(rezultat, "save_");
    strcat(rezultat, imyaIgroka);
    strcat(rezultat, ".txt");
}

// Получить имя файла статистики
void poluchitImyaFilaStatistiki(const char* imyaIgroka, char rezultat[100]) {
    strcpy(rezultat, "stats_");
    strcat(rezultat, imyaIgroka);
    strcat(rezultat, ".txt");
}