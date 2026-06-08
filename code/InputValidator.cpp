#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <iostream>
#include <limits>
#include "InputValidator.h"
#include "ConsoleUI.h"
#include "Exceptions.h"
int InputValidator::safeInputNumber(int min, int max) {//безопас ввод числа
    int number;
    while (true) {
        std::cin >> number;
        if (std::cin.fail() || number < min || number > max) {//проверка корректности
            std::cin.clear();//сброс флага
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//очист буфера
            std::cout << "\t" << Color::brightRed << "Ошибка! " << Color::orange
                << "Введите число от " << min << " до " << max << ": " << Color::reset;
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//очист буфера
            return number;
        }
    }
}
int InputValidator::makeBet(int chips, int minBet) {//ввод ставки
    while (true) {
        if (minBet == 0) {//если мин ставка 0
            std::cout << "\t" << Color::brightCyan << "Введите ставку " << Color::gray << "(0 — пас): " << Color::reset;
        }
        else {
            std::cout << "\t" << Color::brightCyan << "Мин ставка: " << Color::gold << minBet
                << Color::brightCyan << " фишек. Введите ставку " << Color::gray << "(0 — пас): " << Color::reset;
        }
        int bet;
        std::cin >> bet;
        if (std::cin.fail()) {//если ввели не число
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n\t" << Color::brightRed << "Ошибка: введите число." << Color::reset << "\n";
            continue;
        }
        if (bet == 0) {//пас
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return -1;
        }
        if (bet < 0) {//отрицательная ставка
            std::cout << "\n\t" << Color::brightRed << "Ставка не может быть отрицательной!" << Color::reset << "\n";
            continue;
        }
        if (bet > chips) {//ставка больше фишек
            std::cout << "\n\t" << Color::orange << "У вас только " << Color::gold << chips
                << Color::orange << " фишек!" << Color::reset << "\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return bet;
    }
}