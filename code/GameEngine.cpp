#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <ctime>
#include <limits>
#include "GameEngine.h"
#include "ConsoleUI.h"
#include "Card.h"
#include "Deck.h"
#include "CustomStack.h"
#include "HandEvaluator.h"
#include "InputValidator.h"
#include "Exceptions.h"
#include "AIPlayer.h"
#include "Profile.h"
#include "Statistics.h"
#include <thread>
#include <chrono>
constexpr int START_CHIPS = 100;//нач фишки
constexpr int SMALL_BLIND = 5;//мал блайнд
constexpr int BIG_BLIND = 10;//бол блайнд
constexpr int MAX_DEBT = 50;//макс долг
constexpr int LAST_CHANCE = 10;//посл шанс
class Achievements {//класс достижений
public:
    bool novice = false;//новичок
    bool lucky = false;//удачливый
    bool bankruptRescuer = false;//спаситель
    bool combinationKing = false;//король комб
    bool millionaire = false;//миллионер
    int winStreak = 0;//серия побед
};
bool saveGame(const char* playerName, int playerChips, int debt, int computerChips, bool lastChanceUsed, const Achievements& achievements) {//сохр игры
    char fileName[100];
    Profile::getSaveFileName(playerName, fileName);
    std::ofstream file(fileName);
    if (!file.is_open()) {
        std::cout << "\n\t" << Color::brightRed << "Ошибка: не удалось сохр игру." << Color::reset << "\n";
        return false;
    }
    time_t now = time(0);
    char dateTime[30];
    strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", localtime(&now));
    file << "last_login: " << dateTime << "\n\n";
    file << playerChips << "\n" << debt << "\n" << computerChips << "\n" << lastChanceUsed << "\n\n";
    file << achievements.novice << "\n" << achievements.lucky << "\n" << achievements.bankruptRescuer << "\n";
    file << achievements.combinationKing << "\n" << achievements.millionaire << "\n" << achievements.winStreak;
    file.close();
    std::cout << "\n\t" << Color::brightGreen << "Игра и достижения сохранены для \"" << Color::gold << playerName << Color::brightGreen << "\"!" << Color::reset << "\n";
    return true;
}
bool loadGame(const char* playerName, int& playerChips, int& debt, int& computerChips, bool& lastChanceUsed, Achievements& achievements) {//загруз игры
    char fileName[100];
    Profile::getSaveFileName(playerName, fileName);
    std::ifstream file(fileName);
    if (!file.is_open()) return false;
    std::string line;
    std::getline(file, line);
    std::getline(file, line);
    file >> playerChips >> debt >> computerChips >> lastChanceUsed;
    file.ignore(1000, '\n');
    file >> achievements.novice >> achievements.lucky >> achievements.bankruptRescuer >> achievements.combinationKing >> achievements.millionaire >> achievements.winStreak;
    file.close();
    return true;
}
void checkAchievements(Achievements& achievements, int playerChips, int debt, int handResult, const std::string& combination) {//провер достижений
    if (!achievements.novice) {
        achievements.novice = true;
        std::cout << "\n\t" << Color::brightMagenta << Color::bold << "🏆 НОВОЕ ДОСТИЖЕНИЕ: Новичок!" << Color::reset << "\n";
    }
    if (handResult == 1) {
        achievements.winStreak++;
        if (achievements.winStreak >= 5 && !achievements.lucky) {
            achievements.lucky = true;
            std::cout << "\n\t" << Color::brightMagenta << Color::bold << "🏆 НОВОЕ ДОСТИЖЕНИЕ: Удачливый!" << Color::reset << "\n";
        }
    }
    else achievements.winStreak = 0;
    if (!achievements.combinationKing) {
        if (combination.find("Каре") != std::string::npos || combination.find("Стрит-флеш") != std::string::npos || combination.find("Роял-флеш") != std::string::npos) {
            achievements.combinationKing = true;
            std::cout << "\n\t" << Color::brightMagenta << Color::bold << "🏆 НОВОЕ ДОСТИЖЕНИЕ: Король комбинаций!" << Color::reset << "\n";
        }
    }
    if (playerChips >= 1000 && !achievements.millionaire) {
        achievements.millionaire = true;
        std::cout << "\n\t" << Color::gold << Color::bold << "💰 НОВОЕ ДОСТИЖЕНИЕ: Миллионер!" << Color::reset << "\n";
    }
}
int showShowdown(const CustomStack<Card>& playerHand, const CustomStack<Card>& computerHand, const Card table[], int& bank, int& playerChips, int& computerChips) {//шоудаун
    std::cout << "\n\t" << Color::gold << "███████████████████████████████████████████████████████\n";
    std::cout << "\t█" << Color::gold << Color::bold << "                                                     █\n";
    std::cout << Color::brightWhite << Color::bold << "\t█                   Ш О У Д А У Н                     █\n";
    std::cout << "\t█" << Color::brightWhite << Color::bold << "                                                     █\n";
    std::cout << "\t███████████████████████████████████████████████████████" << Color::reset << "\n\n";
    std::cout << "\t" << Color::brightCyan << "^-^ Ваши карты:" << Color::reset << "\n";
    showHand(playerHand);
    std::cout << "\t" << Color::coral << "^~^ Карты компьютера:" << Color::reset << "\n";
    showHand(computerHand);
    std::cout << "\t" << Color::turquoise << "Карты на столе:" << Color::reset << "\n";
    showTable(table, 5);
    int handResult = HandEvaluator::compareHands(playerHand, computerHand, table, 5);
    if (handResult == 1) {
        std::cout << "\n\t" << Color::brightGreen << Color::bold << "Вы выиграли банк: " << Color::gold << bank << Color::brightGreen << " фишек!" << Color::reset << "\n";
        playerChips += bank;
    }
    else if (handResult == -1) {
        std::cout << "\n\t" << Color::brightRed << "Компьютер выиграл банк: " << Color::gold << bank << Color::brightRed << " фишек." << Color::reset << "\n";
        computerChips += bank;
    }
    else {
        std::cout << "\n\t" << Color::brightYellow << "Ничья! Банк разделён." << Color::reset << "\n";
        playerChips += bank / 2;
        computerChips += bank / 2;
        handResult = 0;
    }
    if (handResult == 1) Statistics::update(true, bank);
    else if (handResult == -1) Statistics::update(false, bank);
    return handResult;
}
bool takeCredit(int& chips, int& debt) {//взять кредит
    if (debt > 0) {
        std::cout << "\n\t" << Color::brightRed << "У вас уже есть долг (" << Color::gold << debt << Color::brightRed << " фишек). Сначала погасите его!" << Color::reset << "\n";
        return false;
    }
    if (chips > 10) {
        std::cout << "\n\t" << Color::orange << "Кредит доступен только при балансе ≤ 10 фишек." << Color::reset << "\n";
        return false;
    }
    std::cout << "\n\t" << Color::gold << "🎀 Банк предлагает кредит до " << MAX_DEBT << " фишек.\n";
    std::cout << "\t" << Color::brightCyan << "Сколько хотите взять? " << Color::gray << "(" << Color::brightGreen << "1–" << MAX_DEBT << Color::gray << ", " << Color::brightRed << "0 — отказ): " << Color::reset;
    int amount = InputValidator::safeInputNumber(0, MAX_DEBT);
    if (amount == 0) {
        std::cout << "\n\t" << Color::orange << "Вы отказались от кредита." << Color::reset << "\n";
        return false;
    }
    chips += amount;
    debt = amount;
    std::cout << "\n\t" << Color::brightGreen << "Вы взяли кредит на " << Color::gold << amount << Color::brightGreen << " фишек. " << Color::brightRed << "Долг: " << debt << " фишек." << Color::reset << "\n";
    return true;
}
void GameEngine::startGame(bool loadFromFile, const char* playerName) {//запуск игры
    system("cls");
    std::cout << "\n\t" << Color::brightCyan << "Запуск новой игры..." << Color::reset << "\n\n";
    int playerChips = START_CHIPS;
    int computerChips = 1000;
    int debt = 0;
    bool lastChanceUsed = false;
    Achievements achievements;
    char fileName[100];
    Profile::getSaveFileName(playerName, fileName);
    std::ifstream achievementsFile(fileName);
    if (achievementsFile.is_open()) {
        std::string line;
        std::getline(achievementsFile, line);
        std::getline(achievementsFile, line);
        for (int i = 0; i < 4; i++) std::getline(achievementsFile, line);
        std::getline(achievementsFile, line);
        achievementsFile >> achievements.novice >> achievements.lucky >> achievements.bankruptRescuer >> achievements.combinationKing >> achievements.millionaire >> achievements.winStreak;
        achievementsFile.close();
    }
    if (loadFromFile) {
        std::ifstream testFile(fileName);
        if (testFile.good()) {
            if (loadGame(playerName, playerChips, debt, computerChips, lastChanceUsed, achievements)) {
                std::cout << "\n\t" << Color::brightGreen << "Сохранение успешно загружено!" << Color::reset << "\n";

            }
            else {
                std::cout << "\n\t" << Color::brightRed << "Ошибка загрузки. " << Color::brightGreen << "Начинаем новую игру." << Color::reset << "\n";
                pause();
            }
        }
        else {
            std::cout << "\n\t" << Color::brightRed << "Сохранение для \"" << Color::gold << playerName << Color::brightRed << "\" не найдено. " << Color::brightGreen << "Начинаем новую игру." << Color::reset << "\n";
            pause();
        }
        testFile.close();
    }
    if (playerChips <= 0 && debt > 0 && lastChanceUsed) {//провера на банкротсво 
        system("cls");
        std::cout << "\n\t" << Color::brightRed << "███████████████████████████████████████████████████████\n";
        std::cout << "\t█" << Color::brightRed << Color::bold << "                                                     █\n";
        std::cout << Color::brightWhite << Color::bold << "\t█             И Г Р А   О К О Н Ч Е Н А               █\n";
        std::cout << "\t█" << Color::brightWhite << Color::bold << "                                                     █\n";
        std::cout << "\t███████████████████████████████████████████████████████" << Color::reset << "\n\n";

        std::cout << "\t" << Color::brightRed << "Вы объявлены банкротом!" << Color::reset << "\n";
        std::cout << "\t" << Color::orange << "В предыдущей сессии вы отказались от кредита при нулевом балансе." << Color::reset << "\n\n";

        std::cout << "\t" << Color::brightYellow << "Хотите начать новую игру с 100 фишками?" << Color::reset << "\n";
        std::cout << "\t" << Color::gray << "(Ваша статистика и достижения сохранятся!)" << Color::reset << "\n";
        std::cout << "\n\t" << Color::brightCyan << "(1 — да, восстановить баланс; 0 — нет, выйти в меню): " << Color::reset;

        int choice = InputValidator::safeInputNumber(0, 1);
        if (choice == 1) {
            playerChips = START_CHIPS; // 100 фишек
            debt = 0;
            computerChips = 1000;
            lastChanceUsed = false;
            std::cout << "\n\t" << Color::brightGreen << "Баланс восстановлен! Удачи за столом!" << Color::reset << "\n";
            pause();
        }
        else {
            std::cout << "\n\t" << Color::gray << "Возврат в главное меню..." << Color::reset << "\n";
            pause();
            return; // Выход в главное меню
        }
    }
    while (playerChips >= 0 && computerChips > 0) {//глав цикл игры
        if (playerChips <= 0 && debt > 0) {//провер банкротства
            if (!lastChanceUsed) {
                std::cout << "\n\t" << Color::brightRed << "У вас закончились фишки, но остался долг (" << Color::gold << debt << Color::brightRed << " фишек)!" << Color::reset << "\n";
                std::cout << "\t" << Color::orange << "Банк даёт вам " << Color::brightGreen << "ПОСЛЕДНИЙ ШАНС: " << LAST_CHANCE << " фишек." << Color::reset << "\n\n";
                std::cout << "\t" << Color::brightRed << "Если проиграете эту руку — игра окончена!" << Color::reset << "\n";
                std::cout << "\t" << Color::brightCyan << "Принять последний шанс? " << Color::gray << "(" << Color::brightGreen << "1 — да" << Color::gray << ", " << Color::brightRed << "0 — нет): " << Color::reset;
                int choice = InputValidator::safeInputNumber(0, 1);
                if (choice == 1) {
                    playerChips = LAST_CHANCE;
                    debt += LAST_CHANCE;
                    lastChanceUsed = true;
                    std::cout << "\n\t" << Color::brightGreen << "Вы получили " << Color::gold << LAST_CHANCE << Color::brightGreen << " фишек. " << Color::brightRed << "Новый долг: " << debt << " фишек." << Color::reset << "\n\n";
                }
                else {
                    std::cout << "\n\t" << Color::brightRed << "Вы отказались от последнего шанса." << Color::reset << "\n";
                    std::cout << "\t" << Color::brightRed << "Игра окончена. Банк забирает всё." << Color::reset << "\n";
                    saveGame(playerName, playerChips, debt, computerChips, lastChanceUsed, achievements);//сохр статус банкрота
                    pause();
                    return;
                }
            }
            else {
                std::cout << "\n\t" << Color::brightRed << "У вас нет фишек, долг не погашен, и последний шанс использован." << Color::reset << "\n";
                std::cout << "\t" << Color::brightRed << "Вы объявлены банкротом. Игра окончена." << Color::reset << "\n";
                saveGame(playerName, playerChips, debt, computerChips, lastChanceUsed, achievements);//сохр статус банкрота
                pause();
                return;
            }
       
        }
        if (playerChips <= 10 && debt == 0) {//предл  кредит
            std::cout << "\n\t" << Color::orange << "У вас мало фишек (" << Color::gold << playerChips << Color::orange << "). Хотите взять кредит?" << Color::reset << "\n";
            std::cout << "\t" << Color::gray << "(" << Color::brightGreen << "1 — да" << Color::gray << ", " << Color::brightRed << "0 — нет): " << Color::reset;
            int choice = InputValidator::safeInputNumber(0, 1);
            if (choice == 1) {
                takeCredit(playerChips, debt);
            }
            else {
                if (playerChips <= 0) {
                    std::cout << "\n\t" << Color::brightRed << "Вы отказались от кредита, а фишек нет." << Color::reset << "\n";
                    std::cout << "\t" << Color::brightRed << "Игра окончена." << Color::reset << "\n";
                    saveGame(playerName, playerChips, debt, computerChips, lastChanceUsed, achievements);//сохр статус банкрота
                    pause();
                    return;
                }
                else if (playerChips < SMALL_BLIND) {
                    std::cout << "\n\t" << Color::brightRed << "Вам не хватает фишек даже на малый блайнд." << Color::reset << "\n";
                    std::cout << "\t" << Color::brightRed << "Игра окончена." << Color::reset << "\n";
                    saveGame(playerName, playerChips, debt, computerChips, lastChanceUsed, achievements);//сохр статус банкрота
                    pause();
                    return;
                }

            }
            if (playerChips < SMALL_BLIND && debt > 0) {//если есть долг и не хватает на блайнд
                std::cout << "\n\t" << Color::brightRed << "У вас недостаточно фишек для продолжения игры." << Color::reset << "\n";
                std::cout << "\t" << Color::orange << "Долг: " << Color::gold << debt << Color::orange << " фишек." << Color::reset << "\n";
                std::cout << "\t" << Color::brightRed << "Игра окончена." << Color::reset << "\n";
                saveGame(playerName, playerChips, debt, computerChips, lastChanceUsed, achievements);//сохр статус
                pause();
                return;
            }
            if (playerChips < SMALL_BLIND) {//если фишек меньше блайнда
                std::cout << "\n\t" << Color::brightRed << "У вас недостаточно фишек для продолжения игры." << Color::reset << "\n";
                std::cout << "\t" << Color::brightRed << "Игра окончена." << Color::reset << "\n";
                saveGame(playerName, playerChips, debt, computerChips, lastChanceUsed, achievements);//сохр статус
                pause();
                return;
            }
        }
        std::cout << "\n\t" << Color::turquoise << "███████████████████████████████████████████████████████\n";
        std::cout << "\t█" << Color::turquoise << Color::bold << "                                                     █\n";
        std::cout << Color::brightWhite << Color::bold << "\t█              Н О В А Я   Р У К А                    █\n";
        std::cout << "\t█" << Color::brightWhite << Color::bold << "                                                     █\n";
        std::cout << "\t███████████████████████████████████████████████████████" << Color::reset << "\n\n";
        Deck deck;
        CustomStack<Card> playerHand(2);
        CustomStack<Card> computerHand(2);
        Card table[5];
        for (int i = 0; i < 2; i++) playerHand.push(deck.dealCard());//разд карт
        for (int i = 0; i < 2; i++) computerHand.push(deck.dealCard());
        for (int i = 0; i < 5; i++) table[i] = Card('X', 0);//пустой стол
        int bank = SMALL_BLIND + BIG_BLIND;//банк
        if (playerChips < SMALL_BLIND) {
            bank = playerChips + BIG_BLIND;
            computerChips -= BIG_BLIND;
            playerChips = 0;
        }
        else {
            playerChips -= SMALL_BLIND;
            computerChips -= BIG_BLIND;
        }
        std::cout << "\t" << Color::brightCyan << "Малый блайнд: " << Color::gold << SMALL_BLIND << Color::brightCyan << " (от вас)\n";
        std::cout << "\t" << Color::coral << "Большой блайнд: " << Color::gold << BIG_BLIND << Color::coral << " (от компьютера)\n";
        std::cout << "\t" << Color::gold << "В банке: " << bank << " фишек" << Color::reset << "\n\n";
        showHand(playerHand);
        bool allIn = false;
        bool handFinished = false;
        int handResult = 0;
        std::cout << "\n\t" << Color::brightCyan << "═══════ П Р Е Ф Л О П ═════" << Color::reset << "\n";//префлоп
        int differenceToCall = BIG_BLIND - SMALL_BLIND;
        int playerBet = 0;
        if (playerChips > 0) {
            std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек" << Color::reset;
            if (debt > 0) std::cout << " " << Color::brightRed << "(долг: " << debt << ")" << Color::reset;
            std::cout << "\n";
            playerBet = InputValidator::makeBet(playerChips, differenceToCall);
            if (playerBet == -1) {
                std::cout << "\n\t" << Color::brightRed << "Вы сдались. " << Color::coral << "Компьютер забирает банк: " << Color::gold << bank << Color::coral << " фишек." << Color::reset << "\n";
                computerChips += bank;
                handFinished = true;
            }
        }
        if (!handFinished && playerChips > 0) {
            if (differenceToCall > 0 && playerBet < differenceToCall) {//проверка на ал-ин префлоп
                if (playerBet >= playerChips) {//настоящий ал-ин (ставим все что есть)
                    std::cout << "\n\t" << Color::brightRed << Color::bold << "Вы идёте ALL-IN на " << playerBet << " фишек!" << Color::reset << "\n";
                    playerChips -= playerBet;
                    bank += playerBet;
                    int computerAddition = differenceToCall - playerBet;
                    computerChips -= computerAddition;
                    bank += computerAddition;
                    handFinished = true;
                    allIn = true;
                    for (int i = 0; i < 5; i++) table[i] = deck.dealCard();
                    std::cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                    showTable(table, 5);
                    handResult = showShowdown(playerHand, computerHand, table, bank, playerChips, computerChips);
                }
                else {//ставка меньше минимума, но не ал-ин - повторный ввод
                    std::cout << "\n\t" << Color::orange << "Ставка меньше минимума! Минимум: " << differenceToCall << " фишек. Повторите ввод." << Color::reset << "\n";
                    playerBet = InputValidator::makeBet(playerChips, differenceToCall);
                    if (playerBet == -1) {
                        std::cout << "\n\t" << Color::brightRed << "Вы сдались. " << Color::coral << "Компьютер забирает банк: " << Color::gold << bank << Color::coral << " фишек." << Color::reset << "\n";
                        computerChips += bank;
                        handFinished = true;
                    }
                    else {
                        playerChips -= playerBet;
                        bank += playerBet;
                        int computerBet = AIPlayer::makeDecision(computerChips, playerBet, computerHand, table, 0, playerChips);
                        if (computerBet > 0 && computerBet < playerBet) computerBet = playerBet;
                        if (computerBet == 0) {
                            std::cout << "\n\t" << Color::gray << "Компьютер сбрасывает карты.\n";
                            std::cout << "\n\t" << Color::brightGreen << Color::bold << "Вы выиграли банк: " << Color::gold << bank << Color::brightGreen << " фишек!" << Color::reset << "\n";
                            playerChips += bank;
                            handFinished = true;
                        }
                        else if (computerBet == playerBet) {
                            std::cout << "\n\t" << Color::coral << "Компьютер поддерживает вашу ставку: " << Color::gold << computerBet << Color::coral << " фишек." << Color::reset << "\n";
                            computerChips -= computerBet;
                            bank += computerBet;
                        }
                        else {
                            std::cout << "\n\t" << Color::orange << "Компьютер повышает ставку до " << Color::gold << computerBet << Color::orange << " фишек!" << Color::reset << "\n";
                            int additionalBet = computerBet - playerBet;
                            std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек";
                            if (debt > 0) std::cout << " " << Color::brightRed << "(долг: " << debt << ")" << Color::reset;
                            std::cout << "\n";
                            int playerResponse = InputValidator::makeBet(playerChips, additionalBet);
                            while (true) { // цикл валидации ставки
                                if (playerResponse == -1) { // пас
                                    std::cout << "\n\t" << Color::brightRed << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                                    computerChips += bank;
                                    handFinished = true;
                                    break;
                                }
                                else if (playerResponse > 0 && playerResponse < additionalBet && playerChips >= additionalBet) { // ставка меньше мин но фишки есть
                                    std::cout << "\n\t" << Color::brightRed << "Ошибка: минимальная ставка " << additionalBet << " фишек!" << Color::reset << "\n";
                                    std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек" << Color::reset << "\n";
                                    playerResponse = InputValidator::makeBet(playerChips, additionalBet);
                                }
                                else break; // корректная ставка или реальный ал-ин
                            }
                            if (!handFinished) {
                                if (playerResponse > 0 && playerResponse < additionalBet) { // реальный ал-ин (фишек меньше минимума)
                                    std::cout << "\n\t" << Color::brightRed << Color::bold << "Вы идёте ALL-IN на " << playerResponse << " фишек!" << Color::reset << "\n";
                                    playerChips -= playerResponse;
                                    bank += playerResponse;
                                    int computerAddition = additionalBet - playerResponse;
                                    computerChips -= computerAddition;
                                    bank += computerAddition;
                                    handFinished = true;
                                    allIn = true;
                                    for (int i = 0; i < 5; i++) if (table[i].getRank() == 0) table[i] = deck.dealCard();
                                    std::cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                                    showTable(table, 5);
                                    handResult = showShowdown(playerHand, computerHand, table, bank, playerChips, computerChips);
                                }
                                else if (playerResponse >= additionalBet) { // обычная ставка или рейз
                                    playerChips -= playerResponse;
                                    bank += playerResponse;
                                    int difference = playerResponse - additionalBet;
                                    if (difference > 0) {
                                        int computerAddition = std::min(difference, computerChips);
                                        computerChips -= computerAddition;
                                        bank += computerAddition;
                                        std::cout << "\n\t" << Color::coral << "Компьютер уравнивает: +" << Color::gold << computerAddition << Color::coral << " фишек." << Color::reset << "\n";
                                    }
                                    else {
                                        computerChips -= additionalBet;
                                        bank += additionalBet;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else {//обычная логика ставок префлоп
                playerChips -= playerBet;
                bank += playerBet;
                int computerBet = AIPlayer::makeDecision(computerChips, playerBet, computerHand, table, 0, playerChips);
                if (computerBet > 0 && computerBet < playerBet) computerBet = playerBet;
                if (computerBet == 0) {
                    std::cout << "\n\t" << Color::gray << "Компьютер сбрасывает карты.\n";
                    std::cout << "\n\t" << Color::brightGreen << Color::bold << "Вы выиграли банк: " << Color::gold << bank << Color::brightGreen << " фишек!" << Color::reset << "\n";
                    playerChips += bank;
                    handFinished = true;
                }
                else if (computerBet == playerBet) {
                    std::cout << "\n\t" << Color::coral << "Компьютер поддерживает вашу ставку: " << Color::gold << computerBet << Color::coral << " фишек." << Color::reset << "\n";
                    computerChips -= computerBet;
                    bank += computerBet;
                }
                else {
                    std::cout << "\n\t" << Color::orange << "Компьютер повышает ставку до " << Color::gold << computerBet << Color::orange << " фишек!" << Color::reset << "\n";
                    int additionalBet = computerBet - playerBet;
                    std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек";
                    if (debt > 0) std::cout << " " << Color::brightRed << "(долг: " << debt << ")" << Color::reset;
                    std::cout << "\n";
                    int playerResponse = InputValidator::makeBet(playerChips, additionalBet);
                    while (true) { //цикл валидации ставки
                        if (playerResponse == -1) { //пас
                            std::cout << "\n\t" << Color::brightRed << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                            computerChips += bank;
                            handFinished = true;
                            break;
                        }
                        else if (playerResponse > 0 && playerResponse < additionalBet && playerChips >= additionalBet) {//ставка меньше мин но фишки есть
                            std::cout << "\n\t" << Color::brightRed << "Ошибка: минимальная ставка " << additionalBet << " фишек!" << Color::reset << "\n";
                            std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек" << Color::reset << "\n";
                            playerResponse = InputValidator::makeBet(playerChips, additionalBet);
                        }
                        else break;//корректная ставка или реальный ал-ин
                    }
                    if (!handFinished) {
                        if (playerResponse > 0 && playerResponse < additionalBet) { //реальный ал-ин (фишек меньше минимума)
                            std::cout << "\n\t" << Color::brightRed << Color::bold << "Вы идёте ALL-IN на " << playerResponse << " фишек!" << Color::reset << "\n";
                            playerChips -= playerResponse;
                            bank += playerResponse;
                            int computerAddition = additionalBet - playerResponse;
                            computerChips -= computerAddition;
                            bank += computerAddition;
                            handFinished = true;
                            allIn = true;
                            for (int i = 0; i < 5; i++) if (table[i].getRank() == 0) table[i] = deck.dealCard();
                            std::cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                            showTable(table, 5);
                            handResult = showShowdown(playerHand, computerHand, table, bank, playerChips, computerChips);
                        }
                        else if (playerResponse >= additionalBet) { //обычная ставка или рейз
                            playerChips -= playerResponse;
                            bank += playerResponse;
                            int difference = playerResponse - additionalBet;
                            if (difference > 0) {
                                int computerAddition = std::min(difference, computerChips);
                                computerChips -= computerAddition;
                                bank += computerAddition;
                                std::cout << "\n\t" << Color::coral << "Компьютер уравнивает: +" << Color::gold << computerAddition << Color::coral << " фишек." << Color::reset << "\n";
                            }
                            else {
                                computerChips -= additionalBet;
                                bank += additionalBet;
                            }
                        }
                    }
                }
                if (!allIn && (playerChips == 0 || computerChips == 0)) {//ал-ин после ставок
                    allIn = true;
                    for (int i = 0; i < 5; i++) if (table[i].getRank() == 0) table[i] = deck.dealCard();
                    std::cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                    showTable(table, 5);
                    handResult = showShowdown(playerHand, computerHand, table, bank, playerChips, computerChips);
                }
            }
        }
        if (!handFinished && !allIn) {//флоп
            std::cout << "\n\t" << Color::turquoise << "═══════ Ф Л О П ═════" << Color::reset << "\n";
            for (int i = 0; i < 3; i++) table[i] = deck.dealCard();
            animateCardReveal();
            showTable(table, 3);
            std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек";
            if (debt > 0) std::cout << " " << Color::brightRed << "(долг: " << debt << ")" << Color::reset;
            std::cout << "\n";
            playerBet = InputValidator::makeBet(playerChips, 0);
            if (playerBet == -1) {
                std::cout << "\n\t" << Color::brightRed << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                computerChips += bank;
                handFinished = true;
            }
            if (!handFinished) {
                playerChips -= playerBet;
                bank += playerBet;
                int computerBet = AIPlayer::makeDecision(computerChips, playerBet, computerHand, table, 3, playerChips);
                if (computerBet > 0 && computerBet < playerBet) computerBet = playerBet;
                if (computerBet == 0) {
                    std::cout << "\n\t" << Color::gray << "Компьютер сбрасывает карты.\n";
                    std::cout << "\n\t" << Color::brightGreen << Color::bold << "Вы выиграли банк: " << Color::gold << bank << Color::brightGreen << " фишек!" << Color::reset << "\n";
                    playerChips += bank;
                    handFinished = true;
                }
                else if (computerBet == playerBet) {
                    std::cout << "\n\t" << Color::coral << "Компьютер поддерживает вашу ставку: " << Color::gold << computerBet << Color::coral << " фишек." << Color::reset << "\n";
                    computerChips -= computerBet;
                    bank += computerBet;
                }
                else {
                    std::cout << "\n\t" << Color::orange << "Компьютер повышает ставку до " << Color::gold << computerBet << Color::orange << " фишек!" << Color::reset << "\n";
                    int additionalBet = computerBet - playerBet;
                    std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек";
                    if (debt > 0) std::cout << " " << Color::brightRed << "(долг: " << debt << ")" << Color::reset;
                    std::cout << "\n";
                    int playerResponse = InputValidator::makeBet(playerChips, additionalBet);
                    while (true) {//цикл валидации ставки
                        if (playerResponse == -1) {//пас
                            std::cout << "\n\t" << Color::brightRed << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                            computerChips += bank;
                            handFinished = true;
                            break;
                        }
                        else if (playerResponse > 0 && playerResponse < additionalBet && playerChips >= additionalBet) {//ставка меньше мин но фишки есть
                            std::cout << "\n\t" << Color::brightRed << "Ошибка: минимальная ставка " << additionalBet << " фишек!" << Color::reset << "\n";
                            std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек" << Color::reset << "\n";
                            playerResponse = InputValidator::makeBet(playerChips, additionalBet);
                        }
                        else break;//корректная ставка или реальный ал-ин
                    }
                    if (!handFinished) {
                        if (playerResponse > 0 && playerResponse < additionalBet) {//реальный ал-ин (фишек меньше минимума)
                            std::cout << "\n\t" << Color::brightRed << Color::bold << "Вы идёте ALL-IN на " << playerResponse << " фишек!" << Color::reset << "\n";
                            playerChips -= playerResponse;
                            bank += playerResponse;
                            int computerAddition = additionalBet - playerResponse;
                            computerChips -= computerAddition;
                            bank += computerAddition;
                            handFinished = true;
                            allIn = true;
                            for (int i = 3; i < 5; i++) if (table[i].getRank() == 0) table[i] = deck.dealCard();
                            std::cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                            showTable(table, 5);
                            handResult = showShowdown(playerHand, computerHand, table, bank, playerChips, computerChips);
                        }
                        else if (playerResponse >= additionalBet) {//обычная ставка или рейз
                            playerChips -= playerResponse;
                            bank += playerResponse;
                            int difference = playerResponse - additionalBet;
                            if (difference > 0) {
                                int computerAddition = std::min(difference, computerChips);
                                computerChips -= computerAddition;
                                bank += computerAddition;
                                std::cout << "\n\t" << Color::coral << "Компьютер уравнивает: +" << Color::gold << computerAddition << Color::coral << " фишек." << Color::reset << "\n";
                            }
                            else {
                                computerChips -= additionalBet;
                                bank += additionalBet;
                            }
                        }
                    }
                }
                if (!allIn && (playerChips == 0 || computerChips == 0)) {
                    allIn = true;
                    for (int i = 3; i < 5; i++) table[i] = deck.dealCard();
                    std::cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                    showTable(table, 5);
                    handResult = showShowdown(playerHand, computerHand, table, bank, playerChips, computerChips);
                }
            }
        }
        if (!handFinished && !allIn) {//тёрн
            std::cout << "\n\t" << Color::orange << "═══════ Т Ё Р Н ═════" << Color::reset << "\n";
            table[3] = deck.dealCard();
            animateCardReveal();
            showTable(table, 4);
            std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек";
            if (debt > 0) std::cout << " " << Color::brightRed << "(долг: " << debt << ")" << Color::reset;
            std::cout << "\n";
            playerBet = InputValidator::makeBet(playerChips, 0);
            if (playerBet == -1) {
                std::cout << "\n\t" << Color::brightRed << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                computerChips += bank;
                handFinished = true;
            }
            if (!handFinished) {
                playerChips -= playerBet;
                bank += playerBet;
                int computerBet = AIPlayer::makeDecision(computerChips, playerBet, computerHand, table, 4, playerChips);
                if (computerBet > 0 && computerBet < playerBet) computerBet = playerBet;
                if (computerBet == 0) {
                    std::cout << "\n\t" << Color::gray << "Компьютер сбрасывает карты.\n";
                    std::cout << "\n\t" << Color::brightGreen << Color::bold << "Вы выиграли банк: " << Color::gold << bank << Color::brightGreen << " фишек!" << Color::reset << "\n";
                    playerChips += bank;
                    handFinished = true;
                }
                else if (computerBet == playerBet) {
                    std::cout << "\n\t" << Color::coral << "Компьютер поддерживает вашу ставку: " << Color::gold << computerBet << Color::coral << " фишек." << Color::reset << "\n";
                    computerChips -= computerBet;
                    bank += computerBet;
                }
                else {
                    std::cout << "\n\t" << Color::orange << "Компьютер повышает ставку до " << Color::gold << computerBet << Color::orange << " фишек!" << Color::reset << "\n";
                    int additionalBet = computerBet - playerBet;
                    std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек";
                    if (debt > 0) std::cout << " " << Color::brightRed << "(долг: " << debt << ")" << Color::reset;
                    std::cout << "\n";
                    int playerResponse = InputValidator::makeBet(playerChips, additionalBet);
                    while (true) {//цикл валидации ставки
                        if (playerResponse == -1) {//пас
                            std::cout << "\n\t" << Color::brightRed << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                            computerChips += bank;
                            handFinished = true;
                            break;
                        }
                        else if (playerResponse > 0 && playerResponse < additionalBet && playerChips >= additionalBet) {//ставка меньше мин но фишки есть
                            std::cout << "\n\t" << Color::brightRed << "Ошибка: минимальная ставка " << additionalBet << " фишек!" << Color::reset << "\n";
                            std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек" << Color::reset << "\n";
                            playerResponse = InputValidator::makeBet(playerChips, additionalBet);
                        }
                        else break;//корректная ставка или реальный ал-ин
                    }
                    if (!handFinished) {
                        if (playerResponse > 0 && playerResponse < additionalBet) {//реальный ал-ин (фишек меньше минимума)
                            std::cout << "\n\t" << Color::brightRed << Color::bold << "Вы идёте ALL-IN на " << playerResponse << " фишек!" << Color::reset << "\n";
                            playerChips -= playerResponse;
                            bank += playerResponse;
                            int computerAddition = additionalBet - playerResponse;
                            computerChips -= computerAddition;
                            bank += computerAddition;
                            handFinished = true;
                            allIn = true;
                            table[4] = deck.dealCard();
                            std::cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                            showTable(table, 5);
                            handResult = showShowdown(playerHand, computerHand, table, bank, playerChips, computerChips);
                        }
                        else if (playerResponse >= additionalBet) {//обычная ставка или рейз
                            playerChips -= playerResponse;
                            bank += playerResponse;
                            int difference = playerResponse - additionalBet;
                            if (difference > 0) {
                                int computerAddition = std::min(difference, computerChips);
                                computerChips -= computerAddition;
                                bank += computerAddition;
                                std::cout << "\n\t" << Color::coral << "Компьютер уравнивает: +" << Color::gold << computerAddition << Color::coral << " фишек." << Color::reset << "\n";
                            }
                            else {
                                computerChips -= additionalBet;
                                bank += additionalBet;
                            }
                        }
                    }
                }
                if (!allIn && (playerChips == 0 || computerChips == 0)) {
                    allIn = true;
                    table[4] = deck.dealCard();
                    std::cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                    showTable(table, 5);
                    handResult = showShowdown(playerHand, computerHand, table, bank, playerChips, computerChips);
                }
            }
        }
        if (!handFinished && !allIn) {//ривер
            std::cout << "\n\t" << Color::brightBlue << "═══════ Р И В Е Р ═════" << Color::reset << "\n";
            table[4] = deck.dealCard();
            animateCardReveal();
            showTable(table, 5);
            std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек";
            if (debt > 0) std::cout << " " << Color::brightRed << "(долг: " << debt << ")" << Color::reset;
            std::cout << "\n";
            playerBet = InputValidator::makeBet(playerChips, 0);
            if (playerBet == -1) {
                std::cout << "\n\t" << Color::brightRed << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                computerChips += bank;
                handFinished = true;
            }
            if (!handFinished) {
                playerChips -= playerBet;
                bank += playerBet;
                int computerBet = AIPlayer::makeDecision(computerChips, playerBet, computerHand, table, 5, playerChips);
                if (computerBet > 0 && computerBet < playerBet) computerBet = playerBet;
                if (computerBet == 0) {
                    std::cout << "\n\t" << Color::gray << "Компьютер сбрасывает карты.\n";
                    std::cout << "\n\t" << Color::brightGreen << Color::bold << "Вы выиграли банк: " << Color::gold << bank << Color::brightGreen << " фишек!" << Color::reset << "\n";
                    playerChips += bank;
                    handFinished = true;
                }
                else if (computerBet == playerBet) {
                    std::cout << "\n\t" << Color::coral << "Компьютер поддерживает вашу ставку: " << Color::gold << computerBet << Color::coral << " фишек." << Color::reset << "\n";
                    computerChips -= computerBet;
                    bank += computerBet;
                }
                else {
                    std::cout << "\n\t" << Color::orange << "Компьютер повышает ставку до " << Color::gold << computerBet << Color::orange << " фишек!" << Color::reset << "\n";
                    int additionalBet = computerBet - playerBet;
                    std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек";
                    if (debt > 0) std::cout << " " << Color::brightRed << "(долг: " << debt << ")" << Color::reset;
                    std::cout << "\n";
                    int playerResponse = InputValidator::makeBet(playerChips, additionalBet);
                    while (true) {//цикл валидации ставки
                        if (playerResponse == -1) {//пас
                            std::cout << "\n\t" << Color::brightRed << "Вы сдались. Компьютер забирает банк." << Color::reset << "\n";
                            computerChips += bank;
                            handFinished = true;
                            break;
                        }
                        else if (playerResponse > 0 && playerResponse < additionalBet && playerChips >= additionalBet) {//ставка меньше мин но фишки есть
                            std::cout << "\n\t" << Color::brightRed << "Ошибка: минимальная ставка " << additionalBet << " фишек!" << Color::reset << "\n";
                            std::cout << "\n\t" << Color::lime << "Ваш счёт: " << playerChips << " фишек" << Color::reset << "\n";
                            playerResponse = InputValidator::makeBet(playerChips, additionalBet);
                        }
                        else break;//корректная ставка или реальный ал-ин
                    }
                    if (!handFinished) {
                        if (playerResponse > 0 && playerResponse < additionalBet) {//реальный ал-ин (фишек меньше минимума)
                            std::cout << "\n\t" << Color::brightRed << Color::bold << "Вы идёте ALL-IN на " << playerResponse << " фишек!" << Color::reset << "\n";
                            playerChips -= playerResponse;
                            bank += playerResponse;
                            int computerAddition = additionalBet - playerResponse;
                            computerChips -= computerAddition;
                            bank += computerAddition;
                            handFinished = true;
                            allIn = true;
                            std::cout << "\n\t" << Color::cyan << "ALL-IN! Все карты открыты:" << Color::reset << "\n";
                            showTable(table, 5);
                            handResult = showShowdown(playerHand, computerHand, table, bank, playerChips, computerChips);
                        }
                        else if (playerResponse >= additionalBet) {//обычная ставка или рейз
                            playerChips -= playerResponse;
                            bank += playerResponse;
                            int difference = playerResponse - additionalBet;
                            if (difference > 0) {
                                int computerAddition = std::min(difference, computerChips);
                                computerChips -= computerAddition;
                                bank += computerAddition;
                                std::cout << "\n\t" << Color::coral << "Компьютер уравнивает: +" << Color::gold << computerAddition << Color::coral << " фишек." << Color::reset << "\n";
                            }
                            else {
                                computerChips -= additionalBet;
                                bank += additionalBet;
                            }
                        }
                    }
                }
            }
        }
        if (!handFinished && !allIn) handResult = showShowdown(playerHand, computerHand, table, bank, playerChips, computerChips);
        std::string playerCombination = std::string(HandEvaluator::describeBestHand(playerHand, table, 5));
        checkAchievements(achievements, playerChips, debt, handResult, playerCombination);

        std::cout << "\n\t" << Color::lime << "^-^ Ваш счёт: " << playerChips << " фишек";
        if (debt > 0) std::cout << " " << Color::brightRed << "(долг: " << debt << ")" << Color::reset;
        std::cout << "\n\t" << Color::coral << "^~^ Счёт компьютера: " << computerChips << " фишек" << Color::reset << "\n\n";

        if (computerChips <= 0) {
            std::cout << "\n\t" << Color::brightCyan << "Компьютер обанкротился! " << Color::brightGreen << Color::bold << "🏆 Вы победили!" << Color::reset << "\n";
            pause();
            break;
        }

        if (debt > 0 && playerChips > 0) {//погаш долга
            std::cout << "\n\t" << Color::brightCyan << "Хотите погасить часть долга? " << Color::gray << "(" << Color::brightGreen << "1 — да" << Color::gray << ", " << Color::brightRed << "0 — нет): " << Color::reset;
            if (InputValidator::safeInputNumber(0, 1) == 1) {
                int amount = std::min(playerChips, debt);
                int oldDebt = debt;
                playerChips -= amount;
                debt -= amount;
                std::cout << "\n\t" << Color::brightYellow << "Вы погасили " << Color::gold << amount << Color::brightYellow << " фишек долга." << Color::reset << "\n";
                if (debt == 0) {
                    std::cout << "\t" << Color::brightGreen << "Долг полностью погашен!" << Color::reset << "\n";
                    lastChanceUsed = false;
                    if (oldDebt == MAX_DEBT && !achievements.bankruptRescuer) {
                        achievements.bankruptRescuer = true;
                        std::cout << "\n\t" << Color::brightMagenta << Color::bold << "🏆 НОВОЕ ДОСТИЖЕНИЕ: Банкрот-спаситель!" << Color::reset << "\n";
                    }
                }
                else std::cout << "\t" << Color::orange << "Остаток долга: " << Color::gold << debt << Color::orange << " фишек." << Color::reset << "\n";
            }
        }

        std::cout << "\n\t" << Color::turquoise << "─────────────────────────────────────────────────────" << Color::reset << "\n";
        std::cout << "\t" << Color::brightGreen << "1. Сыграть ещё" << Color::reset << "\n";
        std::cout << "\t" << Color::gold << "2. Сохранить и выйти" << Color::reset << "\n";
        std::cout << "\t" << Color::gray << "0. Выйти без сохранения" << Color::reset << "\n";
        std::cout << "\n\t" << Color::brightCyan << "Ваш выбор: " << Color::reset;
        int choice = InputValidator::safeInputNumber(0, 2);
        if (choice == 2) {
            saveGame(playerName, playerChips, debt, computerChips, lastChanceUsed, achievements);
            break;
        }
        else if (choice == 0) {
            saveGame(playerName, playerChips, debt, computerChips, lastChanceUsed, achievements);
            break;
        }
        system("cls");
    }
}