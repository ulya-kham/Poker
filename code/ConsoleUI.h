#pragma once
#include <iostream>
#include "Card.h"
#include "CustomStack.h"
namespace Color {//ansi цвета для консоли
    inline const char* reset = "\033[0m";//сброс
    inline const char* black = "\033[30m";//чёрный
    inline const char* red = "\033[31m";//красный
    inline const char* green = "\033[32m";//зелёный
    inline const char* yellow = "\033[33m";//жёлтый
    inline const char* blue = "\033[34m";//синий
    inline const char* magenta = "\033[35m";//пурпурный
    inline const char* cyan = "\033[36m";//циановый
    inline const char* white = "\033[37m";//белый
    inline const char* gray = "\033[90m";//серый
    inline const char* brightRed = "\033[91m";//ярко-красный
    inline const char* brightGreen = "\033[92m";//ярко-зеленый
    inline const char* brightYellow = "\033[93m";//ярко-жетый
    inline const char* brightBlue = "\033[94m";//ярко-синий
    inline const char* brightMagenta = "\033[95m";//ярко-пурпурный
    inline const char* brightCyan = "\033[96m";//ярко-циановый
    inline const char* brightWhite = "\033[97m";//ярко-белый
    inline const char* orange = "\033[38;5;208m";//оранжевый
    inline const char* gold = "\033[38;5;220m";//золотой
    inline const char* purple = "\033[38;5;93m";//фиолетовый
    inline const char* pink = "\033[38;5;213m";//розовый
    inline const char* coral = "\033[38;5;203m";//коралловый
    inline const char* turquoise = "\033[38;5;51m";//бирюзовый
    inline const char* lime = "\033[38;5;118m";//лаймовый
    inline const char* navy = "\033[38;5;19m";//тёмно-синий
    inline const char* silver = "\033[38;5;250m";//серебряный
    inline const char* bronze = "\033[38;5;130m";//бронзовый
    inline const char* spade = "\033[37m";//пики
    inline const char* heart = "\033[38;5;196m";//черви
    inline const char* diamond = "\033[38;5;214m";//бубны
    inline const char* club = "\033[38;5;255m";//трефы
    inline const char* bold = "\033[1m";//жирный
    inline const char* dim = "\033[2m";//тусклый
    inline const char* italic = "\033[3m";//курсив
    inline const char* underline = "\033[4m";//подчёркнутый
    inline const char* blink = "\033[5m";//мигающий
    inline const char* inverse = "\033[7m";//инвертированный
    inline const char* strikethrough = "\033[9m";//зачёркнутый
    inline const char* bgRed = "\033[41m";//красный фон
    inline const char* bgGreen = "\033[42m";//зелёный фон
    inline const char* bgYellow = "\033[43m";//жёлтый фон
    inline const char* bgBlue = "\033[44m";//синий фон
    inline const char* bgMagenta = "\033[45m";//пурпурный фон
    inline const char* bgCyan = "\033[46m";//циановый фон
    inline const char* bgWhite = "\033[47m";//белый фон
    inline const char* bgBlack = "\033[40m";//чёрный фон
    inline const char* bgGold = "\033[48;5;220m";//золотой фон
}
void pause();//пауза до энтера
int showMainMenu();//главное меню
void showRules();//правила игры
void endGame();//конец игры
void showHand(const CustomStack<Card>& hand);//показ руки
void showTable(const Card table[], int revealed);//показ стола
void drawProgressBar(int current, int max, int barWidth = 20, const char* label = nullptr);//прогресс-бар
void drawBoxedHeader(const char* title);//рамка для заголовка
void drawBoxedContent(const char* content);//рамка для контента
void animateCardReveal();//анимация карты
void showCard(const Card& card, bool faceDown = false);//показ одной карты