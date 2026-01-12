// game.cpp
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <ctime>
#include <limits>
#include "game.h"
#include "ui.h"
#include "karta.h"
#include "kombinacii.h"
#include "input.h"
#include "computer.h"
#include "profile.h"
#include "stats.h"

using namespace std;

const int START_FICHI = 100;
const int MALYI_BLIND = 5;
const int BOLSHOI_BLIND = 10;
const int MAX_DOLG = 50;
const int POSLEDNIY_SHANS = 10;

// Структура для достижений
struct Dostizheniya {
    bool novichok = false;
    bool udachlivyi = false;
    bool bankrotSpasitel = false;
    bool korolKombinaciy = false;
    bool millioner = false;
    int podryadPobed = 0;
};

// Сохранение игры для конкретного игрока
bool sohranitIgru(const char* imyaIgroka, int fichkiIgroka, int dolg, int fichkiComp, bool poslednyShansIsPolzovan, const Dostizheniya& d) {
    char imyaFila[100];
    poluchitImyaFilaSohraneniya(imyaIgroka, imyaFila);

    ofstream file(imyaFila);
    if (!file.is_open()) {
        cout << "\n\t" << Color::red << "Ошибка: не удалось сохранить игру." << Color::reset << "\n";
        return false;
    }

    time_t now = time(0);
    char dataVremya[30];
    strftime(dataVremya, sizeof(dataVremya), "%Y-%m-%d %H:%M:%S", localtime(&now));
    file << "posledny_vhod: " << dataVremya << "\n\n";

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
    cout << "\n\t" << Color::green << "Игра и достижения сохранены для \"" << imyaIgroka << "\"!" << Color::reset << "\n";
    return true;
}

// Загрузка игры для конкретного игрока
bool zagruzitIgru(const char* imyaIgroka, int& fichkiIgroka, int& dolg, int& fichkiComp, bool& poslednyShansIsPolzovan, Dostizheniya& d) {
    char imyaFila[100];
    poluchitImyaFilaSohraneniya(imyaIgroka, imyaFila);

    ifstream file(imyaFila);
    if (!file.is_open()) return false;

    string line;
    getline(file, line); // дата
    getline(file, line); // пустая строка

    file >> fichkiIgroka >> dolg >> fichkiComp >> poslednyShansIsPolzovan;
    file.ignore(1000, '\n');
    file >> d.novichok >> d.udachlivyi >> d.bankrotSpasitel >> d.korolKombinaciy >> d.millioner >> d.podryadPobed;
    file.close();
    return true;
}

// Вывод достижений с цветным оформлением и категориями
void pokazatDostizheniya(const Dostizheniya& d) {
    system("cls");

    cout << "\n\t" << Color::blue << "=============================================\n";
    cout << "\t\t\tДОСТИЖЕНИЯ\n";
    cout << "\t=============================================" << Color::reset << "\n\n";

    cout << Color::cyan << "\tОБЫКНОВЕННЫЕ:\n" << Color::reset;
    cout << "\t  [ " << (d.novichok ? "+" : " ") << " ] Новичок — сыграть первую руку\n";
    cout << "\t  [ " << (d.udachlivyi ? "+" : " ") << " ] Удачливый — 5 побед подряд\n";
    cout << "\t  [ " << (d.bankrotSpasitel ? "+" : " ") << " ] Банкрот-спаситель — погасить долг 50 фишек\n\n";

    cout << Color::green << "\tНЕСТАНДАРТНЫЕ:\n" << Color::reset;
    cout << "\t  [ " << (d.korolKombinaciy ? "+" : " ") << " ] Король комбинаций — собрать каре или выше\n\n";

    cout << Color::blue << "\tРЕДКИЕ:\n" << Color::reset;
    cout << "\t  [ " << (d.millioner ? "+" : " ") << " ] Миллионер — накопить 1000+ фишек\n\n";

    moyaPauza();
}

// Проверка и разблокировка достижений
void proveritDostizheniya(Dostizheniya& d, int fichkiIgroka, int dolg, int resultatRuki, const string& kombinaciya) {
    if (!d.novichok) {
        d.novichok = true;
        cout << "\n\t" << Color::blue << "НОВОЕ ДОСТИЖЕНИЕ: Новичок!" << Color::reset << "\n";
    }

    if (resultatRuki == 1) {
        d.podryadPobed++;
        if (d.podryadPobed >= 5 && !d.udachlivyi) {
            d.udachlivyi = true;
            cout << "\n\t" << Color::blue << "НОВОЕ ДОСТИЖЕНИЕ: Удачливый!" << Color::reset << "\n";
        }
    }
    else {
        d.podryadPobed = 0;
    }

    if (!d.korolKombinaciy) {
        if (kombinaciya.find("Каре") != string::npos ||
            kombinaciya.find("Стрит-флеш") != string::npos ||
            kombinaciya.find("Роял-флеш") != string::npos) {
            d.korolKombinaciy = true;
            cout << "\n\t" << Color::blue << "НОВОЕ ДОСТИЖЕНИЕ: Король комбинаций!" << Color::reset << "\n";
        }
    }

    if (fichkiIgroka >= 1000 && !d.millioner) {
        d.millioner = true;
        cout << "\n\t" << Color::blue << "НОВОЕ ДОСТИЖЕНИЕ: Миллионер!" << Color::reset << "\n";
    }
}

// Функция для отображения шоудауна
int pokazatShoudaun(karta rukaIgr[], karta rukaKaz[], karta stol[], int& bank, int& fichkiIgroka, int& fichkiComp) {
    cout << "\n\t" << Color::cyan << "*****************************************\n";
    cout << "\t*          Ш О У Д А У Н                *\n";
    cout << "\t*****************************************" << Color::reset << "\n\n";

    cout << "\t" << Color::cyan << "Ваши карты:\n";
    pokazatRuku(rukaIgr, 2);
    cout << "\n\t" << Color::reset << "Карты компьютера:\n";
    pokazatRuku(rukaKaz, 2);
    cout << "\n\t" << Color::cyan << "Карты на столе:\n";
    pokazatStol(stol, 5);

    int resultatRuki = sravnitRuki(rukaIgr, rukaKaz, stol);
    string kombinaciyaIgroka = string(opisatLuchshuyuRuku(rukaIgr, stol));

    if (resultatRuki == 1) {
        cout << "\n\t" << Color::green << "Вы выиграли банк: " << bank << " фишек!" << Color::reset << "\n";
        fichkiIgroka += bank;
    }
    else if (resultatRuki == -1) {
        cout << "\n\t" << Color::red << "Компьютер выиграл банк: " << bank << " фишек." << Color::reset << "\n";
        fichkiComp += bank;
    }
    else {
        cout << "\n\t" << Color::yellow << "Ничья! Банк разделён." << Color::reset << "\n";
        fichkiIgroka += bank / 2;
        fichkiComp += bank / 2;
        resultatRuki = 0;
    }

    if (resultatRuki == 1) {
        obnovitStatistiku(true, bank);
    }
    else if (resultatRuki == -1) {
        obnovitStatistiku(false, bank);
    }

    return resultatRuki;
}

bool vzyatKredit(int& fichki, int& dolg) {
    if (dolg > 0) {
        cout << "\n\t" << Color::red << "У вас уже есть долг перед банком (" << dolg << " фишек). Сначала погасите его!" << Color::reset << "\n";
        return false;
    }
    if (fichki > 10) {
        cout << "\n\t" << Color::yellow << "Кредит доступен только при балансе ? 10 фишек." << Color::reset << "\n";
        return false;
    }
    cout << "\n\t" << Color::yellow << "Банк предлагает кредит до " << MAX_DOLG << " фишек.\n";
    cout << "\tСколько хотите взять? (" << Color::green << "1–" << MAX_DOLG << "" << Color::yellow << ", " << Color::red << "0 — отказ):" << Color::reset << " ";
    int summa = bezopasnyVvodChisla(0, MAX_DOLG);
    if (summa == 0) {
        cout << "\n\t" << Color::yellow << "Вы отказались от кредита." << Color::reset << "\n";
        return false;
    }
    fichki += summa;
    dolg = summa;
    cout << "\n\t" << Color::yellow << "Вы взяли кредит на " << summa << " фишек. " << Color::red << "Долг: " << dolg << " фишек." << Color::reset << "\n";
    return true;
}

// ОСНОВНАЯ ФУНКЦИЯ — принимает флаг загрузки
void zapustitIgru(bool zagruzitIzFila, const char* imyaIgroka) {
    system("cls");
    cout << "\n\t" << Color::cyan << "Запуск новой игры..." << Color::reset << "\n\n";

    int fichkiIgroka = START_FICHI;
    int fichkiComp = 1000;
    int dolg = 0;
    bool poslednyShansIsPolzovan = false;
    Dostizheniya dostizh = {};

    // Загружаем достижения даже при новой игре
    char imyaFila[100];
    poluchitImyaFilaSohraneniya(imyaIgroka, imyaFila);
    ifstream fileDost(imyaFila);
    if (fileDost.is_open()) {
        string line;
        getline(fileDost, line);
        getline(fileDost, line);
        for (int i = 0; i < 4; i++) {
            getline(fileDost, line);
        }
        getline(fileDost, line);
        fileDost >> dostizh.novichok
            >> dostizh.udachlivyi
            >> dostizh.bankrotSpasitel
            >> dostizh.korolKombinaciy
            >> dostizh.millioner
            >> dostizh.podryadPobed;
        fileDost.close();
    }

    if (zagruzitIzFila) {
        ifstream testFile(imyaFila);
        if (testFile.good()) {
            if (zagruzitIgru(imyaIgroka, fichkiIgroka, dolg, fichkiComp, poslednyShansIsPolzovan, dostizh)) {
                cout << "\n\t" << Color::green << "Сохранение успешно загружено!" << Color::reset << "\n";
                pokazatDostizheniya(dostizh);
            }
            else {
                cout << "\n\t" << Color::red << "Ошибка загрузки. " << Color::green << "Начинаем новую игру." << Color::reset << "\n";
                moyaPauza();
            }
        }
        else {
            cout << "\n\t" << Color::red << "Сохранение для \"" << imyaIgroka << "\" не найдено. " << Color::green << "Начинаем новую игру." << Color::reset << "\n";
            moyaPauza();
        }
        testFile.close();
    }

    // Основной игровой цикл
    while (fichkiIgroka >= 0 && fichkiComp > 0) {
        // ... (логика банкротства и кредита без изменений) ...
        if (fichkiIgroka <= 0 && dolg > 0) {
            if (!poslednyShansIsPolzovan) {
                cout << "\n\t" << Color::red << "У вас закончились фишки, но остался долг (" << dolg << " фишек)!" << Color::reset << "\n";
                cout << "\t" << Color::yellow << "Банк даёт вам " << Color::green << "ПОСЛЕДНИЙ ШАНС: " << POSLEDNIY_SHANS << " фишек." << Color::reset << "\n\n";
                cout << "\t" << Color::red << "Если проиграете эту руку — игра окончена!" << Color::reset << "\n";
                cout << "\tПринять последний шанс? (" << Color::green << "1 — да" << Color::reset << ", " << Color::red << "0 — нет" << Color::reset << "): ";
                int vybor = bezopasnyVvodChisla(0, 1);
                if (vybor == 1) {
                    fichkiIgroka = POSLEDNIY_SHANS;
                    dolg += POSLEDNIY_SHANS;
                    poslednyShansIsPolzovan = true;
                    cout << "\n\t" << Color::green << "Вы получили " << POSLEDNIY_SHANS << " фишек. " << Color::red << "Новый долг: " << dolg << " фишек." << Color::reset << "\n\n";
                }
                else {
                    cout << "\n\t" << Color::red << "Вы отказались от последнего шанса." << Color::reset << "\n";
                    cout << "\t" << Color::red << "Игра окончена. Банк забирает всё." << Color::reset << "\n";
                    moyaPauza();
                    return;
                }
            }
            else {
                cout << "\n\t" << Color::red << "У вас нет фишек, долг перед банком не погашен, и последний шанс использован." << Color::reset << "\n";
                cout << "\t" << Color::red << "Вы объявлены банкротом. Игра окончена." << Color::reset << "\n";
                moyaPauza();
                return;
            }
        }

        if (fichkiIgroka <= 10 && dolg == 0) {
            cout << "\n\t" << Color::yellow << "У вас мало фишек (" << fichkiIgroka << "). Хотите взять кредит?" << Color::reset << "\n";
            cout << "\t(" << Color::green << "1 — да" << Color::reset << ", " << Color::red << "0 — нет" << Color::reset << "): ";
            int vybor = bezopasnyVvodChisla(0, 1);
            if (vybor == 1) {
                vzyatKredit(fichkiIgroka, dolg);
            }
        }

        cout << "\n\t" << Color::yellow << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "\t~           " << Color::green << "Н О В А Я   Р У К А             " << Color::yellow << "~\n";
        cout << "\t" << Color::yellow << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << Color::reset << "\n\n";

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
        cout << "\t" << Color::cyan << "В банке: " << bank << " фишек" << Color::reset << "\n\n";

        pokazatRuku(rukaIgr, 2);

        bool allIn = false;
        bool rukaZavershena = false;
        int resultatRuki = 0; // ? добавлено

        // Префлоп
        cout << "\n\t" << Color::cyan << "--- П Р Е Ф Л О П ---" << Color::reset << "\n";
        int raznicaDoUrafnivaniya = BOLSHOI_BLIND - MALYI_BLIND;
        int stavkaIgr = 0;

        if (fichkiIgroka > 0) {
            cout << "\n\t" << Color::green << "Ваш счёт: " << fichkiIgroka << " фишек" << Color::reset;
            if (dolg > 0) cout << " " << Color::red << "(долг: " << dolg << ")" << Color::reset;
            cout << "\n";

            stavkaIgr = sdelatStavku(fichkiIgroka, raznicaDoUrafnivaniya);
            if (stavkaIgr == -1) {
                cout << "\n\t" << Color::red << "Вы сдались. " << Color::yellow << "Компьютер забирает банк: " << bank << " фишек.\n" << Color::reset;
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
                cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                pokazatStol(stol, 5);

                // === ШОУДАУН ПРИ ALL-IN ===
                resultatRuki = pokazatShoudaun(rukaIgr, rukaKaz, stol, bank, fichkiIgroka, fichkiComp);
            }
            else {
                fichkiIgroka -= stavkaIgr;
                bank += stavkaIgr;

                int stavkaComp = reshenieComputera(fichkiComp, stavkaIgr, rukaKaz, stol, 0, fichkiIgroka);
                if (stavkaComp > 0 && stavkaComp < stavkaIgr) {
                    stavkaComp = stavkaIgr;
                }

                if (stavkaComp == 0) {
                    cout << "\n\t" << Color::green << "Компьютер сбрасывает карты.\n";
                    cout << "\n\tВы выиграли банк: " << bank << " фишек!" << Color::reset << "\n";
                    fichkiIgroka += bank;
                    rukaZavershena = true;
                }
                else if (stavkaComp == stavkaIgr) {
                    cout << "\n\tКомпьютер поддерживает вашу ставку: " << stavkaComp << " фишек.\n";
                    fichkiComp -= stavkaComp;
                    bank += stavkaComp;
                }
                else {
                    cout << "\n\t" << Color::yellow << "Компьютер повышает ставку до " << stavkaComp << " фишек!\n" << Color::reset;
                    int dopStavka = stavkaComp - stavkaIgr;

                    cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
                    if (dolg > 0) cout << " " << Color::red << "(долг: " << dolg << ")" << Color::reset;
                    cout << "\n";

                    int otvetIgroka = sdelatStavku(fichkiIgroka, dopStavka);
                    if (otvetIgroka == -1) {
                        cout << "\n\t" << Color::red << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
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
                            cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                            pokazatStol(stol, 5);

                            // === ШОУДАУН ПРИ ALL-IN ===
                            resultatRuki = pokazatShoudaun(rukaIgr, rukaKaz, stol, bank, fichkiIgroka, fichkiComp);
                        }
                        else {
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;

                            int raznica = otvetIgroka - dopStavka;
                            if (raznica > 0) {
                                int compDobavka = min(raznica, fichkiComp);
                                fichkiComp -= compDobavka;
                                bank += compDobavka;
                                cout << "\n\t" << Color::cyan << "Компьютер уравнивает ваше повышение: +" << compDobavka << " фишек." << Color::reset << "\n";
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
                    cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                    pokazatStol(stol, 5);

                    // === ШОУДАУН ПРИ ALL-IN ===
                    resultatRuki = pokazatShoudaun(rukaIgr, rukaKaz, stol, bank, fichkiIgroka, fichkiComp);
                }
            }
        }

        // Флоп
        if (!rukaZavershena && !allIn) {
            for (int i = 0; i < 3; i++) stol[i] = koloda[poz++];
            cout << "\n\t" << Color::cyan << "--- Ф Л О П ---\n" << Color::reset;
            pokazatStol(stol, 3);

            cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
            if (dolg > 0) cout << " " << Color::red << "(долг: " << dolg << ")" << Color::reset << "";
            cout << "\n";

            int stavkaIgr = sdelatStavku(fichkiIgroka, 0);
            if (stavkaIgr == -1) {
                cout << "\n\t" << Color::red << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                fichkiComp += bank;
                rukaZavershena = true;
            }

            if (!rukaZavershena) {
                fichkiIgroka -= stavkaIgr;
                bank += stavkaIgr;

                int stavkaComp = reshenieComputera(fichkiComp, stavkaIgr, rukaKaz, stol, 1, fichkiIgroka);
                if (stavkaComp > 0 && stavkaComp < stavkaIgr) {
                    stavkaComp = stavkaIgr;
                }

                if (stavkaComp == 0) {
                    cout << "\n\t" << Color::green << "Компьютер сбрасывает карты.\n";
                    cout << "\n\tВы выиграли банк: " << bank << " фишек!" << Color::reset << "\n";
                    fichkiIgroka += bank;
                    rukaZavershena = true;
                }
                else if (stavkaComp == stavkaIgr) {
                    cout << "\n\t" << Color::cyan << "Компьютер поддерживает вашу ставку: " << stavkaComp << " фишек." << Color::reset << "\n";
                    fichkiComp -= stavkaComp;
                    bank += stavkaComp;
                }
                else {
                    cout << "\n\t" << Color::cyan << "Компьютер повышает ставку до " << stavkaComp << " фишек!" << Color::reset << "\n";
                    int dopStavka = stavkaComp - stavkaIgr;

                    cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
                    if (dolg > 0) cout << " " << Color::red << "(долг: " << dolg << ")" << Color::reset;
                    cout << "\n";

                    int otvetIgroka = sdelatStavku(fichkiIgroka, dopStavka);
                    if (otvetIgroka == -1) {
                        cout << "\n\t" << Color::red << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                        fichkiComp += bank;
                        rukaZavershena = true;
                    }
                    else {
                        if (otvetIgroka < dopStavka) {
                            cout << "\n\t" << Color::cyan << "Вы идёте ALL-IN на " << otvetIgroka << " фишек!" << Color::reset << "\n";
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;
                            int compDobavka = dopStavka - otvetIgroka;
                            fichkiComp -= compDobavka;
                            bank += compDobavka;
                            rukaZavershena = true;
                            allIn = true;
                            for (int i = 3; i < 5; i++) stol[i] = koloda[poz++];
                            cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                            pokazatStol(stol, 5);

                            // === ШОУДАУН ПРИ ALL-IN ===
                            resultatRuki = pokazatShoudaun(rukaIgr, rukaKaz, stol, bank, fichkiIgroka, fichkiComp);
                        }
                        else {
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;

                            int raznica = otvetIgroka - dopStavka;
                            if (raznica > 0) {
                                int compDobavka = min(raznica, fichkiComp);
                                fichkiComp -= compDobavka;
                                bank += compDobavka;
                                cout << "\n\t" << Color::cyan << "Компьютер уравнивает ваше повышение: +" << compDobavka << " фишек." << Color::reset << "\n";
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
                    cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                    pokazatStol(stol, 5);

                    // === ШОУДАУН ПРИ ALL-IN ===
                    resultatRuki = pokazatShoudaun(rukaIgr, rukaKaz, stol, bank, fichkiIgroka, fichkiComp);
                }
            }
        }

        // Тёрн
        if (!rukaZavershena && !allIn) {
            stol[3] = koloda[poz++];
            cout << "\n\t" << Color::yellow << "--- Т Ё Р Н ---" << Color::reset << "\n";
            pokazatStol(stol, 4);

            cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
            if (dolg > 0) cout << " " << Color::red << "(долг: " << dolg << ")" << Color::reset;
            cout << "\n";

            int stavkaIgr = sdelatStavku(fichkiIgroka, 0);
            if (stavkaIgr == -1) {
                cout << "\n\t" << Color::red << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                fichkiComp += bank;
                rukaZavershena = true;
            }

            if (!rukaZavershena) {
                fichkiIgroka -= stavkaIgr;
                bank += stavkaIgr;

                int stavkaComp = reshenieComputera(fichkiComp, stavkaIgr, rukaKaz, stol, 2, fichkiIgroka);
                if (stavkaComp > 0 && stavkaComp < stavkaIgr) {
                    stavkaComp = stavkaIgr;
                }

                if (stavkaComp == 0) {
                    cout << "\n\t" << Color::green << "Компьютер сбрасывает карты.\n";
                    cout << "\n\tВы выиграли банк: " << bank << " фишек!" << Color::reset << "\n";
                    fichkiIgroka += bank;
                    rukaZavershena = true;
                }
                else if (stavkaComp == stavkaIgr) {
                    cout << "\n\t" << Color::cyan << "Компьютер поддерживает вашу ставку: " << stavkaComp << " фишек." << Color::reset << "\n";
                    fichkiComp -= stavkaComp;
                    bank += stavkaComp;
                }
                else {
                    cout << "\n\t" << Color::cyan << "Компьютер повышает ставку до " << stavkaComp << " фишек!" << Color::reset << "\n";
                    int dopStavka = stavkaComp - stavkaIgr;

                    cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
                    if (dolg > 0) cout << " " << Color::red << "(долг: " << dolg << ")" << Color::reset;
                    cout << "\n";

                    int otvetIgroka = sdelatStavku(fichkiIgroka, dopStavka);
                    if (otvetIgroka == -1) {
                        cout << "\n\t" << Color::red << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                        fichkiComp += bank;
                        rukaZavershena = true;
                    }
                    else {
                        if (otvetIgroka < dopStavka) {
                            cout << "\n\t" << Color::cyan << "Вы идёте ALL-IN на " << otvetIgroka << " фишек!" << Color::reset << "\n";
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;
                            int compDobavka = dopStavka - otvetIgroka;
                            fichkiComp -= compDobavka;
                            bank += compDobavka;
                            rukaZavershena = true;
                            allIn = true;
                            stol[4] = koloda[poz++];
                            cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                            pokazatStol(stol, 5);

                            // === ШОУДАУН ПРИ ALL-IN ===
                            resultatRuki = pokazatShoudaun(rukaIgr, rukaKaz, stol, bank, fichkiIgroka, fichkiComp);
                        }
                        else {
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;

                            int raznica = otvetIgroka - dopStavka;
                            if (raznica > 0) {
                                int compDobavka = min(raznica, fichkiComp);
                                fichkiComp -= compDobavka;
                                bank += compDobavka;
                                cout << "\n\t" << Color::cyan << "Компьютер уравнивает ваше повышение: +" << compDobavka << " фишек." << Color::reset << "\n";
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
                    cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                    pokazatStol(stol, 5);

                    // === ШОУДАУН ПРИ ALL-IN ===
                    resultatRuki = pokazatShoudaun(rukaIgr, rukaKaz, stol, bank, fichkiIgroka, fichkiComp);
                }
            }
        }

        // Ривер
        if (!rukaZavershena && !allIn) {
            stol[4] = koloda[poz++];
            cout << "\n\t" << Color::cyan << "--- Р И В Е Р ---" << Color::reset << "\n";
            pokazatStol(stol, 5);

            cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
            if (dolg > 0) cout << " " << Color::red << "(долг: " << dolg << ")" << Color::reset << "";
            cout << "\n";

            int stavkaIgr = sdelatStavku(fichkiIgroka, 0);
            if (stavkaIgr == -1) {
                cout << "\n\t" << Color::red << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                fichkiComp += bank;
                rukaZavershena = true;
            }

            if (!rukaZavershena) {
                fichkiIgroka -= stavkaIgr;
                bank += stavkaIgr;

                int stavkaComp = reshenieComputera(fichkiComp, stavkaIgr, rukaKaz, stol, 3, fichkiIgroka);
                if (stavkaComp > 0 && stavkaComp < stavkaIgr) {
                    stavkaComp = stavkaIgr;
                }

                if (stavkaComp == 0) {
                    cout << "\n\t" << Color::green << "Компьютер сбрасывает карты.\n";
                    cout << "\n\tВы выиграли банк: " << bank << " фишек!" << Color::reset << "\n";
                    fichkiIgroka += bank;
                    rukaZavershena = true;
                }
                else if (stavkaComp == stavkaIgr) {
                    cout << "\n\t" << Color::cyan << "Компьютер поддерживает вашу ставку: " << stavkaComp << " фишек." << Color::reset << "\n";
                    fichkiComp -= stavkaComp;
                    bank += stavkaComp;
                }
                else {
                    cout << "\n\t" << Color::cyan << "Компьютер повышает ставку до " << stavkaComp << " фишек!" << Color::reset << "\n";
                    int dopStavka = stavkaComp - stavkaIgr;

                    cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
                    if (dolg > 0) cout << " " << Color::red << "(долг: " << dolg << ")" << Color::reset << "";
                    cout << "\n";

                    int otvetIgroka = sdelatStavku(fichkiIgroka, dopStavka);
                    if (otvetIgroka == -1) {
                        cout << "\n\t" << Color::red << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                        fichkiComp += bank;
                        rukaZavershena = true;
                    }
                    else {
                        if (otvetIgroka < dopStavka) {
                            cout << "\n\t" << Color::cyan << "Вы идёте ALL-IN на " << otvetIgroka << " фишек!" << Color::reset << "\n";
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;
                            int compDobavka = dopStavka - otvetIgroka;
                            fichkiComp -= compDobavka;
                            bank += compDobavka;
                            rukaZavershena = true;
                            allIn = true;
                            cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                            pokazatStol(stol, 5);

                            // === ШОУДАУН ПРИ ALL-IN ===
                            resultatRuki = pokazatShoudaun(rukaIgr, rukaKaz, stol, bank, fichkiIgroka, fichkiComp);
                        }
                        else {
                            fichkiIgroka -= otvetIgroka;
                            bank += otvetIgroka;

                            int raznica = otvetIgroka - dopStavka;
                            if (raznica > 0) {
                                int compDobavka = min(raznica, fichkiComp);
                                fichkiComp -= compDobavka;
                                bank += compDobavka;
                                cout << "\n\t" << Color::cyan << "Компьютер уравнивает ваше повышение: +" << compDobavka << " фишек." << Color::reset << "\n";
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

        // Обычный шоудаун (если не был ALL-IN)
        if (!rukaZavershena) {
            resultatRuki = pokazatShoudaun(rukaIgr, rukaKaz, stol, bank, fichkiIgroka, fichkiComp);
        }

        // Проверка достижений
        string kombinaciyaIgroka = string(opisatLuchshuyuRuku(rukaIgr, stol));
        proveritDostizheniya(dostizh, fichkiIgroka, dolg, resultatRuki, kombinaciyaIgroka);

        cout << "\n\tВаш счёт: " << fichkiIgroka << " фишек";
        if (dolg > 0) cout << " " << Color::red << "(долг: " << dolg << ")" << Color::reset << "";
        cout << "\n\tСчёт компьютера: " << fichkiComp << " фишек\n\n";

        if (fichkiComp <= 0) {
            cout << "\n\t" << Color::cyan << "Компьютер обанкротился! " << Color::green << "Вы победили!" << Color::reset << "\n";
            moyaPauza();
            break;
        }

        if (dolg > 0 && fichkiIgroka > 0) {
            cout << "\n\tХотите погасить часть долга? (" << Color::green << "1 — да" << Color::reset << ", " << Color::red << "0 — нет" << Color::reset << "): ";
            if (bezopasnyVvodChisla(0, 1) == 1) {
                int summa = min(fichkiIgroka, dolg);
                int oldDolg = dolg;
                fichkiIgroka -= summa;
                dolg -= summa;
                cout << "\n\t" << Color::yellow << "Вы погасили " << summa << " фишек долга." << Color::reset << "\n";
                if (dolg == 0) {
                    cout << "\t" << Color::green << "Долг полностью погашен!" << Color::reset << "\n";
                    poslednyShansIsPolzovan = false;
                    if (oldDolg == MAX_DOLG && !dostizh.bankrotSpasitel) {
                        dostizh.bankrotSpasitel = true;
                        cout << "\n\t " << Color::blue << "НОВОЕ ДОСТИЖЕНИЕ: Банкрот-спаситель!" << Color::reset << "\n";
                    }
                }
                else {
                    cout << "\t" << Color::yellow << "Остаток долга: " << dolg << " фишек." << Color::reset << "\n";
                }
            }
        }

        // Меню выхода
        cout << "\n\t" << Color::cyan << "--- Выберите действие: ---" << Color::reset << "\n";
        cout << "\t1 — " << Color::green << "Сыграть ещё" << Color::reset << "\n";
        cout << "\t2 — " << Color::yellow << "Сохранить и выйти" << Color::reset << "\n";
        cout << "\t0 — " << Color::gray << "Выйти без сохранения" << Color::reset << "\n\t";
        int prod = bezopasnyVvodChisla(0, 2);
        if (prod == 2) {
            sohranitIgru(imyaIgroka, fichkiIgroka, dolg, fichkiComp, poslednyShansIsPolzovan, dostizh);
            break;
        }
        else if (prod == 0) {
            break;
        }
        system("cls");
    }
}