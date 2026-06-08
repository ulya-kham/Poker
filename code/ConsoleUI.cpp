#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <limits>
#include <cstring>
#include <thread>
#include <chrono>
#include "ConsoleUI.h"
void pause() {//пауза до энтера
    std::cout << Color::brightYellow << "\n\tДля продолжения нажмите Enter... " << Color::reset;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
void showCard(const Card& card, bool faceDown) {//вывод одной карты
    if (faceDown) {
        std::cout << "\t" << Color::gray << "┌─────────┐\n";
        std::cout << "\t│??       │\n";
        std::cout << "\t│         │\n";
        std::cout << "\t│   ???   │\n";
        std::cout << "\t│         │\n";
        std::cout << "\t│       ??│\n";
        std::cout << "\t└─────────┘" << Color::reset << "\n";
        return;
    }
    const char* rank = card.rankToString();
    const char* suit = card.suitToSymbol();
    const char* suitColor;//цвет масти
    switch (card.getSuit()) {
    case 'S': suitColor = Color::spade; break;
    case 'H': suitColor = Color::heart; break;
    case 'D': suitColor = Color::heart; break;
    case 'C': suitColor = Color::club; break;
    default: suitColor = Color::white;
    }
    std::cout << "\t" << suitColor << "┌─────────┐\n";
    if (card.getRank() == 10) std::cout << "\t│10" << suit << "      │\n";
    else std::cout << "\t│" << rank << " " << suit << "      │\n";
    std::cout << "\t│         │\n";
    std::cout << "\t│    " << suit << "    │\n";
    std::cout << "\t│         │\n";
    if (card.getRank() == 10) std::cout << "\t│      " << suit << "10│\n";
    else std::cout << "\t│      " << suit << " " << rank << "│\n";
    std::cout << "\t└─────────┘" << Color::reset << "\n";
}
void showHand(const CustomStack<Card>& hand) {//вывод руки игрока
    int size = hand.getSize();
    if (size <= 0) return;
    const char* suitColors[5];
    const char* rankStr[5];
    const char* suitStr[5];
    for (int i = 0; i < size; i++) {//заполн массивов
        rankStr[i] = hand[i].rankToString();
        suitStr[i] = hand[i].suitToSymbol();
        switch (hand[i].getSuit()) {//цвет масти
        case 'S': suitColors[i] = Color::spade; break;
        case 'H': suitColors[i] = Color::heart; break;
        case 'D': suitColors[i] = Color::heart; break;
        case 'C': suitColors[i] = Color::club; break;
        default: suitColors[i] = Color::white;
        }
    }
    for (int i = 0; i < size; i++) std::cout << "\t" << suitColors[i] << "┌─────────┐  ";
    std::cout << Color::reset << "\n";
    for (int i = 0; i < size; i++) {
        std::cout << "\t" << suitColors[i] << "│";
        if (hand[i].getRank() == 10) std::cout << "10" << suitStr[i] << "      │  ";
        else std::cout << rankStr[i] << " " << suitStr[i] << "      │  ";
    }
    std::cout << Color::reset << "\n";
    for (int i = 0; i < size; i++) std::cout << "\t" << suitColors[i] << "│         │  ";
    std::cout << Color::reset << "\n";
    for (int i = 0; i < size; i++) std::cout << "\t" << suitColors[i] << "│    " << suitStr[i] << "    │  ";
    std::cout << Color::reset << "\n";
    for (int i = 0; i < size; i++) std::cout << "\t" << suitColors[i] << "│         │  ";
    std::cout << Color::reset << "\n";
    for (int i = 0; i < size; i++) {
        std::cout << "\t" << suitColors[i] << "│";
        if (hand[i].getRank() == 10) std::cout << "      " << suitStr[i] << "10│  ";
        else std::cout << "      " << suitStr[i] << " " << rankStr[i] << "│  ";
    }
    std::cout << Color::reset << "\n";
    for (int i = 0; i < size; i++) std::cout << "\t" << suitColors[i] << "└─────────┘  ";
    std::cout << Color::reset << "\n\n";
}
void showTable(const Card table[], int revealed) {//вывод стола
    std::cout << "\t" << Color::turquoise << "═══════════════════════════════════════════════════════" << Color::reset << "\n\n";
    const char* suitColors[5];
    const char* rankStr[5];
    const char* suitStr[5];
    for (int i = 0; i < 5; i++) {
        if (i < revealed) {
            rankStr[i] = table[i].rankToString();
            suitStr[i] = table[i].suitToSymbol();
            switch (table[i].getSuit()) {
            case 'S': suitColors[i] = Color::spade; break;
            case 'H': suitColors[i] = Color::heart; break;
            case 'D': suitColors[i] = Color::heart; break;
            case 'C': suitColors[i] = Color::club; break;
            default: suitColors[i] = Color::white;
            }
        }
    }
    for (int i = 0; i < 5; i++) {//строка 1
        if (i < revealed) std::cout << "\t" << suitColors[i] << "┌─────────┐  ";
        else std::cout << "\t" << Color::gray << "┌─────────┐  ";
    }
    std::cout << Color::reset << "\n";
    for (int i = 0; i < 5; i++) {//строка 2
        if (i < revealed) {
            std::cout << "\t" << suitColors[i] << "│";
            if (table[i].getRank() == 10) std::cout << "10" << suitStr[i] << "      │  ";
            else std::cout << rankStr[i] << " " << suitStr[i] << "      │  ";
        }
        else std::cout << "\t" << Color::gray << "│??       │  ";
    }
    std::cout << Color::reset << "\n";
    for (int i = 0; i < 5; i++) {//строка 3
        if (i < revealed) std::cout << "\t" << suitColors[i] << "│         │  ";
        else std::cout << "\t" << Color::gray << "│         │  ";
    }
    std::cout << Color::reset << "\n";
    for (int i = 0; i < 5; i++) {//строка 4
        if (i < revealed) std::cout << "\t" << suitColors[i] << "│    " << suitStr[i] << "    │  ";
        else std::cout << "\t" << Color::gray << "│   ???   │  ";
    }
    std::cout << Color::reset << "\n";
    for (int i = 0; i < 5; i++) {//строка 5
        if (i < revealed) std::cout << "\t" << suitColors[i] << "│         │  ";
        else std::cout << "\t" << Color::gray << "│         │  ";
    }
    std::cout << Color::reset << "\n";
    for (int i = 0; i < 5; i++) {//строка 6
        if (i < revealed) {
            std::cout << "\t" << suitColors[i] << "│";
            if (table[i].getRank() == 10) std::cout << "      " << suitStr[i] << "10│  ";
            else std::cout << "      " << suitStr[i] << " " << rankStr[i] << "│  ";
        }
        else std::cout << "\t" << Color::gray << "│       ??│  ";
    }
    std::cout << Color::reset << "\n";
    for (int i = 0; i < 5; i++) {//строка 7
        if (i < revealed) std::cout << "\t" << suitColors[i] << "└─────────┘  ";
        else std::cout << "\t" << Color::gray << "└─────────┘  ";
    }
    std::cout << Color::reset << "\n\n";
}
void drawTableAtPosition(const Card table[], int revealed, int startRow) {//живой стол
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    const char* suitColors[5];
    const char* rankStr[5];
    const char* suitStr[5];
    for (int i = 0; i < 5; i++) {
        if (i < revealed) {
            rankStr[i] = table[i].rankToString();
            suitStr[i] = table[i].suitToSymbol();
            switch (table[i].getSuit()) {
            case 'S': suitColors[i] = Color::spade; break;
            case 'H': suitColors[i] = Color::heart; break;
            case 'D': suitColors[i] = Color::heart; break;
            case 'C': suitColors[i] = Color::club; break;
            default: suitColors[i] = Color::white;
            }
        }
    }
    coord.X = 0; coord.Y = static_cast<SHORT>(startRow);
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "\t";
    for (int i = 0; i < 5; i++) {
        if (i < revealed) std::cout << suitColors[i] << "┌─────────┐  ";
        else std::cout << Color::gray << "┌─────────┐  ";
    }
    std::cout << Color::reset << "                    ";
    coord.Y = static_cast<SHORT>(startRow + 1);
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "\t";
    for (int i = 0; i < 5; i++) {
        if (i < revealed) {
            std::cout << suitColors[i] << "│";
            if (table[i].getRank() == 10) std::cout << "10" << suitStr[i] << "      │  ";
            else std::cout << rankStr[i] << " " << suitStr[i] << "      │  ";
        }
        else std::cout << Color::gray << "│??       │  ";
    }
    std::cout << Color::reset << "                    ";
    coord.Y = static_cast<SHORT>(startRow + 2);
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "\t";
    for (int i = 0; i < 5; i++) {
        if (i < revealed) std::cout << suitColors[i] << "│         │  ";
        else std::cout << Color::gray << "│         │  ";
    }
    std::cout << Color::reset << "                    ";
    coord.Y = static_cast<SHORT>(startRow + 3);
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "\t";
    for (int i = 0; i < 5; i++) {
        if (i < revealed) std::cout << suitColors[i] << "│    " << suitStr[i] << "    │  ";
        else std::cout << Color::gray << "│   ???   │  ";
    }
    std::cout << Color::reset << "                    ";
    coord.Y = static_cast<SHORT>(startRow + 4);
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "\t";
    for (int i = 0; i < 5; i++) {
        if (i < revealed) std::cout << suitColors[i] << "│         │  ";
        else std::cout << Color::gray << "│         │  ";
    }
    std::cout << Color::reset << "                    ";
    coord.Y = static_cast<SHORT>(startRow + 5);
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "\t";
    for (int i = 0; i < 5; i++) {
        if (i < revealed) {
            std::cout << suitColors[i] << "│";
            if (table[i].getRank() == 10) std::cout << "      " << suitStr[i] << "10│  ";
            else std::cout << "      " << suitStr[i] << " " << rankStr[i] << "│  ";
        }
        else std::cout << Color::gray << "│       ??│  ";
    }
    std::cout << Color::reset << "                    ";
    coord.Y = static_cast<SHORT>(startRow + 6);
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "\t";
    for (int i = 0; i < 5; i++) {
        if (i < revealed) std::cout << suitColors[i] << "└─────────┘  ";
        else std::cout << Color::gray << "└─────────┘  ";
    }
    std::cout << Color::reset << "                    ";
}
void clearTableArea(int startRow, int rowCount) {//очистка области стола
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    for (int row = 0; row < rowCount; row++) {
        coord.X = 0;
        coord.Y = static_cast<SHORT>(startRow + row);
        SetConsoleCursorPosition(hConsole, coord);
        std::cout << "\t                                                                                ";
    }
}
void showRules() {//вывод правил игры
    system("cls");
    std::cout << "\n\t" << Color::pink << "███████████████████████████████████████████████████████\n";
    std::cout << "\t█" << Color::pink << Color::bold << "                                                     █\n";
    std::cout << Color::brightWhite << Color::bold << "\t█                    П Р А В И Л А                    █\n";
    std::cout << "\t█" << Color::brightWhite << Color::bold << "                                                     █\n";
    std::cout << "\t███████████████████████████████████████████████████████" << Color::reset << "\n\n";
    std::cout << "\t" << Color::brightYellow << "Техасский Холдем" << Color::reset << " — самая популярная разновидность покера.\n\n";
    std::cout << "\t" << Color::brightCyan << "~ " << Color::brightWhite << "У вас на руках 2 " << Color::gray << "закрытые карты." << Color::reset << "\n";
    std::cout << "\t" << Color::brightCyan << "~ " << Color::brightWhite << "На стол выкладываются 5 общих карт\n";
    std::cout << "\t   " << Color::gray << "(сначала три (флоп), затем ещё одна (тёрн) и последняя (ривер)).\n";
    std::cout << "\t" << Color::brightCyan << "~ " << Color::brightWhite << "Ваша цель — собрать лучшую комбинацию из 5 карт,\n";
    std::cout << "\t   " << Color::gray << "используя любые из своих и общих.\n\n";
    std::cout << "\t" << Color::gold << "🂱 " << Color::brightWhite << "Ставки делаются по кругу. Вы можете:\n";
    std::cout << "\t   " << Color::gray << "[0] — сдаться (пас),\n";
    std::cout << "\t   " << Color::gray << "[x] — уравнять ставку (колл),\n";
    std::cout << "\t   " << Color::gray << "[>x] — повысить (рейз),\n";
    std::cout << "\t   " << Color::gray << "[ALL-IN] — поставить все фишки.\n\n";
    std::cout << "\t" << Color::brightMagenta << "🏆 " << Color::brightWhite << "Комбинации (от сильной к слабой):\n";
    std::cout << "\t   " << Color::gold << "1. Роял-флеш" << Color::gray << " (10-J-Q-K-A в одной масти)\n";
    std::cout << "\t   " << Color::brightMagenta << "2. Стрит-флеш" << Color::gray << " (стрит в одной масти)\n";
    std::cout << "\t   " << Color::brightRed << "3. Каре" << Color::gray << " (четыре одинаковых)\n";
    std::cout << "\t   " << Color::brightGreen << "4. Фулл-хаус" << Color::gray << " (тройка + пара)\n";
    std::cout << "\t   " << Color::brightCyan << "5. Флеш" << Color::gray << " (одна масть)\n";
    std::cout << "\t   " << Color::brightBlue << "6. Стрит" << Color::gray << " (5 подряд)\n";
    std::cout << "\t   " << Color::orange << "7. Тройка\n";
    std::cout << "\t   " << Color::lime << "8. Две пары\n";
    std::cout << "\t   " << Color::pink << "9. Пара\n";
    std::cout << "\t   " << Color::brightWhite << "10. Старшая карта\n\n";
    std::cout << "\t" << Color::brightGreen << ":) Удачи за игровым столом!" << Color::reset << "\n";
    pause();
}
void endGame() {//сообщ об окончании игры
    system("cls");
    std::cout << "\n\t" << Color::turquoise << "███████████████████████████████████████████████████████\n";
    std::cout << "\t█" << Color::turquoise << Color::bold << "                                                     █\n";
    std::cout << Color::brightWhite << Color::bold << "\t█            С П А С И Б О   З А   И Г Р У !          █\n";
    std::cout << "\t█" << Color::brightWhite << Color::bold << "                                                     █\n";
    std::cout << "\t███████████████████████████████████████████████████████" << Color::reset << "\n\n";
    std::cout << "\t" << Color::turquoise << "   Собираем колоду" << Color::reset;
    for (int i = 0; i < 3; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        std::cout << Color::brightCyan << "." << Color::reset;
    }
    std::cout << "\n\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(300));


    std::cout << "\t" << Color::turquoise << "═══════════════════════════════════════════════════════\n";
    std::cout << "\t" << Color::brightYellow << "   До новых встреч за покерным столом! 🂡 🂱 🃁 🃑 \n";
    std::cout << "\t" << Color::turquoise << "═══════════════════════════════════════════════════════" << Color::reset << "\n\n\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}
void drawProgressBar(int current, int max, int barWidth, const char* label) {//прогресс-бар
    if (max <= 0) max = 1;
    int filled = (current * barWidth) / max;
    if (filled > barWidth) filled = barWidth;
    if (filled < 0) filled = 0;
    if (label) std::cout << "\t" << label << " ";
    std::cout << Color::brightGreen;
    for (int i = 0; i < filled; i++) std::cout << "█";
    std::cout << Color::gray;
    for (int i = filled; i < barWidth; i++) std::cout << "░";
    std::cout << Color::reset << " " << current << "/" << max << "\n";
}
void drawBoxedHeader(const char* title) {//рамка для заголовка
    int len = static_cast<int>(strlen(title));
    std::cout << "\n\t" << Color::turquoise << "╔";
    for (int i = 0; i < len + 2; i++) std::cout << "═";
    std::cout << "╗\n";
    std::cout << "\t║ " << Color::brightYellow << title << Color::turquoise << " ║\n";
    std::cout << "\t╚";
    for (int i = 0; i < len + 2; i++) std::cout << "═";
    std::cout << "╝" << Color::reset << "\n\n";
}
void animateCardReveal() {//аним открытия карты
    std::cout << "\n\t" << Color::brightYellow << "Открываем карту" << Color::reset;
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    std::cout << "\n";
}