// game.cpp
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include "game.h"
#include "ui.h"
#include "karta.h"
#include "kombinacii.h"
#include "input.h"
#include "computer.h"
#include <iostream>
#include <algorithm>
#include <fstream>

#include <string>
using namespace std;

const int START_FICHI = 100;
const int MALYI_BLIND = 5;
const int BOLSHOI_BLIND = 10;
const int MAX_DOLG = 50;
const int POSLEDNIY_SHANS = 10;

// Структура для достижений
struct Dostizheniya {
    bool novichok = false;          // 1
    bool udachlivyi = false;        // 2
    bool bankrotSpasitel = false;   // 3
    bool korolKombinaciy = false;   // 4
    bool millioner = false;         // 5
    int podryadPobed = 0;           // счётчик побед подряд
};

// Сохранение игры + достижений
bool sohranitIgru(int fichkiIgroka, int dolg, int fichkiComp, bool poslednyShansIsPolzovan, const Dostizheniya& d) {
    ofstream file("poker_save.txt");
    if (!file.is_open()) {
        cout << "\n\tОшибка: не удалось сохранить игру.\n";
        return false;
    }
    file << fichkiIgroka << "\n";
    file << dolg << "\n";
    file << fichkiComp << "\n";
    file << poslednyShansIsPolzovan << "\n\n";
    file << d.novichok << "\n";
    file << d.udachlivyi << "\n";
    file << d.bankrotSpasitel << "\n";
    file << d.korolKombinaciy << "\n";
    file << d.millioner << "\n";
    file << d.podryadPobed;
    file.close();
    cout << "\n\tИгра и достижения сохранены!\n";
    return true;
}

// Загрузка игры + достижений
bool zagruzitIgru(int& fichkiIgroka, int& dolg, int& fichkiComp, bool& poslednyShansIsPolzovan, Dostizheniya& d) {
    ifstream file("poker_save.txt");
    if (!file.is_open()) return false;
    file >> fichkiIgroka >> dolg >> fichkiComp >> poslednyShansIsPolzovan;
    file.ignore(1000, '\n'); // пропустить пустую строку
    file >> d.novichok >> d.udachlivyi >> d.bankrotSpasitel >> d.korolKombinaciy >> d.millioner >> d.podryadPobed;
    file.close();
    return true;
}

// Вывод достижений
void pokazatDostizheniya(const Dostizheniya& d) {
    cout << "\n\t?? ВАШИ ДОСТИЖЕНИЯ:\n";
    cout << "\t[ " << (d.novichok ? "?" : " ") << " ] Новичок — сыграть первую руку\n";
    cout << "\t[ " << (d.udachlivyi ? "?" : " ") << " ] Удачливый — 5 побед подряд\n";
    cout << "\t[ " << (d.bankrotSpasitel ? "?" : " ") << " ] Банкрот-спаситель — погасить долг 50 фишек\n";
    cout << "\t[ " << (d.korolKombinaciy ? "?" : " ") << " ] Король комбинаций — собрать каре или выше\n";
    cout << "\t[ " << (d.millioner ? "?" : " ") << " ] Миллионер — накопить 1000+ фишек\n";
}

// Проверка и разблокировка достижений
void proveritDostizheniya(Dostizheniya& d, int fichkiIgroka, int dolg, int resultatRuki, const string& kombinaciya) {
    // 1. Новичок
    if (!d.novichok) {
        d.novichok = true;
        cout << "\n\t?? НОВОЕ ДОСТИЖЕНИЕ: Новичок!\n";
    }

    // 2. Удачливый
    if (resultatRuki == 1) {
        d.podryadPobed++;
        if (d.podryadPobed >= 5 && !d.udachlivyi) {
            d.udachlivyi = true;
            cout << "\n\t?? НОВОЕ ДОСТИЖЕНИЕ: Удачливый!\n";
        }
    }
    else {
        d.podryadPobed = 0;
    }

    // 4. Король комбинаций
    if (!d.korolKombinaciy) {
        if (kombinaciya.find("Каре") != string::npos ||
            kombinaciya.find("Стрит-флеш") != string::npos ||
            kombinaciya.find("Роял-флеш") != string::npos) {
            d.korolKombinaciy = true;
            cout << "\n\t?? НОВОЕ ДОСТИЖЕНИЕ: Король комбинаций!\n";
        }
    }

    // 5. Миллионер
    if (fichkiIgroka >= 1000 && !d.millioner) {
        d.millioner = true;
        cout << "\n\t?? НОВОЕ ДОСТИЖЕНИЕ: Миллионер!\n";
    }
}

bool vzyatKredit(int& fichki, int& dolg) {
    if (dolg > 0) {
        cout << "\n\tУ вас уже есть долг перед банком (" << dolg << " фишек). Сначала погасите его!\n";
        return false;
    }
    if (fichki > 10) {
        cout << "\n\tКредит доступен только при балансе ? 10 фишек.\n";
        return false;
    }
    cout << "\n\tБанк предлагает кредит до " << MAX_DOLG << " фишек.\n";
    cout << "\tСколько хотите взять? (1–" << MAX_DOLG << ", 0 — отказ): ";
    int summa = bezopasnyVvodChisla(0, MAX_DOLG);
    if (summa == 0) {
        cout << "\n\tВы отказались от кредита.\n";
        return false;
    }
    fichki += summa;
    dolg = summa;
    cout << "\n\tВы взяли кредит на " << summa << " фишек. Долг: " << dolg << " фишек.\n";
    return true;
}

// ОСНОВНАЯ ФУНКЦИЯ — принимает флаг загрузки
void zapustitIgru(bool zagruzitIzFila) {
    system("cls");

    // Инициализация по умолчанию (новая игра)
    int fichkiIgroka = START_FICHI;
    int fichkiComp = 1000;
    int dolg = 0;
    bool poslednyShansIsPolzovan = false;
    Dostizheniya dostizh;

    // Если нужно загрузить — читаем из файла
    if (zagruzitIzFila) {
        ifstream testFile("poker_save.txt");
        if (testFile.good()) {
            if (zagruzitIgru(fichkiIgroka, dolg, fichkiComp, poslednyShansIsPolzovan, dostizh)) {
                cout << "\n\t? Сохранение успешно загружено!\n";
                pokazatDostizheniya(dostizh);
                system("pause");
            }
            else {
                cout << "\n\t? Ошибка загрузки. Начинаем новую игру.\n";
                system("pause");
                // остаются значения по умолчанию
            }
        }
        else {
            cout << "\n\t? Файл сохранения не найден. Начинаем новую игру.\n";
            system("pause");
        }
        testFile.close();
    }

    // Основной игровой цикл
    while (fichkiIgroka >= 0 && fichkiComp > 0) {
        // Проверка на банкротство с долгом
        if (fichkiIgroka <= 0 && dolg > 0) {
            if (!poslednyShansIsPolzovan) {
                cout << "\n\tУ вас закончились фишки, но остался долг (" << dolg << " фишек)!\n";
                cout << "\tБанк даёт вам ПОСЛЕДНИЙ ШАНС: " << POSLEDNIY_SHANS << " фишек.\n";
                cout << "\tЕсли проиграете эту руку — игра окончена.\n";
                cout << "\tПринять последний шанс? (1 — да, 0 — нет): ";
                int vybor = bezopasnyVvodChisla(0, 1);
                if (vybor == 1) {
                    fichkiIgroka = POSLEDNIY_SHANS;
                    dolg += POSLEDNIY_SHANS;
                    poslednyShansIsPolzovan = true;
                    cout << "\n\tВы получили " << POSLEDNIY_SHANS << " фишек. Новый долг: " << dolg << " фишек.\n\n";
                }
                else {
                    cout << "\n\tВы отказались от последнего шанса.\n";
                    cout << "\tИгра окончена. Банк забирает всё.\n";
                    system("pause");
                    return;
                }
            }
            else {
                cout << "\n\tУ вас нет фишек, долг не погашен, и последний шанс использован.\n";
                cout << "\tВы объявлены банкротом. Игра окончена.\n";
                system("pause");
                return;
            }
        }

        if (fichkiIgroka <= 10 && dolg == 0) {
            cout << "\n\tУ вас мало фишек (" << fichkiIgroka << "). Хотите взять кредит?\n";
            cout << "\t(1 — да, 0 — нет): ";
            int vybor = bezopasnyVvodChisla(0, 1);
            if (vybor == 1) {
                vzyatKredit(fichkiIgroka, dolg);
            }
        }

        cout << "\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "\t~           Н О В А Я   Р У К А             ~\n";
        cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

        karta koloda[KOL_KART];
        sozdatKolodu(koloda);
        tasovat(koloda);
        int poz = 0;

        karta rukaIgr[2], rukaKaz[2], stol[5];
        for (int i = 0; i < 2; i++) {
            rukaIgr[i] = koloda[poz++];
            rukaKaz[i] = koloda[poz++];
        }

        for (int i = 0; i < 5; i++) {
            stol[i].znach = 0;
            stol[i].mast = 'X';
        }

        // Блайнды
        int bank = MALYI_BLIND + BOLSHOI_BLIND;
        if (fichkiIgroka < MALYI_BLIND) {
            bank = fichkiIgroka + BOLSHOI_BLIND;
            fichkiComp -= BOLSHOI_BLIND;
            fichkiIgroka = 0;
        }
        else {
            fichkiIgroka -= MALYI_BLIND;
            fichkiComp -= BOLSHOI_BLIND;
        }

        cout << "\tМалый блайнд: " << MALYI_BLIND << " (от вас)\n";
        cout << "\tБольшой блайнд: " << BOLSHOI_BLIND << " (от компьютера)\n";
        cout << "\tВ банке: " << bank << " фишек\n\n";

        pokazatRuku(rukaIgr, 2);

        bool allIn = false;
        bool rukaZavershena = false;

        // Префлоп
        cout << "\n\t--- П Р Е Ф Л О П ---\n";
        int raznicaDoUrafnivaniya = BOLSHOI_BLIND - MALYI_BLIND;
        int stavkaIgr = 0;

        if (fichkiIgroka > 0) {
            cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
            if (dolg > 0) cout << " (долг: " << dolg << ")";
            cout << "\n";

            stavkaIgr = sdelatStavku(fichkiIgroka, raznicaDoUrafnivaniya);
            if (stavkaIgr == -1) {
                cout << "\n\tВы сдались. Компьютер забирает банк: " << bank << " фишек.\n";
                fichkiComp += bank;
                rukaZavershena = true;
            }
        }

        if (!rukaZavershena && fichkiIgroka > 0) {
            if (raznicaDoUrafnivaniya > 0 && stavkaIgr < raznicaDoUrafnivaniya) {
                cout << "\n\tВы идёте ALL-IN на " << stavkaIgr << " фишек!\n";
                fichkiIgroka -= stavkaIgr;
                bank += stavkaIgr;
                int compDobavka = raznicaDoUrafnivaniya - stavkaIgr;
                fichkiComp -= compDobavka;
                bank += compDobavka;
                rukaZavershena = true;
                allIn = true;
                for (int i = 0; i < 5; i++) stol[i] = koloda[poz++];
                cout << "\n\tВСЕ-ИН! Все карты открыты:\n";
                pokazatStol(stol, 5);
            }
            else {
                fichkiIgroka -= stavkaIgr;
                bank += stavkaIgr;

                int stavkaComp = reshenieComputera(fichkiComp, stavkaIgr, rukaKaz, stol, 0);
                if (stavkaComp > 0 && stavkaComp < stavkaIgr) {
                    stavkaComp = stavkaIgr;
                }

                if (stavkaComp == 0) {
                    cout << "\n\tКомпьютер сбрасывает карты.\n";
                    cout << "\n\tВы выиграли банк: " << bank << " фишек!\n";
                    fichkiIgroka += bank;
                    rukaZavershena = true;
                }
                else if (stavkaComp == stavkaIgr) {
                    cout << "\n\tКомпьютер поддерживает вашу ставку: " << stavkaComp << " фишек.\n";
                    fichkiComp -= stavkaComp;
                    bank += stavkaComp;
                }
                else {
                    cout << "\n\tКомпьютер повышает ставку до " << stavkaComp << " фишек!\n";
                    int dopStavka = stavkaComp - stavkaIgr;

                    cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
                    if (dolg > 0) cout << " (долг: " << dolg << ")";
                    cout << "\n";

                    int otvetIgroka = sdelatStavku(fichkiIgroka, dopStavka);
                    if (otvetIgroka == -1) {
                        cout << "\n\tВы сдались. Компьютер забирает банк.\n";
                        fichkiComp += bank;
                        rukaZavershena = true;
                    }
                    else {
                        if (otvetIgroka < dopStavka) {
                            cout << "\n\tВы идёте ALL-IN на " << otvetIgroka << " фишек!\n";
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;
                            int compDobavka = dopStavka - otvetIgroka;
                            fichkiComp -= compDobavka;
                            bank += compDobavka;
                            rukaZavershena = true;
                            allIn = true;
                            for (int i = 0; i < 5; i++) {
                                if (stol[i].znach == 0) stol[i] = koloda[poz++];
                            }
                            cout << "\n\tВСЕ-ИН! Все карты открыты:\n";
                            pokazatStol(stol, 5);
                        }
                        else {
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;

                            int raznica = otvetIgroka - dopStavka;
                            if (raznica > 0) {
                                int compDobavka = min(raznica, fichkiComp);
                                fichkiComp -= compDobavka;
                                bank += compDobavka;
                                cout << "\n\tКомпьютер уравнивает ваше повышение: +" << compDobavka << " фишек.\n";
                            }
                            else {
                                fichkiComp -= dopStavka;
                                bank += dopStavka;
                            }
                        }
                    }
                }

                if (!allIn && (fichkiIgroka == 0 || fichkiComp == 0)) {
                    allIn = true;
                    for (int i = 0; i < 5; i++) {
                        if (stol[i].znach == 0) stol[i] = koloda[poz++];
                    }
                    cout << "\n\tВСЕ-ИН! Все карты открыты:\n";
                    pokazatStol(stol, 5);
                }
            }
        }

        // === ФЛОП, ТЁРН, РИВЕР — без изменений (сокращено для краткости) ===
        // ... (весь код как в предыдущей версии) ...

        // Флоп
        if (!rukaZavershena && !allIn) {
            for (int i = 0; i < 3; i++) stol[i] = koloda[poz++];
            cout << "\n\t--- Ф Л О П ---\n";
            pokazatStol(stol, 3);

            cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
            if (dolg > 0) cout << " (долг: " << dolg << ")";
            cout << "\n";

            int stavkaIgr = sdelatStavku(fichkiIgroka, 0);
            if (stavkaIgr == -1) {
                cout << "\n\tВы сдались. Компьютер забирает банк.\n";
                fichkiComp += bank;
                rukaZavershena = true;
            }

            if (!rukaZavershena) {
                fichkiIgroka -= stavkaIgr;
                bank += stavkaIgr;

                int stavkaComp = reshenieComputera(fichkiComp, stavkaIgr, rukaKaz, stol, 1);
                if (stavkaComp > 0 && stavkaComp < stavkaIgr) {
                    stavkaComp = stavkaIgr;
                }

                if (stavkaComp == 0) {
                    cout << "\n\tКомпьютер сбрасывает карты.\n";
                    cout << "\n\tВы выиграли банк: " << bank << " фишек!\n";
                    fichkiIgroka += bank;
                    rukaZavershena = true;
                }
                else if (stavkaComp == stavkaIgr) {
                    cout << "\n\tКомпьютер поддерживает вашу ставку: " << stavkaComp << " фишек.\n";
                    fichkiComp -= stavkaComp;
                    bank += stavkaComp;
                }
                else {
                    cout << "\n\tКомпьютер повышает ставку до " << stavkaComp << " фишек!\n";
                    int dopStavka = stavkaComp - stavkaIgr;

                    cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
                    if (dolg > 0) cout << " (долг: " << dolg << ")";
                    cout << "\n";

                    int otvetIgroka = sdelatStavku(fichkiIgroka, dopStavka);
                    if (otvetIgroka == -1) {
                        cout << "\n\tВы сдались. Компьютер забирает банк.\n";
                        fichkiComp += bank;
                        rukaZavershena = true;
                    }
                    else {
                        if (otvetIgroka < dopStavka) {
                            cout << "\n\tВы идёте ALL-IN на " << otvetIgroka << " фишек!\n";
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;
                            int compDobavka = dopStavka - otvetIgroka;
                            fichkiComp -= compDobavka;
                            bank += compDobavka;
                            rukaZavershena = true;
                            allIn = true;
                            for (int i = 3; i < 5; i++) stol[i] = koloda[poz++];
                            cout << "\n\tВСЕ-ИН! Все карты открыты:\n";
                            pokazatStol(stol, 5);
                        }
                        else {
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;

                            int raznica = otvetIgroka - dopStavka;
                            if (raznica > 0) {
                                int compDobavka = min(raznica, fichkiComp);
                                fichkiComp -= compDobavka;
                                bank += compDobavka;
                                cout << "\n\tКомпьютер уравнивает ваше повышение: +" << compDobavka << " фишек.\n";
                            }
                            else {
                                fichkiComp -= dopStavka;
                                bank += dopStavka;
                            }
                        }
                    }
                }

                if (!allIn && (fichkiIgroka == 0 || fichkiComp == 0)) {
                    allIn = true;
                    for (int i = 3; i < 5; i++) stol[i] = koloda[poz++];
                    cout << "\n\tВСЕ-ИН! Все карты открыты:\n";
                    pokazatStol(stol, 5);
                }
            }
        }

        // Тёрн
        if (!rukaZavershena && !allIn) {
            stol[3] = koloda[poz++];
            cout << "\n\t--- Т Ё Р Н ---\n";
            pokazatStol(stol, 4);

            cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
            if (dolg > 0) cout << " (долг: " << dolg << ")";
            cout << "\n";

            int stavkaIgr = sdelatStavku(fichkiIgroka, 0);
            if (stavkaIgr == -1) {
                cout << "\n\tВы сдались. Компьютер забирает банк.\n";
                fichkiComp += bank;
                rukaZavershena = true;
            }

            if (!rukaZavershena) {
                fichkiIgroka -= stavkaIgr;
                bank += stavkaIgr;

                int stavkaComp = reshenieComputera(fichkiComp, stavkaIgr, rukaKaz, stol, 2);
                if (stavkaComp > 0 && stavkaComp < stavkaIgr) {
                    stavkaComp = stavkaIgr;
                }

                if (stavkaComp == 0) {
                    cout << "\n\tКомпьютер сбрасывает карты.\n";
                    cout << "\n\tВы выиграли банк: " << bank << " фишек!\n";
                    fichkiIgroka += bank;
                    rukaZavershena = true;
                }
                else if (stavkaComp == stavkaIgr) {
                    cout << "\n\tКомпьютер поддерживает вашу ставку: " << stavkaComp << " фишек.\n";
                    fichkiComp -= stavkaComp;
                    bank += stavkaComp;
                }
                else {
                    cout << "\n\tКомпьютер повышает ставку до " << stavkaComp << " фишек!\n";
                    int dopStavka = stavkaComp - stavkaIgr;

                    cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
                    if (dolg > 0) cout << " (долг: " << dolg << ")";
                    cout << "\n";

                    int otvetIgroka = sdelatStavku(fichkiIgroka, dopStavka);
                    if (otvetIgroka == -1) {
                        cout << "\n\tВы сдались. Компьютер забирает банк.\n";
                        fichkiComp += bank;
                        rukaZavershena = true;
                    }
                    else {
                        if (otvetIgroka < dopStavka) {
                            cout << "\n\tВы идёте ALL-IN на " << otvetIgroka << " фишек!\n";
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;
                            int compDobavka = dopStavka - otvetIgroka;
                            fichkiComp -= compDobavka;
                            bank += compDobavka;
                            rukaZavershena = true;
                            allIn = true;
                            stol[4] = koloda[poz++];
                            cout << "\n\tВСЕ-ИН! Все карты открыты:\n";
                            pokazatStol(stol, 5);
                        }
                        else {
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;

                            int raznica = otvetIgroka - dopStavka;
                            if (raznica > 0) {
                                int compDobavka = min(raznica, fichkiComp);
                                fichkiComp -= compDobavka;
                                bank += compDobavka;
                                cout << "\n\tКомпьютер уравнивает ваше повышение: +" << compDobavka << " фишек.\n";
                            }
                            else {
                                fichkiComp -= dopStavka;
                                bank += dopStavka;
                            }
                        }
                    }
                }

                if (!allIn && (fichkiIgroka == 0 || fichkiComp == 0)) {
                    allIn = true;
                    stol[4] = koloda[poz++];
                    cout << "\n\tВСЕ-ИН! Все карты открыты:\n";
                    pokazatStol(stol, 5);
                }
            }
        }

        // Ривер
        if (!rukaZavershena && !allIn) {
            stol[4] = koloda[poz++];
            cout << "\n\t--- Р И В Е Р ---\n";
            pokazatStol(stol, 5);

            cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
            if (dolg > 0) cout << " (долг: " << dolg << ")";
            cout << "\n";

            int stavkaIgr = sdelatStavku(fichkiIgroka, 0);
            if (stavkaIgr == -1) {
                cout << "\n\tВы сдались. Компьютер забирает банк.\n";
                fichkiComp += bank;
                rukaZavershena = true;
            }

            if (!rukaZavershena) {
                fichkiIgroka -= stavkaIgr;
                bank += stavkaIgr;

                int stavkaComp = reshenieComputera(fichkiComp, stavkaIgr, rukaKaz, stol, 3);
                if (stavkaComp > 0 && stavkaComp < stavkaIgr) {
                    stavkaComp = stavkaIgr;
                }

                if (stavkaComp == 0) {
                    cout << "\n\tКомпьютер сбрасывает карты.\n";
                    cout << "\n\tВы выиграли банк: " << bank << " фишек!\n";
                    fichkiIgroka += bank;
                    rukaZavershena = true;
                }
                else if (stavkaComp == stavkaIgr) {
                    cout << "\n\tКомпьютер поддерживает вашу ставку: " << stavkaComp << " фишек.\n";
                    fichkiComp -= stavkaComp;
                    bank += stavkaComp;
                }
                else {
                    cout << "\n\tКомпьютер повышает ставку до " << stavkaComp << " фишек!\n";
                    int dopStavka = stavkaComp - stavkaIgr;

                    cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
                    if (dolg > 0) cout << " (долг: " << dolg << ")";
                    cout << "\n";

                    int otvetIgroka = sdelatStavku(fichkiIgroka, dopStavka);
                    if (otvetIgroka == -1) {
                        cout << "\n\tВы сдались. Компьютер забирает банк.\n";
                        fichkiComp += bank;
                        rukaZavershena = true;
                    }
                    else {
                        if (otvetIgroka < dopStavka) {
                            cout << "\n\tВы идёте ALL-IN на " << otvetIgroka << " фишек!\n";
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;
                            int compDobavka = dopStavka - otvetIgroka;
                            fichkiComp -= compDobavka;
                            bank += compDobavka;
                            rukaZavershena = true;
                            allIn = true;
                            cout << "\n\tВСЕ-ИН! Все карты открыты:\n";
                            pokazatStol(stol, 5);
                        }
                        else {
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;

                            int raznica = otvetIgroka - dopStavka;
                            if (raznica > 0) {
                                int compDobavka = min(raznica, fichkiComp);
                                fichkiComp -= compDobavka;
                                bank += compDobavka;
                                cout << "\n\tКомпьютер уравнивает ваше повышение: +" << compDobavka << " фишек.\n";
                            }
                            else {
                                fichkiComp -= dopStavka;
                                bank += dopStavka;
                            }
                        }
                    }
                }
            }
        }

        // Шоудаун
        string kombinaciyaIgroka = "";
        int resultatRuki = 0;

        if (!rukaZavershena) {
            cout << "\n\t*****************************************\n";
            cout << "\t*          Ш О У Д А У Н                *\n";
            cout << "\t*****************************************\n\n";

            cout << "\tВаши карты:\n";
            pokazatRuku(rukaIgr, 2);
            cout << "\n\tКарты компьютера:\n";
            pokazatRuku(rukaKaz, 2);
            cout << "\n\tКарты на столе:\n";
            pokazatStol(stol, 5);

            resultatRuki = sravnitRuki(rukaIgr, rukaKaz, stol);
            kombinaciyaIgroka = string(opisatLuchshuyuRuku(rukaIgr, stol));

            if (resultatRuki == 1) {
                cout << "\n\tВы выиграли банк: " << bank << " фишек!\n";
                fichkiIgroka += bank;
            }
            else if (resultatRuki == -1) {
                cout << "\n\tКомпьютер выиграл банк: " << bank << " фишек.\n";
                fichkiComp += bank;
            }
            else {
                cout << "\n\tНичья! Банк разделён.\n";
                fichkiIgroka += bank / 2;
                fichkiComp += bank / 2;
                resultatRuki = 0;
            }
        }

        // Проверка достижений
        proveritDostizheniya(dostizh, fichkiIgroka, dolg, resultatRuki, kombinaciyaIgroka);

        // Вывод счёта
        cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
        if (dolg > 0) {
            cout << " (долг: " << dolg << ")";
        }
        cout << "\n\tСчёт компьютера: " << fichkiComp << " фишек\n\n";

        if (fichkiComp <= 0) {
            cout << "\n\tКомпьютер обанкротился! Вы победили!\n";
            system("pause");
            break;
        }

        // Погашение долга
        if (dolg > 0 && fichkiIgroka > 0) {
            cout << "\n\tХотите погасить часть долга? (1 — да, 0 — нет): ";
            if (bezopasnyVvodChisla(0, 1) == 1) {
                int summa = min(fichkiIgroka, dolg);
                int oldDolg = dolg;
                fichkiIgroka -= summa;
                dolg -= summa;
                cout << "\n\tВы погасили " << summa << " фишек долга.\n";
                if (dolg == 0) {
                    cout << "\tДолг полностью погашен!\n";
                    poslednyShansIsPolzovan = false;
                    if (oldDolg == MAX_DOLG && !dostizh.bankrotSpasitel) {
                        dostizh.bankrotSpasitel = true;
                        cout << "\n\t?? НОВОЕ ДОСТИЖЕНИЕ: Банкрот-спаситель!\n";
                    }
                }
                else {
                    cout << "\tОстаток долга: " << dolg << " фишек.\n";
                }
            }
        }

        // Меню выхода
        cout << "\n\tВыберите действие:\n";
        cout << "\t1 — Сыграть ещё\n";
        cout << "\t2 — Сохранить и выйти\n";
        cout << "\t3 — Показать достижения\n";
        cout << "\t0 — Выйти без сохранения\n\t";
        int prod = bezopasnyVvodChisla(0, 3);
        if (prod == 3) {
            pokazatDostizheniya(dostizh);
            system("pause");
            system("cls");
            continue;
        }
        else if (prod == 2) {
            sohranitIgru(fichkiIgroka, dolg, fichkiComp, poslednyShansIsPolzovan, dostizh);
            break;
        }
        else if (prod == 0) {
            break;
        }
        system("cls");
    }
}