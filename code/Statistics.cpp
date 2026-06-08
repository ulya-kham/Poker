#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <windows.h>
#include "Statistics.h"
#include "ConsoleUI.h"
#include "Profile.h"
#include "Exceptions.h"
Statistics::Stats Statistics::stats;//статистика
char Statistics::currentPlayerName[50] = { 0 };//имя тек игрока
void Statistics::setCurrentPlayer(const char* name) {//уст тек игрока
    if (name) {
        strcpy(currentPlayerName, name);
        load();
    }
}
void Statistics::load() {//загруз стат из файла
    if (strlen(currentPlayerName) == 0) return;
    char fileName[100];
    Profile::getStatsFileName(currentPlayerName, fileName);
    std::ifstream file(fileName);
    if (!file.is_open()) {
        stats = Stats();//сброс до нуля
        return;
    }
    std::string key;
    int value;
    bool ok = true;
    for (int i = 0; i < 5; i++) {
        if (!(file >> key >> value)) {
            ok = false;
            break;
        }
        if (key == "igry:") stats.games = value;
        else if (key == "pobedy:") stats.wins = value;
        else if (key == "porazheniya:") stats.losses = value;
        else if (key == "vyigrysh:") stats.totalWin = value;
        else if (key == "proigrish:") stats.totalLoss = value;
        else {
            ok = false;
            break;
        }
    }
    file.close();
    if (!ok) stats = Stats();
}
void Statistics::save() {//сохр стат в файл
    if (strlen(currentPlayerName) == 0) return;
    char fileName[100];
    Profile::getStatsFileName(currentPlayerName, fileName);
    std::ofstream file(fileName);
    if (!file.is_open()) {
        std::cout << "\n\t" << Color::brightRed << "Не удалось сохр статистику." << Color::reset << "\n";
        return;
    }
    file << "igry: " << stats.games << "\n";
    file << "pobedy: " << stats.wins << "\n";
    file << "porazheniya: " << stats.losses << "\n";
    file << "vyigrysh: " << stats.totalWin << "\n";
    file << "proigrish: " << stats.totalLoss << "\n";
    file.close();
}
void Statistics::update(bool won, int amount) {//обнов стат после руки
    if (amount < 0) return;
    if (strlen(currentPlayerName) == 0) return;
    stats.games++;
    if (won) {
        stats.wins++;
        stats.totalWin += amount;
    }
    else {
        stats.losses++;
        stats.totalLoss += amount;
    }
    save();
}
void Statistics::display() {//вывод стат в консоль
    system("cls");
    std::cout << "\n\t" << Color::lime << "███████████████████████████████████████████████████████\n";
    std::cout << "\t█" << Color::lime << Color::bold << "                                                     █\n";
    std::cout << Color::brightWhite << Color::bold << "\t█                С Т А Т И С Т И К А                  █\n";
    std::cout << "\t█" << Color::brightWhite << Color::bold << "                                                     █\n";
    std::cout << "\t███████████████████████████████████████████████████████" << Color::reset << "\n\n";
    if (stats.games == 0) {
        std::cout << "\t" << Color::orange << "Вы ещё не сыграли ни одной руки!" << Color::reset << "\n\n";
    }
    else {
        std::cout << "\t" << Color::brightCyan << "Всего сыграно рук:  " << Color::brightCyan << stats.games << Color::reset << "\n";
        std::cout << "\t" << Color::brightGreen << "Побед:              " << stats.wins << Color::reset << "\n";
        std::cout << "\t" << Color::brightRed << "Поражений:          " << stats.losses << Color::reset << "\n";
        std::cout << "\t" << Color::gold << "Суммарный выигрыш:  " << stats.totalWin << " фишек" << Color::reset << "\n";
        std::cout << "\t" << Color::coral << "Суммарный проигрыш: " << stats.totalLoss << " фишек" << Color::reset << "\n";
        double winrate = (stats.games > 0) ? (stats.wins * 100.0 / stats.games) : 0.0;
        std::cout << "\t" << Color::brightMagenta << "Процент побед:      " << std::fixed << winrate << "%" << Color::reset << "\n\n";
    }
    pause();
}
void Statistics::showAchievementsFromFile(const char* playerName) {//показ достижений из файла
    char fileName[100];
    Profile::getSaveFileName(playerName, fileName);
    std::ifstream file(fileName);
    if (!file.is_open()) {
        system("cls");
        std::cout << Color::brightMagenta << "\n\tДостижений" << Color::orange << " пока нет. Сыграйте хотя бы одну руку!" << Color::reset << "\n";
        pause();
        return;
    }
    std::string line;
    std::getline(file, line);//дата
    std::getline(file, line);//пустая
    for (int i = 0; i < 4; i++) {
        if (!std::getline(file, line)) {
            file.close();
            system("cls");
            std::cout << Color::orange << "\n\tДостижений пока нет." << Color::reset << "\n";
            pause();
            return;
        }
    }
    std::getline(file, line);//пустая перед достижениями
    struct Achievements {//структ достижений
        bool novice, lucky, bankruptRescuer, combinationKing, millionaire;
        int winStreak;
    } achievements;
    file >> achievements.novice >> achievements.lucky >> achievements.bankruptRescuer
        >> achievements.combinationKing >> achievements.millionaire >> achievements.winStreak;
    file.close();
    system("cls");
    std::cout << "\n\t" << Color::brightMagenta << "███████████████████████████████████████████████████████\n";
    std::cout << "\t█" << Color::brightMagenta << Color::bold << "                                                     █\n";
    std::cout << "\t█" << Color::brightWhite << Color::bold << "                 Д О С Т И Ж Е Н И Я                 █\n";
    std::cout << "\t█" << Color::brightWhite << Color::bold << "                                                     █\n";
    std::cout << "\t███████████████████████████████████████████████████████" << Color::reset << "\n\n";
      std::cout << Color::brightCyan << "\tОБЫКНОВЕННЫЕ:" << Color::reset << "\n";
    std::cout << "\t  [ " << (achievements.novice ? Color::brightGreen : Color::gray) << "+" << Color::reset << " ] " << Color::brightCyan << "Новичок" << Color::reset << " — сыграть первую руку\n";
    std::cout << "\t  [ " << (achievements.lucky ? Color::brightGreen : Color::gray) << "+" << Color::reset << " ] " << Color::brightCyan << "Удачливый" << Color::reset << " — 5 побед подряд\n";
    std::cout << "\t  [ " << (achievements.bankruptRescuer ? Color::brightGreen : Color::gray) << "+" << Color::reset << " ] " << Color::brightCyan << "Банкрот-спаситель" << Color::reset << " — погасить долг\n";
    std::cout << Color::brightGreen << "\n\tНЕСТАНДАРТНЫЕ:" << Color::reset << "\n";
    std::cout << "\t  [ " << (achievements.combinationKing ? Color::brightGreen : Color::gray) << "+" << Color::reset << " ] " << Color::brightGreen << "Король комбинаций" << Color::reset << " — каре или выше\n";
    std::cout << Color::gold << Color::bold << "\n\tРЕДКИЕ:" << Color::reset << "\n";
    std::cout << "\t  [ " << (achievements.millionaire ? Color::gold : Color::gray) << "+" << Color::reset << " ] " << Color::gold << "Миллионер" << Color::reset << " — накопить 1000+ фишек\n";
    pause();
}